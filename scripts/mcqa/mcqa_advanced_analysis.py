import os
import glob
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats

ROOT = os.path.dirname(os.path.dirname(__file__))        # REmark-Master/
MCQA_DIR = os.path.join(ROOT, "outputs", "mcqa_results")
FUNC_CSV = os.path.join(ROOT, "data", "csv", "functions.csv")
OUT_DIR  = os.path.join(ROOT, "outputs", "mcqa_results")
os.makedirs(OUT_DIR, exist_ok=True)

# -------------------------------
def load_mcqa():
    """連結 + レベル列追加"""
    dfs = []
    for csv in glob.glob(os.path.join(MCQA_DIR, "L*_scored.csv")):
        level = os.path.basename(csv).split("_")[0]  # L0〜L7
        df = pd.read_csv(csv)
        df["level"] = level
        dfs.append(df)
    return pd.concat(dfs, ignore_index=True)

def prepare_func_meta():
    """functions.csv を読み込み、列名を正規化"""
    func_meta = pd.read_csv(FUNC_CSV)

    # 関数名列をリネーム
    if "関数名" in func_meta.columns:
        func_meta = func_meta.rename(columns={"関数名": "function"})
    elif "Function Name" in func_meta.columns:
        func_meta = func_meta.rename(columns={"Function Name": "function"})
    else:
        raise ValueError("functions.csv に function 列が見つかりません")

    # カテゴリ列が無い場合は Unknown を補う
    if "category" not in func_meta.columns:
        func_meta["category"] = "Unknown"

    return func_meta

def category_bar(df, func_meta):
    """カテゴリ別正答率バー"""
    merged = df.merge(func_meta, on="function", how="left")
    cat_acc = merged.groupby("category")["is_correct"].mean().sort_values()
    plt.figure(figsize=(8, 4 + 0.4*len(cat_acc)))
    sns.barplot(x=cat_acc.values, y=cat_acc.index, palette="crest")
    plt.xlabel("MCQA Accuracy")
    plt.title("Category‑wise MCQA Accuracy")
    plt.xlim(0, 1)
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "mcqa_category_bar.png"))
    plt.close()

def level_effect_curve(df):
    """コメントレベル別平均 + 信頼区間バンド"""
    df["mcqa"] = df["is_correct"].astype(int)
    stats_df = (
        df.groupby("level")["mcqa"]
          .agg(["mean", "count"])
          .reset_index()
          .sort_values("level", key=lambda col: col.str.slice(1).astype(int))
    )
    # 95% CI for proportion: CI = p ± 1.96 * sqrt(p(1-p)/n)
    stats_df["ci"] = 1.96 * np.sqrt(stats_df["mean"] * (1 - stats_df["mean"]) / stats_df["count"])
    ordered_levels = stats_df["level"]

    plt.figure(figsize=(8, 5))
    plt.plot(ordered_levels, stats_df["mean"], marker="o", color="royalblue")
    plt.fill_between(
        ordered_levels,
        stats_df["mean"] - stats_df["ci"],
        stats_df["mean"] + stats_df["ci"],
        alpha=0.2,
        color="royalblue"
    )
    plt.ylim(0, 1)
    plt.title("Comment‑Level Effect on MCQA Accuracy")
    plt.xlabel("Comment Level")
    plt.ylabel("Accuracy ±95% CI")
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "mcqa_comment_level_effect.png"))
    plt.close()

def hard_questions(df, top_n=10):
    """誤答が多い設問 TOP‑N"""
    if "question_id" not in df.columns:
        print("question_id 列が無いため難問分析をスキップします")
        return

    hard = (
        df.groupby("question_id")["is_correct"]
          .mean()
          .sort_values()
          .head(top_n)
    )
    plt.figure(figsize=(8, 0.4*top_n + 2))
    sns.barplot(x=(1-hard).values, y=hard.index, palette="rocket_r")
    plt.xlabel("Error Rate (1 − accuracy)")
    plt.title(f"Hardest Questions (Top {top_n})")
    plt.xlim(0, 1)
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "mcqa_hard_questions.png"))
    plt.close()

# -------------------------------
def main():
    mcqa_df = load_mcqa()
    func_meta = prepare_func_meta()
    category_bar(mcqa_df, func_meta)
    level_effect_curve(mcqa_df)
    hard_questions(mcqa_df, top_n=15)
    print(f"Analysis done. Check {OUT_DIR}")

if __name__ == "__main__":
    main()
