import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
import platform
import matplotlib.font_manager as fm

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

# データ読み込み関数
def load_data(base_dir, max_level=7):
    dfs = []
    for level in range(max_level+1):
        path = os.path.join(base_dir, f'L{level}.csv')
        if os.path.exists(path):
            df = pd.read_csv(path)
            df['level'] = f'L{level}'
            dfs.append(df)
        else:
            print(f'[!] ファイルが存在しません: {path}')
    if not dfs:
        raise ValueError('評価CSVが見つかりません。')
    return pd.concat(dfs, ignore_index=True)

# ベースディレクトリからデータをロード
base_dir = 'outputs/sbc_results'
all_df = load_data(base_dir, max_level=7)

# メトリクスを melt して扱いやすく
metrics = ['semantic_similarity','bleu','completeness','f1_score','sbc']
all_melt = all_df.melt(
    id_vars=['level','function','field'],
    value_vars=metrics,
    var_name='metric', value_name='score'
)

# プロット出力用フォルダ
plots_dir = os.path.join(base_dir, 'plots')
os.makedirs(plots_dir, exist_ok=True)

# 1. 指標ごとのヒストグラム
fig, axes = plt.subplots(2, 3, figsize=(18, 10))
axes = axes.flatten()
for i, m in enumerate(metrics):
    sns.histplot(all_df[m], kde=True, ax=axes[i])
    axes[i].set_title(m, fontsize=14)
    axes[i].set_xlim(0, 1)
    axes[i].tick_params(axis='x', rotation=15)
axes[-1].axis('off')  # 余分なサブプロットを非表示
plt.tight_layout()
fig.savefig(os.path.join(plots_dir, 'histograms.png'), dpi=300, bbox_inches='tight')
plt.close(fig)

# 2. 指標間の相関行列ヒートマップ
corr = all_df[metrics].corr()
fig = plt.figure(figsize=(6, 5))
ax = sns.heatmap(corr, annot=True, vmin=0, vmax=1, cmap='Blues')
ax.set_title('指標間の相関行列', fontsize=14)
fig.savefig(os.path.join(plots_dir, 'correlation_heatmap.png'), dpi=300, bbox_inches='tight')
plt.close(fig)

# 3. コメントレベル別箱ひげ図
fig = plt.figure(figsize=(12, 6))
ax = sns.boxplot(data=all_melt, x='metric', y='score', hue='level')
ax.set_title('コメントレベル別の指標の箱ひげ図', fontsize=14)
ax.set_ylim(0, 1)
fig.savefig(os.path.join(plots_dir, 'boxplot_metrics_by_level.png'), dpi=300, bbox_inches='tight')
plt.close(fig)

# 4. 平均スコア推移折れ線
agg = all_df.groupby('level')[metrics].mean().reset_index()
fig = plt.figure(figsize=(10, 6))
for m in metrics:
    plt.plot(agg['level'], agg[m], marker='o', label=m)
plt.title('コメントレベル別の平均スコア推移', fontsize=14)
plt.xlabel('コメントレベル')
plt.ylabel('スコア')
plt.ylim(0, 1)
plt.legend(bbox_to_anchor=(1.05, 1))
plt.tight_layout()
fig.savefig(os.path.join(plots_dir, 'mean_score_trend.png'), dpi=300, bbox_inches='tight')
plt.close(fig)

# 5. 関数ごとの SBC 推移 FacetGrid
sns.set_context('notebook', font_scale=0.8)
g = sns.FacetGrid(all_df, col='function', col_wrap=4, height=3, sharey=True)
g.map_dataframe(sns.lineplot, x='level', y='sbc', marker='o')
g.set(ylim=(0, 1), xlabel='コメントレベル', ylabel='SBC')
plt.tight_layout()
g.savefig(os.path.join(plots_dir, 'facet_sbc_trend.png'), dpi=300, bbox_inches='tight')
plt.close()

# 6. レーダーチャート一括出力

radar_dir = os.path.join(base_dir, 'radar_charts')
os.makedirs(radar_dir, exist_ok=True)
fields = ['trigger', 'action', 'effect', 'constraint']
for func in all_df['function'].unique():
    for level in all_df['level'].unique():
        sub = all_df[(all_df['function'] == func) & (all_df['level'] == level)]
        radar_data = sub[sub['field'].isin(fields)].set_index('field').loc[fields]
        if radar_data.empty:
            continue
        scores = radar_data['sbc'].values
        angles = np.linspace(0, 2 * np.pi, len(fields), endpoint=False).tolist()
        scores_loop = np.concatenate((scores, [scores[0]]))
        angles_loop = angles + [angles[0]]
        fig = plt.figure(figsize=(4, 4))
        ax = fig.add_subplot(111, polar=True)
        ax.plot(angles_loop, scores_loop, marker='o')
        ax.fill(angles_loop, scores_loop, alpha=0.25)
        ax.set_xticks(angles)
        ax.set_xticklabels(fields)
        ax.set_ylim(0, 1)
        ax.set_title(f'{func} {level} SBC per Field', fontsize=10)
        out_png = os.path.join(radar_dir, f'{func}_{level}_radar.png')
        fig.savefig(out_png, dpi=300, bbox_inches='tight')
        plt.close(fig)

print('[✓] 全てのプロットをPNG形式で保存しました')
