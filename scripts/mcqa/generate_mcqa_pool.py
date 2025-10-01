
import os
import re
import json
import random
import pandas as pd
from openai import OpenAI
from dotenv import load_dotenv
from sentence_transformers import SentenceTransformer, util

# ------------------------------
# 初期化
# ------------------------------
load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
model_sbert = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')

FUNC_CSV = "data/csv/functions.csv"
SRC_FILE = "data/original/Meeting_Room_Reservation_System.c"
POOL_DIR = "data/mcqa_pools_refined_mmr"
os.makedirs(POOL_DIR, exist_ok=True)

func_df = pd.read_csv(FUNC_CSV, encoding="utf-8-sig")
with open(SRC_FILE, encoding="utf-8") as f:
    src_lines = f.readlines()

def extract_json(text: str) -> dict | None:
    m = re.search(r"```json[\s\S]*?(\{.*?\})[\s\S]*?```", text, re.S)
    if m:
        candidate = m.group(1)
    else:
        m2 = re.search(r"\{[\s\S]*\}", text)
        candidate = m2.group(0) if m2 else ""

    candidate = candidate.strip().replace("```", "")
    if not candidate:
        return None

    try:
        return json.loads(candidate)
    except json.JSONDecodeError:
        return None

def generate_distractors(correct_text, code, n=10):
    prompt = f"""
以下のC関数に対して、正解の説明文があります：

正解: "{correct_text}"

この正解と意味的に異なり、間違っている説明文を {n} 個作成してください。
- 1文で短く簡潔に
- 正解の意味と重複しない
- 相互に意味が被らないように（多様性を持たせる）
- 正解と明確に区別できる内容を含む
- C言語コードの動作とずれていても良い

```c
{code}
```

出力形式：
{{
  "distractors": ["...", "...", "..."]
}}
"""
    try:
        res = client.chat.completions.create(
            model="gpt-4o",
            messages=[{"role": "user", "content": prompt}],
            temperature=0.7,
        )
    except Exception as e:
        print(f"API error (distractor generation): {e}")
        return []

    json_dict = extract_json(res.choices[0].message.content)
    if json_dict and "distractors" in json_dict:
        return json_dict["distractors"]
    return []

def select_mmr(candidates, correct_embedding, k=3, lambda_=0.6):
    selected = []
    while len(selected) < k:
        scores = []
        for i, cand in enumerate(candidates):
            if i in selected:
                scores.append(-float('inf'))
                continue
            sim_to_correct = util.cos_sim(cand, correct_embedding).item()
            redundancy = max([util.cos_sim(cand, candidates[j]).item() for j in selected], default=0)
            mmr_score = lambda_ * sim_to_correct - (1 - lambda_) * redundancy
            scores.append(mmr_score)
        selected.append(int(scores.index(max(scores))))
    return selected

for _, row in func_df.iterrows():
    name = row["関数名"]
    start = int(row["開始行"]) - 1
    end = int(row["終了行"])
    code = "".join(src_lines[start:end])

    # まず正解＋distractor候補3つを1回出力（正解抽出用）
    prompt = f"""
以下の C 関数 `{name}` を読み、
- 正しい説明 (最も正しい) 1 つ
- 紛らわしいが誤った説明 3 つ
を **JSON だけ** で返してください。

```c
{code}
```

```json
{{
  "question": "関数の目的を最も正しく説明している選択肢は？",
  "choices": ["...", "...", "...", "..."],
  "answer_index": [正しい選択肢のインデックス番号（0〜3）]
}}
```
"""
    try:
        res = client.chat.completions.create(
            model="gpt-4o",
            messages=[{"role": "user", "content": prompt}],
            temperature=0.3,
        )
    except Exception as e:
        print(f"API error for {name}: {e}")
        continue

    json_dict = extract_json(res.choices[0].message.content)
    if json_dict is None:
        print(f"{name} で JSON 抽出失敗")
        continue

    index = json_dict["answer_index"]
    if isinstance(index, list):
        index = index[0]
    correct_answer = json_dict["choices"][index]

    # distractor 候補を生成
    distractor_texts = generate_distractors(correct_answer, code, n=10)
    if not distractor_texts or len(distractor_texts) < 3:
        print(f"{name}: distractor が不足")
        continue

    # MMR による選択
    embeddings = model_sbert.encode([correct_answer] + distractor_texts, convert_to_tensor=True)
    correct_emb = embeddings[0]
    candidate_embs = embeddings[1:]

    selected_idx = select_mmr(candidate_embs, correct_emb, k=3)
    selected_distractors = [distractor_texts[i] for i in selected_idx]

    # 選択肢を構成
    final_choices = [correct_answer] + selected_distractors
    random.shuffle(final_choices)
    answer_index = final_choices.index(correct_answer)

    output = {
        "question": "関数の目的を最も正しく説明している選択肢は？",
        "choices": final_choices,
        "answer_index": answer_index
    }

    out_path = os.path.join(POOL_DIR, f"{name}.json")
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(output, f, ensure_ascii=False, indent=2)

    print(f"Refined pool (MMR) saved → {out_path}")
