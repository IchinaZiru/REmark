import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

plt.rcParams['font.family'] = 'IPAexGothic'
plt.rcParams['axes.unicode_minus'] = False
sns.set(style='whitegrid')

BASE = 'outputs'
LVLS = [f'L{i}' for i in range(8)]
results = []

def ensure_cols(df, rename_map):
    for old, new in rename_map.items():
        if old in df.columns:
            df = df.rename(columns={old: new})
    if 'id' not in df.columns and 'func_id' in df.columns:
        df['id'] = df['func_id']
    if 'function' not in df.columns and 'function_name' in df.columns:
        df['function'] = df['function_name']
    if 'id' not in df.columns:
        df['id'] = pd.NA
    if 'function' not in df.columns:
        raise KeyError("function列が見つかりません")
    return df

for lvl in LVLS:
    try:
        # MCQA
        p_m = os.path.join(BASE, 'mcqa_results', f'{lvl}_scored.csv')
        df_m = pd.read_csv(p_m)
        df_m = ensure_cols(df_m, {})
        if 'is_correct' in df_m.columns:
            df_m = df_m.rename(columns={'is_correct': 'mcqa'})
        elif 'mcqa' not in df_m.columns:
            raise KeyError('mcqa列が見つかりません')
        df_m = df_m[['function', 'mcqa']]

        # BLEURT
        p_b = os.path.join(BASE, 'bleurt_scores', f'{lvl}.csv')
        df_b = pd.read_csv(p_b)
        df_b = ensure_cols(df_b, {})
        if 'bleurt' not in df_b.columns:
            sc_col = [c for c in df_b.columns if c.lower().startswith('bleurt') or c == 'score']
            df_b = df_b.rename(columns={sc_col[0]: 'bleurt'})
        df_b = df_b[['id', 'function', 'bleurt']]

        # SBC
        p_s = os.path.join(BASE, 'sbc_results', f'{lvl}.csv')
        df_s = pd.read_csv(p_s)
        df_s = ensure_cols(df_s, {})
        df_s = df_s[df_s['field'].isin(['trigger', 'action', 'effect', 'constraint'])]
        df_s = df_s.groupby(['id', 'function'])['sbc'].mean().reset_index()

        # マージ
        tmp = df_m.merge(df_b, on='function', how='left')
        merged = tmp.merge(df_s, on=['id', 'function'], how='left')
        merged['level'] = lvl
        merged = merged[['id', 'function', 'mcqa', 'bleurt', 'sbc', 'level']]
        results.append(merged)

    except Exception as e:
        print(f'[!] {lvl} スキップ: {e}')

if not results:
    raise ValueError('どのレベルも結合できませんでした。列名を再確認してください。')

all_df = pd.concat(results, ignore_index=True)
all_df['id'] = all_df['id'].fillna('')
all_df = all_df.sort_values(by=['level', 'id'])

# CSV形式で保存
out_path = os.path.join(BASE, 'merged_results', 'merged_scores.csv')
os.makedirs(os.path.dirname(out_path), exist_ok=True)
all_df.to_csv(out_path, index=False, encoding='utf-8-sig')
print('[✓] 統合スコアCSV出力完了 ➜', out_path)
