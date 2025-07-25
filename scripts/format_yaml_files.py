import os
from pathlib import Path
from ruamel.yaml import YAML
from ruamel.yaml.comments import CommentedMap

yaml = YAML()
yaml.indent(mapping=2, sequence=4, offset=2)
yaml.preserve_quotes = True

TARGET_DIR = Path("C:/Users/yuta/Documents/GitHub/REmark/data/yaml/requirements/0-shot")
FIELD_ORDER = [
    "id",
    "function",
    "range",
    "args",
    "return",
    "requirement",
    "code",
    "description",
    "description_old"
]
REQUIREMENT_SUBFIELDS = ["trigger", "action", "effect", "constraint"]

def reorder_yaml_fields(data):
    ordered = CommentedMap()

    for key in FIELD_ORDER:
        if key == "requirement":
            req_data = data.get("requirement", {})
            req_ordered = CommentedMap()
            for subkey in REQUIREMENT_SUBFIELDS:
                req_ordered[subkey] = req_data.get(subkey, "")
            ordered["requirement"] = req_ordered
        elif key in data:
            ordered[key] = data[key]

    # その他のキーを末尾に追加
    for k in data:
        if k not in ordered:
            ordered[k] = data[k]

    return ordered

def format_yaml_file(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        data = yaml.load(f)

    if data is None:
        print(f"⚠️ スキップ: {filepath.name} - YAMLが空または無効")
        return

    ordered_data = reorder_yaml_fields(data)

    # バックアップ
    backup_path = filepath.with_suffix(".yaml.bak")
    os.replace(filepath, backup_path)

    # 書き込み
    with open(filepath, "w", encoding="utf-8") as f:
        yaml.dump(ordered_data, f)

    print(f"✅ 整形完了: {filepath.name}")

# 実行
yaml_files = sorted(TARGET_DIR.glob("F0*_*.yaml"))
print(f"📁 対象ファイル数: {len(yaml_files)}")

for file_path in yaml_files:
    try:
        format_yaml_file(file_path)
    except Exception as e:
        print(f"❌ 失敗: {file_path.name} - {e}")
