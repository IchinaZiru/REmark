import os
import glob
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# ---------- 設定 ----------
ROOT = os.path.dirname(os.path.dirname(__file__))      # REmark-Master/
MCQA_DIR = os.path.join(ROOT, "outputs", "mcqa_results")
OUT_DIR  = os.path.join(ROOT, "outputs", "mcqa_results")
os.makedirs(OUT_DIR, exist_ok=True)
TOP_N = 10   # 難問関数ランキング数
# --------------------------

def load_mcqa_results():
    """L0～L7 の scored.csv を全部読み込んで連結"""
    all_csv = glob.glob(os.path.join(MCQA_DIR, "L*_scored.csv"))
    dfs = []
    for path in all_csv:
        level = os.path.basename(path).split("_")[0]  # "L0" など
        df = pd.read_csv(path)                       # cols: function, is_correct, ...
        df["level"] = level
        dfs.append(df[["function", "is_correct", "level"]])
    return pd.concat(dfs, ignore_index=True)

def pivot_accuracy(df):
    """関数 × レベル の正答率ピボットテーブル"""
    return (
        df.groupby(["function", "level"])["is_correct"]
          .mean()                       # True/False → 1.0 / 0.0
          .reset_index()
          .pivot(index="function", columns="level", values="is_correct")
          .sort_index()
    )

def heatmap_accuracy(pivot_df):
    plt.figure(figsize=(12, max(6, len(pivot_df)*0.25)))
    sns.heatmap(
        pivot_df, annot=True, fmt=".2f", cmap="YlGnBu",
        cbar_kws=dict(label="MCQA Accuracy")
    )
    plt.title("MCQA Accuracy per Function and Comment Level")
    plt.xlabel("Comment Level")
    plt.ylabel("Function")
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "mcqa_heatmap_function_level.png"))
    plt.close()

def small_multiples_line(pivot_df):
    """各関数の折れ線を小分割プロットで描画"""
    ordered_levels = sorted(pivot_df.columns, key=lambda x: int(x[1:]))
    n_funcs = len(pivot_df)
    ncols = 4
    nrows = int(np.ceil(n_funcs / ncols))
    fig, axes = plt.subplots(nrows, ncols, figsize=(ncols*3, nrows*2), sharey=True)
    axes = axes.flatten()

    for ax, (func, row) in zip(axes, pivot_df.iterrows()):
        ax.plot(ordered_levels, row[ordered_levels], marker="o")
        ax.set_title(func, fontsize=8)
        ax.set_ylim(0, 1)
        ax.set_xticks(ordered_levels)
        ax.set_xticklabels(ordered_levels, rotation=90, fontsize=6)
        ax.grid(alpha=0.3)

    # 残りの空白サブプロットを非表示
    for ax in axes[n_funcs:]:
        ax.axis("off")

    fig.suptitle("MCQA Accuracy Trend per Function (L0→L7)", fontsize=14)
    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(os.path.join(OUT_DIR, "mcqa_function_smallmultiples.png"))
    plt.close()

def hardest_functions_bar(pivot_df):
    """平均正答率が低い関数をランキング表示"""
    avg_scores = pivot_df.mean(axis=1).sort_values()[:TOP_N]
    plt.figure(figsize=(8, TOP_N*0.4 + 1))
    sns.barplot(x=avg_scores.values, y=avg_scores.index, palette="rocket_r")
    plt.xlabel("Average MCQA Accuracy")
    plt.title(f"Hardest Functions (Bottom {TOP_N})")
    plt.xlim(0, 1)
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "mcqa_hardest_functions.png"))
    plt.close()

def main():
    df = load_mcqa_results()
    pivot_df = pivot_accuracy(df)
    heatmap_accuracy(pivot_df)
    small_multiples_line(pivot_df)
    hardest_functions_bar(pivot_df)
    print(f"3 つの可視化を {OUT_DIR} に保存しました")

if __name__ == "__main__":
    main()
