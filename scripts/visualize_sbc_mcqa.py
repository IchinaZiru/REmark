import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib
import os
import glob

# 明示的に日本語フォントを指定（Windows向け）
matplotlib.rcParams['font.family'] = 'Meiryo'

# 出力ディレクトリ
output_dir = "outputs/sbc_mcqa"
os.makedirs(output_dir, exist_ok=True)

# SBC結果のCSVファイル
sbc_csv_paths = sorted(glob.glob("outputs/sbc_results/L[0-7].csv"))

# mcqa結果（統合ファイル）
mcqa_path = "outputs/merged_results/merged_scores.csv"

# SBCデータ読み込み
sbc_dataframes = []
for path in sbc_csv_paths:
    level = path.split("/")[-1].replace(".csv", "")
    df = pd.read_csv(path)
    df["level"] = level
    sbc_dataframes.append(df)

sbc_all = pd.concat(sbc_dataframes, ignore_index=True)

# mcqaデータ読み込み
df_mcqa = pd.read_csv(mcqa_path)[["id", "mcqa"]]

# SBCとMCQAを結合
sbc_merged = pd.merge(sbc_all, df_mcqa, on="id", how="left")
sbc_merged = sbc_merged.dropna(subset=["mcqa"])

# スコア列
score_columns = ["f1_score", "semantic_similarity", "bleu", "completeness"]
score_labels = {
    "f1_score": "F1スコア (F1 Score)",
    "semantic_similarity": "意味的類似度 (Semantic Similarity)",
    "bleu": "BLEUスコア (BLEU)",
    "completeness": "網羅性 (Completeness)"
}

# ---- 追加: 数値結果保存ディレクトリ ----
csv_output_dir = os.path.join(output_dir, "csv_results")
os.makedirs(csv_output_dir, exist_ok=True)

# 1. 全体平均の比較
avg_scores = sbc_merged.groupby("mcqa")[score_columns].mean().T
avg_scores.columns = ["mcqa=False", "mcqa=True"]
avg_scores.index = [score_labels[col] for col in avg_scores.index]

# 保存
avg_scores.to_csv(os.path.join(csv_output_dir, "overall_score_comparison.csv"))

plt.figure(figsize=(10, 6))
avg_scores.plot(kind='bar')
plt.title("SBCスコア平均の比較（mcqa=True vs False）")
plt.ylabel("平均スコア")
plt.ylim(0, 1.05)
plt.xticks(rotation=45)
plt.grid(axis='y')
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "overall_score_comparison.png"))
plt.close()

# 2. フィールド別の平均比較
for score in score_columns:
    fieldwise = sbc_merged.groupby(["field", "mcqa"])[score].mean().unstack()
    fieldwise.to_csv(os.path.join(csv_output_dir, f"fieldwise_{score}.csv"))

    plt.figure(figsize=(8, 5))
    sns.barplot(data=sbc_merged, x="field", y=score, hue="mcqa", ci="sd")
    plt.title(f"{score_labels[score]} のフィールド別比較（mcqa単位）")
    plt.ylim(0, 1.05)
    plt.ylabel("平均スコア")
    plt.xlabel("フィールド")
    plt.legend(title="mcqa")
    plt.tight_layout()
    filename = f"fieldwise_{score}.png"
    plt.savefig(os.path.join(output_dir, filename))
    plt.close()

print("画像とCSVが outputs/sbc_mcqa に保存されました。")
