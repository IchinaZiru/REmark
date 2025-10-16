#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Extract (per level) the functions that an MCQA model got wrong and the
function-header comments immediately preceding those functions from C sources.

Assumptions (matches the user's project layout):
- Source files live under: <project_root>/data/levels and are named like:
  "..._L0.c", "..._L1.c", ..., "..._L7.c", "..._L99.c"
- Scored CSVs live under: <project_root>/outputs/mcqa_scored/<model>/
  and have filenames like "L0_scored.csv". If your scored CSVs are directly
  under outputs/mcqa_scored (no model folder), that's also supported.
- Each scored CSV has columns: ["function","predicted","correct","is_correct",...]
  where is_correct is boolean-ish.

Outputs:
- CSV files and a combined CSV under <project_root>/outputs/misunderstood_functions/<model>/
  with columns:
  ["level","model","function","predicted","correct","is_correct",
   "source_file","function_def_line","signature","leading_comment"]

Usage:
  # 例: モデル別ディレクトリがある場合
  python scripts/extract_wrong_function_comments.py \
      --project-root . \
      --levels-dir data/levels \
      --scored-root outputs/mcqa_scored \
      --models gpt-4o gpt-4o-mini \
      --outdir outputs/misunderstood_functions

  # 例: outputs/mcqa_scored 直下に L*_scored.csv がある場合
  python scripts/extract_wrong_function_comments.py \
      --project-root . \
      --levels-dir data/levels \
      --scored-root outputs/mcqa_scored \
      --outdir outputs/misunderstood_functions
"""
import argparse
import csv
import glob
import os
import re
from pathlib import Path
import pandas as pd

# 関数定義(本体)の先頭行を見つけるための正規表現
#  - プロトタイプ宣言(末尾が;)は除外し、'{‘ を含む定義のみマッチ
FUNC_DEF_RE_TMPL = r'^[\t ]*[A-Za-z_][\w\s\*\(\),\[\]]+\b{func}\s*\([^;]*\)\s*\{{'

def find_source_for_level(level_dir: Path, level_tag: str) -> Path:
    """Find a C source that ends with _L{level}.c under level_dir."""
    pattern = f"*_{level_tag}.c"
    matches = list(level_dir.glob(pattern))
    if not matches:
        matches = list(level_dir.rglob(pattern))
    return matches[0] if matches else None

def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")

def extract_comment_and_signature(source_text: str, func_name: str):
    """
    Return (start_line_index_1based, signature_text, leading_comment_text) for func_name.
    If not found, returns (None, None, None).
    """
    pattern = re.compile(FUNC_DEF_RE_TMPL.format(func=re.escape(func_name)), re.MULTILINE)
    m = pattern.search(source_text)
    if not m:
        return (None, None, None)
    # 定義開始の行番号(1-based)
    pre = source_text[:m.start()]
    start_line = pre.count("\n") + 1
    lines = source_text.splitlines()

    idx = start_line - 1  # 0-based index of the definition's starting line

    # 直前コメントを上方向へ収集（// 行の連続ブロック＋空行を許容）
    j = idx - 1
    leading = []
    saw_comment = False
    while j >= 0:
        s = lines[j].rstrip("\n")
        st = s.strip()
        if st.startswith("//"):
            leading.insert(0, s)
            saw_comment = True
            j -= 1
            continue
        if saw_comment and st == "":
            leading.insert(0, s)
            j -= 1
            continue
        if saw_comment:
            break
        if not saw_comment and st == "":
            j -= 1
            continue
        break

    while leading and leading[0].strip() == "":
        leading.pop(0)
    while leading and leading[-1].strip() == "":
        leading.pop()

    # シグネチャは '{' まで（複数行定義も対応）
    sig_lines = []
    k = idx
    while k < len(lines):
        sig_lines.append(lines[k])
        if "{" in lines[k]:
            break
        k += 1
    signature = "\n".join(sig_lines)
    leading_comment = "\n".join(leading)
    return (start_line, signature, leading_comment)

def coerce_bool(x):
    if isinstance(x, bool):
        return x
    if isinstance(x, (int, float)):
        return bool(x)
    if x is None:
        return False
    s = str(x).strip().lower()
    if s in ("true","1","yes","y"):
        return True
    if s in ("false","0","no","n"):
        return False
    return False

def load_scored_csvs(scored_root: Path, models):
    """
    Return a dict: {model_name: {level_tag: Path_to_scored_csv}}
    Supports two layouts:
      A) outputs/mcqa_scored/<model>/L7_scored.csv
      B) outputs/mcqa_scored/L7_scored.csv  (no model dir)
    """
    result = {}
    root = Path(scored_root)

    # A: モデル別サブディレクトリ
    subdirs = [p for p in root.iterdir() if p.is_dir()]
    if subdirs:
        for md in subdirs:
            model_name = md.name
            if models and model_name not in models:
                continue
            level_csvs = {}
            for p in md.glob("L*_scored.csv"):
                level_tag = p.stem.split("_")[0]  # "L7"
                level_csvs[level_tag] = p
            if level_csvs:
                result[model_name] = level_csvs

    # B: 直下に L*_scored.csv
    flat_csvs = list(root.glob("L*_scored.csv"))
    if flat_csvs:
        model_name = models[0] if models else "default"
        result[model_name] = {p.stem.split("_")[0]: p for p in flat_csvs}

    return result

def main():
    ap = argparse.ArgumentParser(description="Extract wrong-answer functions and their leading comments per level.")
    ap.add_argument("--project-root", default=".", type=str)
    ap.add_argument("--levels-dir", default="data/levels", type=str,
                    help="Directory containing level C sources (e.g. Meeting_Room_Reservation_System_L7.c).")
    ap.add_argument("--scored-root", default="outputs/mcqa_scored", type=str,
                    help="Directory containing L*_scored.csv (optionally under model subdirs).")
    ap.add_argument("--models", nargs="*", default=None, help="Optional model names to include (matches subdir names).")
    ap.add_argument("--outdir", default="outputs/misunderstood_functions", type=str)
    ap.add_argument("--include-correct", action="store_true",
                    help="If set, include correct rows as well (for completeness).")
    args = ap.parse_args()

    project_root = Path(args.project_root)
    levels_dir = project_root / args.levels_dir
    scored_root = project_root / args.scored_root
    out_root = project_root / args.outdir
    out_root.mkdir(parents=True, exist_ok=True)

    model_to_levels = load_scored_csvs(scored_root, args.models)
    if not model_to_levels:
        raise SystemExit(f"No scored CSVs found under: {scored_root}")

    for model, level_map in model_to_levels.items():
        model_out = out_root / model
        model_out.mkdir(parents=True, exist_ok=True)
        rows = []
        for level_tag, csv_path in sorted(level_map.items(), key=lambda kv: kv[0]):
            # level_tag == "L7" など
            level_num = level_tag[1:]
            src = find_source_for_level(levels_dir, level_tag)
            if not src:
                src = find_source_for_level(levels_dir, f"L{level_num}")
            if not src:
                print(f"[WARN] Source for {level_tag} not found under {levels_dir}. Skipping its comment extraction.")
                df = pd.read_csv(csv_path)
                wrong_mask = df["is_correct"].apply(coerce_bool)
                take = df if args.include_correct else df[~wrong_mask]
                for _, r in take.iterrows():
                    rows.append({
                        "level": level_tag,
                        "model": model,
                        "function": r["function"],
                        "predicted": r.get("predicted", ""),
                        "correct": r.get("correct", ""),
                        "is_correct": coerce_bool(r.get("is_correct", False)),
                        "source_file": "",
                        "function_def_line": "",
                        "signature": "",
                        "leading_comment": "",
                    })
                continue

            source_text = read_text(src)
            df = pd.read_csv(csv_path)
            df.columns = [c.strip() for c in df.columns]
            must_have = {"function","is_correct"}
            if not must_have.issubset(df.columns):
                raise SystemExit(f"CSV {csv_path} missing required columns {must_have}. Found: {df.columns.tolist()}")

            mask_wrong = ~df["is_correct"].apply(coerce_bool)
            take_df = df if args.include_correct else df[mask_wrong]

            for _, r in take_df.iterrows():
                func = str(r["function"])
                start_line, signature, leading_comment = extract_comment_and_signature(source_text, func)
                rows.append({
                    "level": level_tag,
                    "model": model,
                    "function": func,
                    "predicted": r.get("predicted", ""),
                    "correct": r.get("correct", ""),
                    "is_correct": coerce_bool(r.get("is_correct", False)),
                    "source_file": str(src.relative_to(project_root)) if src.is_absolute() and str(src).startswith(str(project_root)) else str(src),
                    "function_def_line": start_line or "",
                    "signature": signature or "",
                    "leading_comment": leading_comment or "",
                })

            # レベル別 CSV も吐く
            lvl_out = model_out / f"{level_tag}_wrong_functions.csv"
            pd.DataFrame(rows).query("level == @level_tag").to_csv(
                lvl_out,
                index=False,
                quoting=csv.QUOTE_ALL,
                encoding="utf-8-sig",
                lineterminator="\n"
            )

        # モデル横断まとめ
        all_out = model_out / "all_levels_wrong_functions.csv"
        pd.DataFrame(rows).to_csv(
            all_out,
            index=False,
            quoting=csv.QUOTE_ALL,
            encoding="utf-8-sig",
            lineterminator="\n"
        )
        print(f"[OK] Wrote: {all_out}")

        # 人間が見やすい Markdown もオプショナルで出力
        md_lines = ["# Misunderstood functions & comments",
                    f"Model: **{model}**",
                    ""]
        for level_tag in sorted({r['level'] for r in rows}):
            md_lines.append(f"## {level_tag}")
            for r in [x for x in rows if x["level"] == level_tag]:
                md_lines.append(f"### {r['function']}")
                md_lines.append("")
                md_lines.append("**Signature**")
                md_lines.append("")
                md_lines.append("```c")
                md_lines.append(r["signature"])
                md_lines.append("```")
                md_lines.append("")
                md_lines.append("**Leading comment**")
                md_lines.append("")
                md_lines.append("```")
                md_lines.append(r["leading_comment"])
                md_lines.append("```")
                md_lines.append("")
        (model_out / "wrong_functions.md").write_text("\n".join(md_lines), encoding="utf-8")

if __name__ == "__main__":
    main()
