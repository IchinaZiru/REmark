import os
import pandas as pd
import matplotlib.pyplot as plt

# -------------------------
# 設定
# -------------------------
RESULT_DIR = "outputs/mcqa_results"
LEVELS = list(range(0, 8))  # L0〜L7

scores = []
for level in LEVELS:
    score_path = os.path.join(RESULT_DIR, f"L{level}_scored.csv")
    if not os.path.exists(score_path):
        print(f"スコアファイルがありません: {score_path}")
        scores.append(None)
        continue

    df = pd.read_csv(score_path)
    acc = df["is_correct"].mean()
    scores.append(acc)
    print(f"L{level}: 正答率 = {acc:.2%}")

# -------------------------
# グラフ表示
# -------------------------
plt.figure(figsize=(8, 5))
plt.plot(LEVELS, scores, marker='o', linestyle='-', color='blue')
plt.xticks(LEVELS)
plt.xlabel("Comment Level (L0~L7)")
plt.ylabel("Accuracy")
plt.title("MCQA Accuracy Trends")
plt.grid(True)
plt.ylim(0, 1.0)

# 保存と表示
out_path = os.path.join(RESULT_DIR, "mcqa_scores_plot.png")
plt.savefig(out_path)
plt.show()
print(f"\nグラフを保存しました: {out_path}")
