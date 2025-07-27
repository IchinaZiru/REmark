import os
import re
import json
import random
import pandas as pd
from openai import OpenAI
from dotenv import load_dotenv

# ------------------------------
# 初期化
# ------------------------------
load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

# ------------------------------
# 入力ファイル
# ------------------------------
FUNC_CSV = "data/csv/functions.csv"
SRC_FILE = "data/original/Meeting_Room_Reservation_System.c"
POOL_DIR = "data/mcqa_pools"

os.makedirs(POOL_DIR, exist_ok=True)

func_df = pd.read_csv(FUNC_CSV, encoding="utf-8-sig")
with open(SRC_FILE, encoding="utf-8") as f:
    src_lines = f.readlines()

# ------------------------------
# GPT 出力から JSON を安全に抽出
# ------------------------------
def extract_json(text: str) -> dict | None:
    """GPT から返ってきた文字列から JSON オブジェクトを抜き dict にして返す"""

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

# ------------------------------
# メインループ
# ------------------------------
for _, row in func_df.iterrows():
    name = row["関数名"]
    start = int(row["開始行"]) - 1
    end = int(row["終了行"])
    code = "".join(src_lines[start:end])

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

    # GPT 呼び出し
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
        print(f"{name} で JSON 抽出失敗\n-----\n{res.choices[0].message.content}\n-----")
        continue
    
    index = json_dict["answer_index"]
    if isinstance(index, list):
        index = index[0]

    # シャッフル処理を追加
    correct_answer = json_dict["choices"][index]
    random.shuffle(json_dict["choices"])
    json_dict["answer_index"] = json_dict["choices"].index(correct_answer)

    out_path = os.path.join(POOL_DIR, f"{name}.json")
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(json_dict, f, ensure_ascii=False, indent=2)

    print(f"pool saved → {out_path}")