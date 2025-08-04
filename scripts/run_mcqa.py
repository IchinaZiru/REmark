import os
import glob
import json
import pandas as pd
import pathlib
import argparse
import time
import re
import requests
import datetime
from openai import OpenAI
import anthropic
from anthropic._exceptions import OverloadedError
from dotenv import load_dotenv

# ------------------------------
# 環境変数ロード
# ------------------------------
load_dotenv()

# ------------------------------
# CLI 引数
# ------------------------------
DEFAULT_OLLAMA_HOST = os.getenv("OLLAMA_HOST", "http://localhost:11434")

parser = argparse.ArgumentParser(description="Run MCQA evaluation for a specific comment level")
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
parser.add_argument("--model", type=str, required=True,
                    help="使用モデル名 (例: gpt-4o / openwebui-llama3:latest / claude-3-sonnet / ollama-llama3:latest)")
parser.add_argument("--ollama_host", type=str, default=DEFAULT_OLLAMA_HOST,
                    help=f"Ollama APIのホストURL (デフォルト: 環境変数 OLLAMA_HOST または {DEFAULT_OLLAMA_HOST})")
args = parser.parse_args()

LEVEL = args.level
assert 0 <= LEVEL <= 7, "--level は 0〜7 を指定してください"

MODEL = args.model

# ------------------------------
# クライアントクラス
# ------------------------------
class OllamaClient:
    def __init__(self, host):
        self.host = host

    def ask(self, model, prompt):
        response = requests.post(f"{self.host}/api/generate", json={
            "model": model,
            "prompt": prompt,
            "stream": False
        })
        return response.json().get("response", "").strip()

# ------------------------------
# モデル別クライアント設定
# ------------------------------
if MODEL.startswith("openwebui-"):
    _, model_name = MODEL.split("-", 1)
    client = OpenAI(
        base_url=os.getenv("OPENWEBUI_API_BASE", "http://localhost:3000/v1"),
        api_key=os.getenv("OPENWEBUI_API_KEY", "dummy")
    )
    target_model = model_name

elif MODEL.startswith("claude"):
    claude_client = anthropic.Anthropic(api_key=os.getenv("ANTHROPIC_API_KEY"))
    target_model = MODEL

elif MODEL.startswith("ollama-"):
    model_name = MODEL.split("-", 1)[1]
    ollama_client = OllamaClient(host=args.ollama_host)
    target_model = model_name

else:
    client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
    target_model = MODEL

# ------------------------------
# コードファイル読み込み
# ------------------------------
CODE_FILE = f"data/levels/L{LEVEL}/Meeting_Room_Reservation_System_L{LEVEL}.c"
if not os.path.exists(CODE_FILE):
    raise FileNotFoundError(f"コードが見つかりません: {CODE_FILE}. 先に insert_comments.py で生成してください。")

with open(CODE_FILE, encoding="utf-8") as f:
    code_block = f.read()

results = []
quiz_files = sorted(glob.glob("data/mcqa_pools/*.json"))
total = len(quiz_files)
print(f"L{LEVEL} 用に {total} 問のMCQAを {MODEL} で評価中...\n")
start_time_all = time.time()

MAX_RETRIES = 5

# ------------------------------
# MCQA 実行ループ
# ------------------------------
for i, quiz_path in enumerate(quiz_files, 1):
    quiz = json.load(open(quiz_path, encoding="utf-8"))
    choice_lines = "\n".join(f"{chr(65+i)}. {c}" for i, c in enumerate(quiz["choices"]))

    if MODEL.startswith("ollama-deepseek"):
        prompt = (
            "次のCコードと問題を読んで、"
            "必ず A / B / C / D の1文字だけを1行で出力してください。\n"
            "説明や理由は禁止です。\n"
            "<think>タグも禁止です。\n"
            "出力例: B\n\n"
            f"```c\n{code_block}\n```\n\n"
            f"問題: {quiz['question']}\n{choice_lines}"
        )
    else:
        prompt = (
            "以下のCコードを読んで四択問題に答えてください。\n"
            "【重要】出力は必ず以下の中から1文字だけ選んでください：\n"
            "A / B / C / D\n\n"
            "絶対に文章や説明は書かないでください。\n"
            "もしわからない場合でも必ず A〜D の中から最も適切なものを選んでください。\n\n"
            f"```c\n{code_block}\n```\n\n"
            f"問題: {quiz['question']}\n{choice_lines}"
        )
    
    start_time = time.time()
    answer_raw = ""

    if MODEL.startswith("claude"):
        for attempt in range(MAX_RETRIES):
            try:
                response = claude_client.messages.create(
                    model=target_model,
                    max_tokens=10,
                    messages=[{"role": "user", "content": prompt}],
                )
                answer_raw = response.content[0].text.strip()
                break
            except OverloadedError:
                wait_time = 2 ** attempt
                print(f"Claude API overloaded. Retrying in {wait_time}s...")
                time.sleep(wait_time)
        else:
            answer_raw = "?"

    elif MODEL.startswith("ollama-"):
        answer_raw = ollama_client.ask(target_model, prompt)

    else:
        res = client.chat.completions.create(
            model=target_model,
            messages=[{"role": "user", "content": prompt}],
            temperature=0,
        )
        answer_raw = res.choices[0].message.content.strip()
        
    elapsed_time = round(time.time() - start_time, 2)

    if "deepseek" in MODEL.lower():
        match = re.search(r"(?:解答|答えは|Answer)[:：]?\s*([ABCD])", answer_raw, re.IGNORECASE)
        if match:
            answer = match.group(1).upper()
        else:
            matches = re.findall(r"\b([ABCD])\b", answer_raw, re.IGNORECASE)
            answer = matches[-1].upper() if matches else "?"
    else:
        # 通常の抽出処理
        match = re.search(r"\b([ABCD])\b", answer_raw, re.IGNORECASE)
        if not match:
            match = re.search(r"(?:Option|Answer|選択肢)?\s*([ABCD])", answer_raw, re.IGNORECASE)
        if not match:
            num_match = re.search(r"\b([1-4])\b", answer_raw)
            if num_match:
                num_to_char = {"1": "A", "2": "B", "3": "C", "4": "D"}
                answer = num_to_char.get(num_match.group(1), "?")
            else:
                answer = "?"
        else:
            answer = match.group(1).upper()

    fname = pathlib.Path(quiz_path).stem
    results.append({"function": fname, "answer": answer, "elapsed_time_sec": elapsed_time})

    print(f"[{i}/{total}] {fname}: 回答 = {answer} | 時間: {elapsed_time}s | raw: {answer_raw}".encode("cp932", errors="ignore").decode("cp932"))

    time.sleep(1.0 if MODEL.startswith(("gpt-", "claude")) else 0.2)

total_elapsed_time = round(time.time() - start_time_all, 2)
print(f"\nモデル {MODEL} (L{LEVEL}) の総処理時間: {total_elapsed_time}s")

# ------------------------------
# 結果保存
# ------------------------------
safe_model_name = MODEL.replace(":", "-")
out_dir = os.path.join("outputs", "mcqa_results", safe_model_name)
os.makedirs(out_dir, exist_ok=True)
out_csv = os.path.join(out_dir, f"L{LEVEL}.csv")
pd.DataFrame(results).to_csv(out_csv, index=False)

with open(out_csv, "a", encoding="utf-8") as f:
    f.write(f"\nTOTAL_ELAPSED_TIME(sec),{total_elapsed_time}\n")

print(f"\n回答収集完了 → {out_csv}")
