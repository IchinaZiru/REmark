import os
import yaml
from glob import glob
from datetime import datetime
from pathlib import Path
from openai import OpenAI
from dotenv import load_dotenv

load_dotenv()

# OpenAIクライアント初期化
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

MODEL_NAME = "gpt-4o"
PROMPT_TEMPLATE = """以下のC関数を読んで、その機能を日本語で簡潔に1〜2文で要約してください。

- 対象はC言語の関数です。
- 関数の目的・動作・出力の特徴がわかるように、開発者向けの設計書に記載する説明文をイメージしてください。
- 処理の流れや主な条件分岐、外部関数の呼び出し結果なども、意味が分かる範囲で含めてください。
- 冗長な逐次説明ではなく、「何をする関数か」がすぐに分かるような目的ベースの自然文にしてください。
- 出力は日本語で。コードの繰り返しや細部の列挙ではなく、抽象化した説明にしてください。

出力形式：日本語1〜2文の自然文のみ（記号や番号などは不要）

以下がその関数です：
{code}
"""

yaml_dir = Path("C:/Users/yuta/Documents/GitHub/REmark/data/yaml/requirements/0-shot")
yaml_files = sorted(yaml_dir.glob("F0*_*.yaml"))
log_path = yaml_dir / "description_generation_log.txt"
os.makedirs(yaml_dir, exist_ok=True)

def extract_code_from_yaml(filepath):
    try:
        with open(filepath, "r", encoding="utf-8") as f:
            content = yaml.safe_load(f)
        return content.get("code", ""), content
    except Exception as e:
        raise RuntimeError(f"YAML読み込みエラー: {e}")

def generate_description(code):
    prompt = PROMPT_TEMPLATE.format(code=code)
    response = client.chat.completions.create(
        model=MODEL_NAME,
        messages=[{"role": "user", "content": prompt}],
        temperature=0.2,
    )
    return response.choices[0].message.content.strip()

with open(log_path, "a", encoding="utf-8") as log:
    log.write(f"\n--- {datetime.now().isoformat()} description generation start ---\n")

for file_path in yaml_files:
    filename = file_path.name

    try:
        code, data = extract_code_from_yaml(file_path)
        if not code.strip():
            print(f"⚠️ {filename}：codeフィールドが空です")
            continue

        if "description" in data:
            print(f"⏩ {filename}：既にdescriptionあり（スキップ）")
            continue

        description = generate_description(code)
        data["description"] = description

        # バックアップ（.bak）作成
        file_path.rename(file_path.with_suffix(".yaml.bak"))

        # YAMLファイル上書き
        with open(file_path, "w", encoding="utf-8") as f:
            yaml.safe_dump(data, f, allow_unicode=True)

        print(f"✅ {filename}：完了")
        with open(log_path, "a", encoding="utf-8") as log:
            log.write(f"[OK] {filename}：{description}\n")

    except Exception as e:
        print(f"❌ {filename}：失敗 - {str(e)}")
        with open(log_path, "a", encoding="utf-8") as log:
            log.write(f"[ERROR] {filename}：{str(e)}\n")
