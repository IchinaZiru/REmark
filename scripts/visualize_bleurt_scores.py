import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# フォント設定（日本語対応）
plt.rcParams['font.family'] = 'MS Gothic'

# -------------------------------
# ディレクトリとファイル設定
# -------------------------------
BLEURT_DIR = "outputs/bleurt_scores"
PLOTS_DIR = os.path.join(BLEURT_DIR, "plots")
os.makedirs(PLOTS_DIR, exist_ok=True)

# -------------------------------
# 集計用の空リスト
# -------------------------------
summary = []
all_scores = []

# -------------------------------
# 各レベルのCSV読み込みと集計
# -------------------------------
for level in range(8):
    file_path = os.path.join(BLEURT_DIR, f"L{level}.csv")
    if not os.path.exists(file_path):
        print(f"ファイルが見つかりません: {file_path}")
        continue

    df = pd.read_csv(file_path, encoding="utf-8-sig")
    df["level"] = level
    all_scores.append(df)

    avg_score = df["bleurt"].mean()
    median_score = df["bleurt"].median()
    min_score = df["bleurt"].min()
    max_score = df["bleurt"].max()
    std_score = df["bleurt"].std()

    summary.append({
        "level": level,
        "mean": avg_score,
        "median": median_score,
        "min": min_score,
        "max": max_score,
        "std": std_score
    })

# -------------------------------
# DataFrame 作成
# -------------------------------
summary_df = pd.DataFrame(summary)
all_df = pd.concat(all_scores, ignore_index=True)

# -------------------------------
# コメントレベル別 BLEURT 平均スコアの可視化
# -------------------------------
plt.figure(figsize=(10, 6))
plt.plot(summary_df["level"], summary_df["mean"], marker="o", label="平均")
plt.plot(summary_df["level"], summary_df["median"], marker="s", label="中央値")
plt.fill_between(summary_df["level"],
                 summary_df["mean"] - summary_df["std"],
                 summary_df["mean"] + summary_df["std"],
                 alpha=0.2, label="± 標準偏差")

plt.title("BLEURTスコア統計（コメントレベル別）")
plt.xlabel("コメントレベル")
plt.ylabel("BLEURTスコア")
plt.grid(True)
plt.legend()
plt.xticks(summary_df["level"])
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_score_stats_plot.png"), dpi=300)
plt.close()

# -------------------------------
# コメントレベル別 BLEURT スコアの分布可視化（箱ひげ図）
# -------------------------------
all_df["level"] = all_df["level"].astype(str)
plt.figure(figsize=(10, 6))
sns.boxplot(data=all_df, x="level", y="bleurt", palette="Set3")
plt.title("BLEURTスコアの分布（箱ひげ図）")
plt.xlabel("コメントレベル")
plt.ylabel("BLEURTスコア")
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_score_boxplot.png"), dpi=300)
plt.close()

# -------------------------------
# コメントレベル別 各関数ごとのスコアの変動可視化（折れ線）
# -------------------------------
plt.figure(figsize=(12, 8))
for function in all_df["function"].unique():
    func_df = all_df[all_df["function"] == function]
    func_df = func_df.sort_values("level")
    plt.plot(func_df["level"], func_df["bleurt"], marker="o", label=function)

plt.title("関数別 BLEURTスコアの変動（コメントレベル別）")
plt.xlabel("コメントレベル")
plt.ylabel("BLEURTスコア")
plt.grid(True)
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', fontsize=8)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_function_score_lines.png"), dpi=300)
plt.close()

# -------------------------------
# BLEURTスコアの密度分布（コメントレベル別）
# -------------------------------
plt.figure(figsize=(10, 6))
for level in sorted(all_df["level"].unique(), key=int):
    sns.kdeplot(all_df[all_df["level"] == level]["bleurt"], label=f"L{level}")
plt.title("BLEURTスコアの密度分布（コメントレベル別）")
plt.xlabel("BLEURTスコア")
plt.ylabel("密度")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_density_by_level.png"), dpi=300)
plt.close()

# -------------------------------
# 関数別BLEURTスコア（コメントレベル別）ヒートマップ
# -------------------------------
pivot_df = all_df.pivot_table(index="function", columns="level", values="bleurt")
plt.figure(figsize=(12, len(pivot_df)*0.4))
sns.heatmap(pivot_df, annot=True, fmt=".3f", cmap="YlGnBu")
plt.title("関数別BLEURTスコア（コメントレベル別）")
plt.xlabel("コメントレベル")
plt.ylabel("関数名")
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_function_heatmap.png"), dpi=300)
plt.close()

# -------------------------------
# L0→L7のスコア差分（関数別）
# -------------------------------
diff_df = all_df.pivot(index="function", columns="level", values="bleurt")
diff_df["diff_L7_L0"] = diff_df["7"] - diff_df["0"]
diff_df_sorted = diff_df.sort_values("diff_L7_L0")

plt.figure(figsize=(10, 6))
plt.barh(diff_df_sorted.index, diff_df_sorted["diff_L7_L0"], color="teal")
plt.axvline(0, color='gray', linestyle='--')
plt.title("L0→L7 の BLEURTスコア変化量（関数別）")
plt.xlabel("スコア差分（L7 - L0）")
plt.ylabel("関数名")
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_diff_l7_l0.png"), dpi=300)
plt.close()

# -------------------------------
# BLEURTスコア標準偏差が高い関数
# -------------------------------
stds = all_df.groupby("function")["bleurt"].std().sort_values(ascending=False)
top_std = stds.head(10)

plt.figure(figsize=(10, 6))
top_std.plot(kind="barh", color="salmon")
plt.title("BLEURTスコアの標準偏差が大きい関数（上位10）")
plt.xlabel("標準偏差")
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "bleurt_top_std_functions.png"), dpi=300)
plt.close()

# -------------------------------
# CSV保存
# -------------------------------
summary_df.to_csv(os.path.join(PLOTS_DIR, "bleurt_summary.csv"), index=False, encoding="utf-8-sig")
print("BLEURTスコアの統計と可視化を保存しました。")
