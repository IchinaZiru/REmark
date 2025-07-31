import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import platform
from tqdm import tqdm

# -------------------------------
# 日本語フォント設定
# -------------------------------
system = platform.system()
if system == 'Darwin':
    font_family = 'AppleGothic'
elif system == 'Windows':
    font_family = 'MS Gothic'
else:
    font_family = 'IPAPGothic'

plt.rcParams['font.family'] = font_family
plt.rcParams['font.sans-serif'] = [font_family]
plt.rcParams['axes.unicode_minus'] = False
sns.set(style='whitegrid', font=font_family)

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
OUTPUT_DIR = os.path.join(BASE_DIR, "outputs", "mcqa_qualities")
os.makedirs(OUTPUT_DIR, exist_ok=True)

# データ読み込み
df = pd.read_csv(os.path.join(BASE_DIR, "outputs", "mcqa_quality_report.csv"))

# -------------------------------
# 既存グラフ
# -------------------------------
plt.figure(figsize=(8, 5))
sns.histplot(df["avg_bleurt"], bins=20, kde=True)
plt.title("平均BLEURTスコア分布")
plt.xlabel("BLEURT Score")
plt.ylabel("Count")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "bleurt_distribution.png"))

plt.figure(figsize=(8, 5))
sns.histplot(df["avg_sim"], bins=20, kde=True)
plt.title("平均類似度分布")
plt.xlabel("Sentence-BERT Similarity")
plt.ylabel("Count")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "similarity_distribution.png"))

top_duplicates = df.sort_values("near_duplicates", ascending=False).head(10)
plt.figure(figsize=(10, 5))
sns.barplot(data=top_duplicates, x="file", y="near_duplicates", palette="Reds_r")
plt.xticks(rotation=45, ha="right")
plt.title("重複選択肢が多い問題 TOP 10")
plt.xlabel("File")
plt.ylabel("Near Duplicates")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "top10_near_duplicates.png"))

plt.figure(figsize=(6, 5))
sns.heatmap(df[["avg_sim", "avg_bleurt"]].corr(), annot=True, cmap="coolwarm", vmin=-1, vmax=1)
plt.title("類似度とBLEURTの相関")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "correlation_heatmap.png"))

# -------------------------------
# 関数別 可視化
# -------------------------------
debug_df = pd.read_csv(os.path.join(BASE_DIR, "outputs", "mcqa_quality_debug.csv"))
FUNC_DIR = os.path.join(OUTPUT_DIR, "functions")
os.makedirs(FUNC_DIR, exist_ok=True)

for file_name, group in tqdm(debug_df.groupby("file"), desc="関数別グラフ生成中"):
    fig, ax1 = plt.subplots(figsize=(8, 4))

    # 類似度 (棒グラフ)
    sns.barplot(data=group, x="choice_index", y="similarity_to_correct",
                hue="is_correct", dodge=False, ax=ax1, palette={True: "green", False: "blue"})
    ax1.set_ylabel("Similarity to Correct")
    ax1.set_xlabel("Choice Index")
    ax1.set_ylim(0, 1.05)

    # BLEURT (折れ線グラフ)
    ax2 = ax1.twinx()
    ax2.plot(group["choice_index"], group["bleurt_score"], color="red", marker="o", label="BLEURT Score")
    ax2.set_ylabel("BLEURT Score")
    ax2.set_ylim(0, 1.05)

    fig.suptitle(f"{file_name} - 選択肢ごとの類似度とBLEURT")
    ax1.legend(title="Is Correct", loc="upper left")
    ax2.legend(loc="upper right")

    plt.tight_layout()
    plt.savefig(os.path.join(FUNC_DIR, f"{file_name}_similarity_bleurt.png"))
    plt.close()

print("✔ 可視化画像を outputs/mcqa_qualities に保存しました")
