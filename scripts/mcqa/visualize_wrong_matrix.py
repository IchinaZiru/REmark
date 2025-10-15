"""
各レベル(L0..L7, L99)で間違えた関数を可視化するマトリクスを生成し、
CSV / Markdown / PNG(ヒートマップ) を outputs/misunderstood_functions/<model>/ に出力する。

前提:
- 抽出済みファイル: outputs/misunderstood_functions/<model>/L*_wrong_functions.csv
  (列: level, model, function, ...)

使い方(改行なし例):
python scripts/visualize_wrong_matrix.py --model gpt-4o --root . --in-dir outputs/misunderstood_functions --out-name wrong_matrix
"""
import argparse
from pathlib import Path
import re
import csv
import pandas as pd
import matplotlib.pyplot as plt

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default=".", type=str, help="プロジェクトルート")
    ap.add_argument("--in-dir", default="outputs/misunderstood_functions", type=str,
                    help="抽出CSVの親ディレクトリ(=その下にモデル名ディレクトリ)")
    ap.add_argument("--model", required=True, type=str, help="対象モデル名 (例: gpt-4o)")
    ap.add_argument("--out-name", default="wrong_matrix", type=str, help="出力ファイル名のベース")
    args = ap.parse_args()

    root = Path(args.root)
    base = root / args.in_dir / args.model
    if not base.exists():
        raise SystemExit(f"Not found: {base}")

    # L*ファイルを収集
    files = sorted(base.glob("L*_wrong_functions.csv"))
    if not files:
        raise SystemExit(f"No L*_wrong_functions.csv under: {base}")

    # レベルの並び順を揃えるためのキー
    def level_key(p: Path) -> tuple:
        # L99 は最後に
        m = re.match(r"L(\d+)_wrong_functions\.csv", p.name)
        if not m:
            return (999, p.name)
        n = int(m.group(1))
        if n == 99:
            return (998, p.name)
        return (n, p.name)

    files.sort(key=level_key)

    # 関数×レベルの出現(誤答)を集計
    levels = []
    func_set = set()
    per_level_funcs = {}  # level -> list of functions
    for f in files:
        level = f.stem.split("_")[0]  # "L0", "L7", "L99"
        levels.append(level)
        df = pd.read_csv(f, dtype=str, keep_default_na=False)
        # function列のみ使用（重複があればカウントで表す）
        funcs = df["function"].dropna().astype(str).tolist() if "function" in df.columns else []
        per_level_funcs[level] = funcs
        func_set.update(funcs)
    levels = list(dict.fromkeys(levels))  # 重複除去(順序維持)
    funcs_sorted = sorted(func_set)

    # マトリクス化（値は出現カウント; 基本1。ただし同関数が同レベルで複数行ある場合は加算）
    data = []
    for fn in funcs_sorted:
        row = {"function": fn}
        for lv in levels:
            cnt = sum(1 for x in per_level_funcs.get(lv, []) if x == fn)
            row[lv] = cnt
        data.append(row)

    mat_df = pd.DataFrame(data, columns=["function"] + levels)

    # 出力: CSV (BOM付き)
    csv_path = base / f"{args.out_name}.csv"
    mat_df.to_csv(csv_path, index=False, encoding="utf-8-sig", lineterminator="\n", quoting=csv.QUOTE_ALL)

    # 出力: Markdown（○/・ で可視化、数>0で○）
    md_lines = ["# Wrong-Function Matrix", f"Model: **{args.model}**", ""]
    header = "| function | " + " | ".join(levels) + " |"
    sep    = "|" + "---|" * (len(levels)+1)
    md_lines.append(header)
    md_lines.append(sep)
    for _, r in mat_df.iterrows():
        cells = []
        for lv in levels:
            cells.append("○" if int(r[lv]) > 0 else "・")
        md_lines.append(f"| {r['function']} | " + " | ".join(cells) + " |")
    md_path = base / f"{args.out_name}.md"
    md_path.write_text("\n".join(md_lines), encoding="utf-8")

    # 出力: PNGヒートマップ（0/1/2...をカラーで可視化）
    #  依存は matplotlib のみ（seaborn不要）
    if len(funcs_sorted) and len(levels):
        plt.figure(figsize=(max(6, len(levels)*0.9), max(6, len(funcs_sorted)*0.35)))
        im_data = mat_df[levels].astype(int).values
        plt.imshow(im_data, aspect="auto")
        plt.xticks(range(len(levels)), levels, rotation=45, ha="right")
        plt.yticks(range(len(funcs_sorted)), funcs_sorted)
        plt.title(f"Wrong answers per function/level ({args.model})")
        plt.xlabel("Level")
        plt.ylabel("Function")
        plt.colorbar(label="count")
        plt.tight_layout()
        png_path = base / f"{args.out_name}.png"
        plt.savefig(png_path, dpi=200)
        plt.close()

    print(f"[OK] Wrote: {csv_path}")
    print(f"[OK] Wrote: {md_path}")
    print(f"[OK] Wrote: {base / (args.out_name + '.png')}")

if __name__ == "__main__":
    main()
