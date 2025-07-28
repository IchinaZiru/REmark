import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import plotly.express as px

# ======================
# データ読み込み
# ======================
df = pd.read_csv("outputs/merged_results/merged_scores.csv")
df["mcqa_score"] = df["mcqa"].astype(int)  # True/False → 1/0
df["integrated_score"] = (df["mcqa_score"] + df["bleurt"] + df["sbc"]) / 3

output_dir = "outputs/visualizations"
os.makedirs(output_dir, exist_ok=True)

# ======================
# 1️ コメントレベル別平均スコア
# ======================
avg_scores = df.groupby("level")[["mcqa_score", "bleurt", "sbc", "integrated_score"]].mean()

plt.figure(figsize=(10, 6))
avg_scores.plot(kind="bar")
plt.title("Average Scores by Comment Level")
plt.ylabel("Score")
plt.xticks(rotation=0)
plt.legend(title="Metric")
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "avg_scores_by_level.png"))
plt.close()

# ======================
# 2️ 関数ごとのヒートマップ（BLEURT）
# ======================
heatmap_data = df.pivot_table(index="function", columns="level", values="bleurt", aggfunc=np.mean)

plt.figure(figsize=(14, 10))
sns.heatmap(heatmap_data, cmap="coolwarm", annot=False)
plt.title("BLEURT Score Heatmap (Function x Comment Level)")
plt.ylabel("Function")
plt.xlabel("Comment Level")
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "heatmap_bleurt.png"))
plt.close()

# ======================
# 3️ 3D散布図（MCQA × BLEURT × SBC）
# ======================
fig_3d = px.scatter_3d(
    df,
    x="bleurt",
    y="sbc",
    z="mcqa_score",
    color="level",
    size="integrated_score",  # MCQAも含めた統合スコアで点サイズ
    hover_name="function",
    opacity=0.7,
    color_discrete_sequence=px.colors.qualitative.Vivid
)
fig_3d.update_layout(
    title="3D Scatter: BLEURT vs SBC vs MCQA",
    scene=dict(
        xaxis_title="BLEURT (Semantic Similarity)",
        yaxis_title="SBC (Structural Accuracy)",
        zaxis_title="MCQA (0=Incorrect, 1=Correct)"
    )
)
fig_3d.write_html("outputs/visualizations/3d_scatter_plot.html")

# ======================
# 4️ コメントレベル別の統合スコア箱ひげ図
# ======================
plt.figure(figsize=(10, 6))
sns.boxplot(x="level", y="integrated_score", data=df)
plt.title("Integrated Score Distribution by Comment Level")
plt.ylabel("Integrated Score")
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "boxplot_integrated_score.png"))
plt.close()

# ======================
# 5️ コメントレベル順の進化グラフ
# ======================
ordered_levels = sorted(df["level"].unique(), key=lambda x: int(x[1:]))
avg_progress = avg_scores.loc[ordered_levels]

plt.figure(figsize=(10, 6))
plt.plot(avg_progress.index, avg_progress["integrated_score"], marker="o")
plt.title("Integrated Score Progression (L0 → L7)")
plt.xlabel("Comment Level")
plt.ylabel("Integrated Score")
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "score_progression.png"))
plt.close()

print(f"Visualization completed. Files saved in: {output_dir}")
