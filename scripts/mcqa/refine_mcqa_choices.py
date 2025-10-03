import os
import json
import numpy as np
from sentence_transformers import SentenceTransformer, util

INPUT_DIR = "data/mcqa_pools"
OUTPUT_DIR = "outputs/mcqa_refined"
os.makedirs(OUTPUT_DIR, exist_ok=True)

model = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')

def select_mmr(candidates, correct_embedding, k=3, lambda_=0.6):
    selected = []
    while len(selected) < k:
        scores = []
        for i, cand in enumerate(candidates):
            if i in selected:
                scores.append(-np.inf)
                continue
            sim_to_correct = util.cos_sim(cand, correct_embedding).item()
            redundancy = max([util.cos_sim(cand, candidates[j]).item() for j in selected], default=0)
            mmr_score = lambda_ * sim_to_correct - (1 - lambda_) * redundancy
            scores.append(mmr_score)
        selected.append(int(np.argmax(scores)))
    return selected

files = [f for f in os.listdir(INPUT_DIR) if f.endswith('.json')]

for file in files:
    path = os.path.join(INPUT_DIR, file)
    with open(path, "r", encoding="utf-8") as f:
        data = json.load(f)

    choices = data["choices"]
    ans_idx = data["answer_index"]
    correct = choices[ans_idx]

    # Embeddings
    embeddings = model.encode(choices, convert_to_tensor=True)
    correct_embedding = embeddings[ans_idx]
    
    # Candidate distractors (exclude correct)
    distractor_indices = [i for i in range(len(choices)) if i != ans_idx]
    distractor_embeddings = embeddings[distractor_indices]
    
    # Select top 3 distractors with MMR
    selected_idx = select_mmr(distractor_embeddings, correct_embedding, k=3)
    new_distractors = [choices[distractor_indices[i]] for i in selected_idx]
    
    # New choices (correct always first)
    refined_choices = [correct] + new_distractors
    new_data = data.copy()
    new_data["choices"] = refined_choices
    new_data["answer_index"] = 0  # correct is now first
    
    out_path = os.path.join(OUTPUT_DIR, file)
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(new_data, f, ensure_ascii=False, indent=2)

print(f"✔ MMR 再選択完了: {len(files)} files -> {OUTPUT_DIR}")
