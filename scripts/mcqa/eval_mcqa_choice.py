import os
import json
import pandas as pd
from bleurt import score
from sentence_transformers import SentenceTransformer, util
from tqdm import tqdm

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
MCQA_DIR = os.path.join(BASE_DIR, "data", "mcqa_pools_refined")
OUTPUT_DIR = os.path.join(BASE_DIR, "outputs/mcqa_qualities_refined")
os.makedirs(OUTPUT_DIR, exist_ok=True)

MODEL_PATH = os.path.join(BASE_DIR, "models", "../models/Bleurt")
bleurt_scorer = score.BleurtScorer(MODEL_PATH)

sbert_model = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')

summary_results = []
debug_results = []

files = [f for f in os.listdir(MCQA_DIR) if f.endswith('.json')]
total_files = len(files)
print(f"対象ファイル数: {total_files}")

for idx, file in enumerate(tqdm(files, desc="MCQA 品質チェック進行中", unit="file")):
    path = os.path.join(MCQA_DIR, file)
    try:
        with open(path, 'r', encoding='utf-8') as f:
            data = json.load(f)
    except Exception as e:
        print(f"{file} 読み込みエラー: {e}")
        continue

    question = data['question']
    choices = data['choices']
    ans_idx = data['answer_index']
    correct = choices[ans_idx]

    embeddings = sbert_model.encode([correct] + choices, convert_to_tensor=True)
    similarities = util.cos_sim(embeddings[0], embeddings[1:]).cpu().numpy()[0]
    bleurt_scores = bleurt_scorer.score(references=[correct] * len(choices),
                                        candidates=choices)

    summary_results.append({
        "file": file,
        "avg_sim": float(similarities.mean()),
        "max_sim": float(similarities.max()),
        "near_duplicates": int((similarities > 0.9).sum()),
        "avg_bleurt": sum(bleurt_scores) / len(bleurt_scores),
        "bleurt_max": max(bleurt_scores),
        "answer": correct
    })

    for i, choice in enumerate(choices):
        sim_score = float(util.cos_sim(embeddings[0], embeddings[i+1]).item())
        debug_results.append({
            "file": file,
            "question": question,
            "correct_answer": correct,
            "choice_index": i,
            "choice_text": choice,
            "similarity_to_correct": sim_score,
            "bleurt_score": bleurt_scores[i],
            "is_correct": (i == ans_idx)
        })

# 結果を一括出力
summary_df = pd.DataFrame(summary_results)
summary_path = os.path.join(OUTPUT_DIR, "mcqa_quality_report.csv")
summary_df.to_csv(summary_path, index=False, encoding='utf-8-sig')

debug_df = pd.DataFrame(debug_results)
debug_path = os.path.join(OUTPUT_DIR, "mcqa_quality_debug.csv")
debug_df.to_csv(debug_path, index=False, encoding='utf-8-sig')

print(f"\n✔ 品質チェック完了")
print(f"  - サマリー: {summary_path}")
print(f"  - デバッグ: {debug_path}")
