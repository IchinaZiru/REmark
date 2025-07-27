import os, glob, json, pandas as pd, pathlib, argparse
from openai import OpenAI
from dotenv import load_dotenv
import time

# ------------------------------
# CLI 引数
# ------------------------------
parser = argparse.ArgumentParser(description="Run MCQA evaluation for a specific comment level")
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
args = parser.parse_args()
LEVEL = args.level
assert 0 <= LEVEL <= 7, "--level は 0〜7 を指定してください"

# ------------------------------
load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

CODE_FILE = f"data/levels/L{LEVEL}/Meeting_Room_Reservation_System_L{LEVEL}.c"
if not os.path.exists(CODE_FILE):
    raise FileNotFoundError(f"コードが見つかりません: {CODE_FILE}. 先に insert_comments.py で生成してください。")

with open(CODE_FILE, encoding="utf-8") as f:
    code_block = f.read()

results = []
quiz_files = sorted(glob.glob("data/mcqa_pools/*.json"))
total = len(quiz_files)
print(f"L{LEVEL} 用に {total} 問のMCQAを評価中...\n")

for i, quiz_path in enumerate(quiz_files, 1):
    quiz = json.load(open(quiz_path, encoding="utf-8"))
    choice_lines = "\n".join(f"{chr(65+i)}. {c}" for i, c in enumerate(quiz["choices"]))

    prompt = (
        "以下の C コードを読んで四択問題に A/B/C/D で答えてください。**回答は A〜D の1文字だけ。余計な説明は不要です。**\n\n"
        f"```c\n{code_block}\n```\n\n"
        f"問題: {quiz['question']}\n{choice_lines}"
    )

    res = client.chat.completions.create(
        model="gpt-4o",
        messages=[{"role": "user", "content": prompt}],
        temperature=0,
    )

    answer = res.choices[0].message.content.strip()[0].upper()
    fname = pathlib.Path(quiz_path).stem
    results.append({"function": fname, "answer": answer})

    print(f"[{i}/{total}] {fname}: 回答 = {answer}")
    time.sleep(1.2)  # 過剰リクエスト防止

out_dir = "outputs/mcqa_results"
os.makedirs(out_dir, exist_ok=True)
out_csv = f"{out_dir}/L{LEVEL}.csv"
pd.DataFrame(results).to_csv(out_csv, index=False)
print(f"\n回答収集完了 → {out_csv}")