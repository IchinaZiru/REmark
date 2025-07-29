"""
metrics_analysis.py

A: メトリック相関ヒートマップ
B: 重回帰による動的重み推定＆比較プロット
C: 外れ値検出
D: カテゴリ別統合スコア箱ひげ図
E: コメントレベル進化曲線
F: メタメトリック（パーセンタイル平均）＆分布
"""

import os
import glob
import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import seaborn as sns
import platform

# -------------------------------
# 日本語フォント設定
# -------------------------------
system = platform.system()
if system == 'Darwin':  # macOS
    font_family = 'AppleGothic'
elif system == 'Windows':  # Windows
    font_family = 'MS Gothic'
else:  # Linux
    font_family = 'IPAPGothic'  # または適切な日本語フォント

# matplotlib および seaborn にフォントを設定
plt.rcParams['font.family'] = font_family
plt.rcParams['font.sans-serif'] = [font_family]
plt.rcParams['axes.unicode_minus'] = False  # マイナス記号の文字化け防止
sns.set(style='whitegrid', font=font_family)

# ──────────────────────────
# Path configuration
# ──────────────────────────
PROJECT_ROOT    = os.path.dirname(os.path.dirname(__file__))
MERGED_CSV_PATH = os.path.join(PROJECT_ROOT, "outputs", "merged_results", "merged_scores.csv")
FUNC_CSV_PATH   = os.path.join(PROJECT_ROOT, "data", "csv", "functions.csv")
OUTPUT_DIR      = os.path.join(PROJECT_ROOT, "outputs", "merged_results")
os.makedirs(OUTPUT_DIR, exist_ok=True)

# ──────────────────────────
# 0. Load data
# ──────────────────────────
df = pd.read_csv(MERGED_CSV_PATH)
df["MCQA"] = df["mcqa"].astype(int)
if "integrated_score" not in df.columns:
    df["統合スコア"] = (df["MCQA"] + df["bleurt"] + df["sbc"]) / 3

# load function metadata
func_meta = pd.read_csv(FUNC_CSV_PATH)
if "関数名" in func_meta.columns:
    func_meta = func_meta.rename(columns={"関数名":"function"})
else:
    raise ValueError("functions.csv に「関数名」列が見つかりません")
if "category" not in func_meta.columns:
    func_meta["category"] = "Unknown"

METRICS = ["MCQA","bleurt","sbc","統合スコア"]

# ──────────────────────────
# A. Correlation heatmap
# ──────────────────────────
corr = df[METRICS].corr()
plt.figure(figsize=(6,5))
sns.heatmap(corr, annot=True, cmap="coolwarm", vmin=-1, vmax=1)
plt.title("指標間の相関ヒートマップ")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "correlation_heatmap.png"))
plt.close()

# ──────────────────────────
# B. Weighted regression
# ──────────────────────────
X = df[["MCQA","bleurt","sbc"]]
y = df["統合スコア"]
model = LinearRegression().fit(X, y)
weights = dict(zip(["MCQA","bleurt","sbc"], model.coef_))
pd.Series(weights).to_csv(os.path.join(OUTPUT_DIR,"regression_weights.csv"), header=["weight"])

df["weighted_score"] = model.predict(X)
df[["function","level","weighted_score"]].to_csv(
    os.path.join(OUTPUT_DIR,"weighted_scores.csv"), index=False)

plt.figure(figsize=(6,4))
sns.scatterplot(x="統合スコア", y="weighted_score", data=df, alpha=0.5)
plt.plot([0,1],[0,1],"r--")
plt.xlabel("元の統合スコア")
plt.ylabel("重回帰で算出したスコア")
plt.title("元スコアと重み付きスコアの比較")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR, "weighted_score_scatter.png"))
plt.close()

# ──────────────────────────
# C. Outlier detection
# ──────────────────────────
outliers = []
for m in METRICS:
    q1 = df[m].quantile(0.25)
    q3 = df[m].quantile(0.75)
    iqr = q3 - q1
    low = q1 - 1.5*iqr
    high= q3 + 1.5*iqr
    mask = (df[m]<low)|(df[m]>high)
    tmp = df.loc[mask, ["function","level",m]].copy()
    tmp["metric"]=m
    outliers.append(tmp)
outliers_df = pd.concat(outliers, ignore_index=True)
outliers_df.to_csv(os.path.join(OUTPUT_DIR,"outlier_functions.csv"), index=False)

# ──────────────────────────
# D. Category boxplot
# ──────────────────────────
merged = df.merge(func_meta[["function","category"]], on="function", how="left")
plt.figure(figsize=(8,6))
sns.boxplot(x="category", y="統合スコア", data=merged)
plt.xticks(rotation=45)
plt.xlabel("カテゴリ")
plt.ylabel("統合スコア")
plt.title("カテゴリ別の統合スコア分布")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR,"category_boxplot.png"))
plt.close()

# ──────────────────────────
# E. Evolution curves
# ──────────────────────────
pivot = df.pivot_table(index="level", values=METRICS, aggfunc="mean")
pivot = pivot.sort_index(key=lambda idx: idx.str.slice(1).astype(int))
plt.figure(figsize=(8,5))
for m in METRICS:
    plt.plot(pivot.index, pivot[m], marker="o", label=m)
plt.xlabel("コメントレベル")
plt.ylabel("平均スコア")
plt.title("コメントレベルごとのメトリック推移")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR,"evolution_curves.png"))
plt.close()

# ──────────────────────────
# F. Meta-metric percentile
# ──────────────────────────
for m in METRICS:
    df[f"{m}_pct"] = df[m].rank(pct=True)
pct_cols = [f"{m}_pct" for m in METRICS]
df["meta_score"] = df[pct_cols].mean(axis=1)
df[["function","level","meta_score"]].to_csv(
    os.path.join(OUTPUT_DIR,"meta_scores.csv"), index=False)

plt.figure(figsize=(6,4))
sns.histplot(df["meta_score"], bins=20, kde=True)
plt.xlabel("メタスコア")
plt.ylabel("件数")
plt.title("メタスコアの分布")
plt.tight_layout()
plt.savefig(os.path.join(OUTPUT_DIR,"meta_score_dist.png"))
plt.close()

print("Analysis complete. Outputs in:", OUTPUT_DIR)