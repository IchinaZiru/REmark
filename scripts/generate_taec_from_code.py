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
OUT_DIR = f"data/yaml/requirements/generated_taec/L{LEVEL}/"
DEBUG_DIR = f"outputs/llm_inputs/taec/L{LEVEL}"
os.makedirs(OUT_DIR, exist_ok=True)
os.makedirs(DEBUG_DIR, exist_ok=True)

with open(CODE_PATH, encoding="utf-8") as f:
    code_lines = f.readlines()

# -------------------------------
# コメント / 関数検出パターン
# -------------------------------
comment_pattern = re.compile(r'^\s*//\s*\[[A-Za-z]+\]')
func_head_pattern = re.compile(r'^\s*[\w\*\s]+\s+([A-Za-z_]\w*)\s*\([^)]*\)\s*(;|$)')

# -------------------------------
# 関数検出と T/A/E/C 生成
# -------------------------------
func_counter = 1
idx = 0

while idx < len(code_lines):
    line = code_lines[idx]
    match = func_head_pattern.match(line)
    if match:
        func_name = match.group(1)
        start_idx = idx

        # 関数本体の開始（{）までスキップ
        while idx < len(code_lines) and "{" not in code_lines[idx]:
            idx += 1
        if idx >= len(code_lines):
            break

        body_start = idx
        brace_count = 0
        while idx < len(code_lines):
            brace_count += code_lines[idx].count("{")
            brace_count -= code_lines[idx].count("}")
            if brace_count == 0:
                end_idx = idx + 1
                break
            idx += 1

        # コメントブロックを探索
        context_start = start_idx
        while context_start > 0 and comment_pattern.match(code_lines[context_start - 1]):
            context_start -= 1

        code_block = "".join(code_lines[context_start:end_idx])
        func_id = f"F{func_counter:03d}"

        # デバッグ用に LLM に渡したコードを保存
        debug_path = os.path.join(DEBUG_DIR, f"{func_id}_{func_name}.c")
        with open(debug_path, "w", encoding="utf-8") as f_debug:
            f_debug.write(code_block)

        # プロンプト生成
        prompt = f"""
以下のC関数 `{func_name}` のコメントとコードを参考に、
Trigger / Action / Effect / Constraint をそれぞれ1文で日本語で記述してください。

```c
{code_block}

```

# 出力形式（YAML）
trigger: "..."
action: "..."
effect: "..."
constraint: "..."
"""
        try:
            res = client.chat.completions.create(
                model="gpt-4o",
                messages=[{"role": "user", "content": prompt}],
                temperature=0,
            )

            content = res.choices[0].message.content.strip()
            match = re.search(r'trigger:\s*"(.*?)".*?action:\s*"(.*?)".*?effect:\s*"(.*?)".*?constraint:\s*"(.*?)"', content, re.S)

            if not match:
                print(f"[×] {func_name}: T/A/E/Cの抽出に失敗しました\n---\n{content}\n---")
                func_counter += 1
                continue

            taec_data = {
                "id": func_id,
                "function": func_name,
                "taec": {
                    "trigger": match.group(1).strip(),
                    "action": match.group(2).strip(),
                    "effect": match.group(3).strip(),
                    "constraint": match.group(4).strip()
                }
            }

            out_path = os.path.join(OUT_DIR, f"{func_id}_{func_name}.yaml")
            with open(out_path, "w", encoding="utf-8") as f_out:
                yaml.dump(taec_data, f_out, allow_unicode=True)

            print(f"[✓] {func_name} → {out_path}")

        except Exception as e:
            print(f"[×] {func_name}: エラー → {e}")

        func_counter += 1
    else:
        idx += 1
