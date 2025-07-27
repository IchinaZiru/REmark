import os
import re
import yaml
import argparse
from openai import OpenAI
from dotenv import load_dotenv

# -------------------------------
# 引数と初期化
# -------------------------------
parser = argparse.ArgumentParser()
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
args = parser.parse_args()
LEVEL = args.level
assert 0 <= LEVEL <= 7, "--level は 0〜7 の整数で指定してください"

load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

# -------------------------------
# ソース読み込み
# -------------------------------
CODE_PATH = f"data/levels/L{LEVEL}/Meeting_Room_Reservation_System_L{LEVEL}.c"
OUT_DIR = f"data/yaml/requirements/generated/L{LEVEL}/"
DEBUG_DIR = f"outputs/llm_inputs/L{LEVEL}"
os.makedirs(OUT_DIR, exist_ok=True)
os.makedirs(DEBUG_DIR, exist_ok=True)

with open(CODE_PATH, encoding="utf-8") as f:
    code_lines = f.readlines()

# コメントアウトパターン
comment_pattern = re.compile(r'^\s*//\s*\[[A-Za-z]+\]')
# 関数定義パターン（{ が別行でも検出可能にする）
func_head_pattern = re.compile(r'^\s*[\w\*\s]+\s+([A-Za-z_]\w*)\s*\([^)]*\)\s*(;|$)')

# -------------------------------
# 関数検出と description 生成
# -------------------------------
functions = []  # (func_id, func_name, start_idx, end_idx)
func_counter = 1
idx = 0

while idx < len(code_lines):
    line = code_lines[idx]
    match = func_head_pattern.match(line)
    if match:
        func_name = match.group(1)
        start_idx = idx

        # 関数本体の開始位置（{ の行）を探す
        while idx < len(code_lines) and '{' not in code_lines[idx]:
            idx += 1
        if idx >= len(code_lines):
            break

        brace_count = 0
        body_start_idx = idx
        while idx < len(code_lines):
            brace_count += code_lines[idx].count('{')
            brace_count -= code_lines[idx].count('}')
            if brace_count == 0:
                end_idx = idx + 1
                break
            idx += 1

        # コメントを含める範囲を拡張
        context_start = start_idx
        while context_start > 0 and comment_pattern.match(code_lines[context_start - 1]):
            context_start -= 1

        code_block = "".join(code_lines[context_start:end_idx])

        # デバッグ用に LLM に渡したコードを保存
        func_id = f"F{func_counter:03d}"
        debug_path = os.path.join(DEBUG_DIR, f"{func_id}_{func_name}.c")
        with open(debug_path, "w", encoding="utf-8") as f_debug:
            f_debug.write(code_block)

        # プロンプト生成
        prompt = f"""
                以下のC関数 `{func_name}` の上部には説明的なコメントがあります。
                それらの情報を活用し、この関数の目的を1文で要約してください。

                ```c
                {code_block}
                ```

                # 出力形式
                日本語で1文のみ。
                """

        try:
            res = client.chat.completions.create(
                model="gpt-4o",
                messages=[{"role": "user", "content": prompt}],
                temperature=0,
            )
            output = res.choices[0].message.content.strip()
            description = re.search(r"^(.+)$", output, re.M).group(1)

            out_data = {
                "id": func_id,
                "function": func_name,
                "description": description,
            }

            out_path = os.path.join(OUT_DIR, f"{func_id}_{func_name}.yaml")
            with open(out_path, "w", encoding="utf-8") as f_out:
                yaml.dump(out_data, f_out, allow_unicode=True)

            print(f"[✓] {func_name} → {out_path}")
        except Exception as e:
            print(f"[×] {func_name}: エラー → {e}")

        func_counter += 1
    else:
        idx += 1