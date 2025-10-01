import argparse
import pandas as pd
from pathlib import Path
import re

BASE_DIR = Path("outputs/mcqa_results")

def main():
    parser = argparse.ArgumentParser(
        description="Merge split MCQA result CSVs into one, drop intermediate TOTAL_ELAPSED_TIME rows"
    )
    parser.add_argument(
        "--dir",
        required=True,
        help="モデル結果のディレクトリ名 (例: gpt-4o, gpt-4o-mini)"
    )
    parser.add_argument(
        "--output",
        default="L99_merged.csv",
        help="統合後のCSVファイル名 (default: L99_merged.csv)"
    )
    args = parser.parse_args()

    target_dir = BASE_DIR / args.dir
    if not target_dir.exists():
        raise FileNotFoundError(f"ディレクトリが存在しません: {target_dir}")

    # off番号順にソート
    files = sorted(
        target_dir.glob("L99_off*_lim*.csv"),
        key=lambda p: int(re.search(r"off(\d+)", p.stem).group(1))
    )
    if not files:
        raise RuntimeError(f"{target_dir} に L99_off*_lim*.csv が見つかりませんでした。")

    print(f"[INFO] {len(files)} ファイルを統合します ({args.dir})")

    dfs = []
    for f in files:
        df = pd.read_csv(f)
        # 中間の TOTAL_ELAPSED_TIME 行を削除
        df = df[df["function"] != "TOTAL_ELAPSED_TIME(sec)"]
        dfs.append(df)

    merged = pd.concat(dfs, ignore_index=True)

    # 必要カラムだけに揃える
    merged = merged[["function", "answer", "elapsed_time_sec"]]

    # 合計時間を計算して TOTAL 行を追加
    total_time = merged["elapsed_time_sec"].sum()
    
    blank_row = pd.DataFrame([{
        "function": "",
        "answer": "",
        "elapsed_time_sec": ""
    }])
        
    summary_row = pd.DataFrame([{
        "function": "TOTAL_ELAPSED_TIME(sec)",
        "answer": round(total_time, 2),
        "elapsed_time_sec": ""
    }])
    final_df = pd.concat([merged, blank_row, summary_row], ignore_index=True)
    out_path = target_dir / args.output
    final_df.to_csv(out_path, index=False, encoding="utf-8-sig")

    print(f"[INFO] 統合完了 → {out_path}")

if __name__ == "__main__":
    main()
