import os
import glob
import yaml
import argparse
import pandas as pd
from bleurt import score

# -------------------------------
# 引数とパスの設定
# -------------------------------
parser = argparse.ArgumentParser()
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
args = parser.parse_args()
LEVEL = args.level

GEN_DIR = f"data/yaml/requirements/generated/L{LEVEL}/"
GOLD_DIR = "data/yaml/requirements/0-shot/"
BLEURT_DIR = "models/bleurt"
OUT_DIR = "outputs/bleurt_scores"
os.makedirs(OUT_DIR, exist_ok=True)

# -------------------------------
# BLEURTスコアラーの初期化
# -------------------------------
scorer = score.BleurtScorer(BLEURT_DIR)

# -------------------------------
# ペアごとにスコアを計算
# -------------------------------
results = []
for gen_path in sorted(glob.glob(os.path.join(GEN_DIR, "*.yaml"))):
    filename = os.path.basename(gen_path)
    gold_path = os.path.join(GOLD_DIR, filename)

    if not os.path.exists(gold_path):
        print(f"対応する正解ファイルが見つかりません: {filename}")
        continue

    with open(gen_path, encoding="utf-8") as f:
        gen_yaml = yaml.safe_load(f)
    with open(gold_path, encoding="utf-8") as f:
        gold_yaml = yaml.safe_load(f)

    ref = gold_yaml.get("description", "").strip()
    hyp = gen_yaml.get("description", "").strip()

    if not ref or not hyp:
        print(f"スキップ: {filename}（空のdescription）")
        continue

    score_val = scorer.score(references=[ref], candidates=[hyp])[0]

    results.append({
        "id": gen_yaml.get("id", ""),
        "function": gen_yaml.get("function", ""),
        "bleurt": score_val,
        "ref": ref,
        "hyp": hyp,
    })
    print(f"[✓] {filename} → BLEURT: {score_val:.4f}")

# -------------------------------
# 結果出力
# -------------------------------
out_csv = os.path.join(OUT_DIR, f"L{LEVEL}.csv")
pd.DataFrame(results).to_csv(out_csv, index=False)
print(f"\nBLEURTスコア出力完了 → {out_csv}")
