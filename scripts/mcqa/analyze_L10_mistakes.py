import os, json
import pandas as pd
from pathlib import Path

MODEL = "gpt-4o"
RESULT_CSV = f"outputs/mcqa_results/{MODEL}/L10.csv"
POOL_DIR = "data/mcqa_pools_L10"
OUT_DIR = f"outputs/mcqa_scored/{MODEL}"
Path(OUT_DIR).mkdir(parents=True, exist_ok=True)

def letter(i): return "ABCD"[i]

# 読み込み（サマリ行を除去）
df = pd.read_csv(RESULT_CSV)
df = df[df["function"].notna() & (df["function"] != "TOTAL_ELAPSED_TIME(sec)")]
df["answer"] = df["answer"].astype(str).str.strip().str.upper()
df.loc[~df["answer"].isin(list("ABCD")), "answer"] = "?"

rows = []
for _, r in df.iterrows():
    func = r["function"]
    pred = r["answer"]
    pool_json = os.path.join(POOL_DIR, f"{func}.json")
    if not os.path.exists(pool_json):
        rows.append({"function": func, "note": f"POOL NOT FOUND: {pool_json}"})
        continue
    pool = json.load(open(pool_json, encoding="utf-8"))
    choices = pool["choices"]
    correct_letter = letter(int(pool["answer_index"]))
    pred_text = choices["ABCD".index(pred)] if pred in "ABCD" else None

    rows.append({
        "function": func,
        "question": pool.get("question", ""),
        "predicted_letter": pred,
        "predicted_text": pred_text,
        "correct_letter": correct_letter,
        "correct_text": choices[int(pool["answer_index"])],
    })

out_path = os.path.join(OUT_DIR, "L10_errors_detailed.csv")
pd.DataFrame(rows).to_csv(out_path, index=False, encoding="utf-8-sig")
print("Wrote:", out_path)

# 誤答だけスライスも保存
df_all = pd.DataFrame(rows)
df_err = df_all[(df_all["predicted_letter"].notna())
                & (df_all["correct_letter"].notna())
                & (df_all["predicted_letter"] != df_all["correct_letter"])]
err_path = os.path.join(OUT_DIR, "L10_errors_only.csv")
df_err.to_csv(err_path, index=False, encoding="utf-8-sig")
print("Wrote:", err_path)