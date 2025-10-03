import os, json, argparse
import pandas as pd

# CLI 引数
parser = argparse.ArgumentParser(description="MCQA結果を採点")
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
args = parser.parse_args()
LEVEL = args.level

# パス設定
MCQA_CSV = f"outputs/mcqa_results/L{LEVEL}.csv"
POOL_DIR = "data/mcqa_pools"

# 読み込み
df = pd.read_csv(MCQA_CSV)
results = []

for _, row in df.iterrows():
    func_name = row["function"]
    predicted = row["answer"]

    # 正解インデックス取得
    pool_path = os.path.join(POOL_DIR, f"{func_name}.json")
    if not os.path.exists(pool_path):
        print(f"プールが見つかりません: {pool_path}")
        continue

    with open(pool_path, encoding="utf-8") as f:
        pool = json.load(f)

    correct_index = pool["answer_index"]
    correct_letter = chr(65 + correct_index)  # 例: 0 → A, 1 → B...

    is_correct = predicted.strip().upper() == correct_letter
    results.append({
        "function": func_name,
        "predicted": predicted,
        "correct": correct_letter,
        "is_correct": is_correct,
    })

# DataFrame化・保存
results_df = pd.DataFrame(results)
out_path = f"outputs/mcqa_results/L{LEVEL}_scored.csv"
os.makedirs(os.path.dirname(out_path), exist_ok=True)
results_df.to_csv(out_path, index=False)
accuracy = results_df["is_correct"].mean()

print(f"\n採点完了: {out_path}")
print(f"正答率 (L{LEVEL}): {accuracy:.2%}")
