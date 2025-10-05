#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Build per-level .c snippets that contain only the misunderstood functions:
'leading comment block' (contiguous // lines just above the function) + the function body.

Inputs:
- outputs/misunderstood_functions/<model>/L*_wrong_functions.csv
  (columns must include at least: level,function)
- data/csv/functions.csv (列: 「関数名」「開始行」「終了行」など; 日本語列名対応)
- data/levels/*_L{level}.c (original level sources)

Outputs:
- mcqa/extracted/<model>/L{level}_wrong_snippets.c
- mcqa/extracted/<model>/L{level}_index.md  … 収録関数の目次

Encoding:
- Windows Excel/エディタでの文字化け回避のため UTF-8 BOM (utf-8-sig) で保存
"""

from pathlib import Path
import argparse
import re
import csv
import pandas as pd
import re

JP_FUNC_COLS = ["関数名", "開始行", "終了行"]
EN_FUNC_COLS = ["function", "start_line", "end_line"]
FUNC_HEAD_PREFIX = r'^[\t ]*[A-Za-z_][\w\s\*\(\),\[\]]+\b'

def find_function_span(src_text: str, func_name: str):
    """
    指定関数の本体ブロック（{ ... }）の行範囲を (start_line, end_line) で返す（1-based）。
    見つからなければ None。
    """
    # 関数ヘッダ（{ の直前まで）にマッチ
    head = re.compile(
        FUNC_HEAD_PREFIX + re.escape(func_name) + r'\s*\([^;]*\)\s*' + r'\{',
        re.MULTILINE
    )
    m = head.search(src_text)
    if not m:
        return None

    start_pos = m.start()
    start_line = src_text[:start_pos].count("\n") + 1

    # ブレースカウントで対応する '}' を探す
    brace = 0
    i = m.start()
    n = len(src_text)
    while i < n:
        ch = src_text[i]
        if ch == '{':
            brace += 1
        elif ch == '}':
            brace -= 1
            if brace == 0:
                end_line = src_text[:i].count("\n") + 1
                return (start_line, end_line)
        i += 1
    return None

def read_functions_csv(path: Path) -> pd.DataFrame:
    # 日本語カラム/英語カラムのどちらでも可
    df = pd.read_csv(path, dtype=str, keep_default_na=False, encoding="utf-8")
    cols = [c.strip() for c in df.columns]
    df.columns = cols
    if set(JP_FUNC_COLS).issubset(cols):
        df = df.rename(columns={"関数名":"function", "開始行":"start_line", "終了行":"end_line"})
    elif set(EN_FUNC_COLS).issubset(cols):
        pass
    else:
        raise SystemExit(f"functions.csv に必要な列がありません: {JP_FUNC_COLS} または {EN_FUNC_COLS}")
    # 数値列整形
    df["start_line"] = df["start_line"].astype(int)
    df["end_line"]   = df["end_line"].astype(int)
    return df[["function","start_line","end_line"]]

def find_source_file(levels_dir: Path, level_tag: str) -> Path | None:
    # e.g. *_L7.c / *_L99.c
    pats = [f"*_{level_tag}.c", f"*_{level_tag.upper()}.c"]
    for pat in pats:
        hits = list(levels_dir.glob(pat)) or list(levels_dir.rglob(pat))
        if hits:
            return hits[0]
    return None

def extract_leading_comment(lines: list[str], start_idx0: int) -> list[str]:
    """
    直前コメント（// が連続したブロック + 空行）を上に遡って抽出。
    ブロック外の /* */ は扱わず（元データが // ベースのため）。
    """
    res: list[str] = []
    j = start_idx0 - 1
    saw_comment = False
    while j >= 0:
        s = lines[j].rstrip("\n")
        st = s.strip()
        if st.startswith("//"):
            res.insert(0, s)
            saw_comment = True
            j -= 1
            continue
        if saw_comment and st == "":
            res.insert(0, s)
            j -= 1
            continue
        if saw_comment:
            break
        if not saw_comment and st == "":
            j -= 1
            continue
        break
    # 余分な空行をトリム
    while res and res[0].strip() == "":
        res.pop(0)
    while res and res[-1].strip() == "":
        res.pop()
    return res

def build_snippet_for_function(src_text: str, start_line: int, end_line: int) -> str:
    """
    start_line/end_line は 1-based（functions.csv 基準）。
    関数本体（start..end）と、その直前コメントを結合して返す。
    """
    lines = src_text.splitlines()
    n = len(lines)
    s0 = max(0, start_line - 1)
    e0 = min(n - 1, end_line - 1)
    body = lines[s0:e0+1]
    leading = extract_leading_comment(lines, s0)
    snippet = []
    if leading:
        snippet.extend(leading)
        if leading and (not leading[-1].strip() == ""):
            snippet.append("")  # コメントと関数の間に空行
    snippet.extend(body)
    return "\n".join(snippet).rstrip() + "\n"

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default=".", type=str, help="project root")
    ap.add_argument("--levels-dir", default="data/levels", type=str)
    ap.add_argument("--functions-csv", default="data/csv/functions.csv", type=str)
    ap.add_argument("--wrong-dir", default="outputs/misunderstood_functions", type=str,
                    help="…/<model>/L*_wrong_functions.csv がある親ディレクトリ")
    ap.add_argument("--model", required=True, type=str, help="例: gpt-4o")
    ap.add_argument("--outdir", default="mcqa/extracted", type=str, help="出力先ルート")
    ap.add_argument("--dedup", action="store_true", help="同一関数の重複収録を抑止")
    args = ap.parse_args()

    root = Path(args.root)
    levels_dir = root / args.levels_dir
    func_csv = root / args.functions_csv
    wrong_base = root / args.wrong_dir / args.model
    out_root = root / "data/mcqa_levels"
    out_root.mkdir(parents=True, exist_ok=True)

    if not wrong_base.exists():
        raise SystemExit(f"Not found: {wrong_base}")
    if not func_csv.exists():
        raise SystemExit(f"Not found: {func_csv}")
    func_df = read_functions_csv(func_csv)

    # L*_wrong_functions.csv を集めて L0..L7..L99 の順に
    files = sorted(wrong_base.glob("L*_wrong_functions.csv"),
                   key=lambda p: (int(p.stem.split("_")[0][1:]) if p.stem.split("_")[0][1:].isdigit() else 999, p.name))

    for csv_path in files:
        level_tag = csv_path.stem.split("_")[0]  # "L0", "L7", "L99"
        src_file = find_source_file(levels_dir, level_tag)
        if not src_file:
            print(f"[WARN] source for {level_tag} not found under {levels_dir}; skip")
            continue
        src_text = src_file.read_text(encoding="utf-8", errors="ignore")

        df = pd.read_csv(csv_path, dtype=str, keep_default_na=False, encoding="utf-8")
        if "function" not in df.columns:
            print(f"[WARN] {csv_path} に function 列がありません；skip")
            continue

        funcs = df["function"].dropna().astype(str).tolist()
        if args.dedup:
            funcs = list(dict.fromkeys(funcs))  # 重複排除（順序維持）

        # 出力組み立て
        out_lines = []
        index_lines = [f"# Index for {level_tag} ({args.model})", ""]
        out_lines.append(f"// === Extracted misunderstood snippets: {level_tag} / {args.model} ===")
        out_lines.append(f"// source: {src_file}")
        out_lines.append("")

        for fn in funcs:
            span = find_function_span(src_text, fn)
            if not span:
                if func_df[func_df["function"] == fn].empty:
                    print(f"[WARN] {level_tag}: function not found by parser and not in functions.csv: {fn}")
                else:
                    print(f"[WARN] {level_tag}: function not found by parser: {fn}")
                continue
            start_line, end_line = span
            snippet = build_snippet_for_function(src_text, start_line, end_line)


            out_lines.append(snippet.rstrip())
            out_lines.append("")  # 関数間の空行

            index_lines.append(f"- `{fn}` ({start_line}-{end_line})")

        level_dir = (root / "data/mcqa_levels" / level_tag)
        level_dir.mkdir(parents=True, exist_ok=True)

        out_c = level_dir / f"L{level_tag[1:]}.c"
        out_md = level_dir / "index.md"

        out_c.write_text("\n".join(out_lines) + "\n", encoding="utf-8-sig")
        out_md.write_text("\n".join(index_lines) + "\n", encoding="utf-8-sig")
        print(f"[OK] wrote: {out_c}")
        print(f"[OK] wrote: {out_md}")

if __name__ == "__main__":
    main()
