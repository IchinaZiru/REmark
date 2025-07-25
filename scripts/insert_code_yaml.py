from pathlib import Path
import pandas as pd
import yaml

def find_yaml_by_func_name(yaml_dir, func_id, func_name):
    candidates = list(yaml_dir.glob(f"{func_id}_*.yaml"))
    for file in candidates:
        if func_name in file.stem:
            return file
    return None

# ファイルパス
csv_path = Path("data/csv/functions.csv")
c_path = Path("data/original/Meeting_Room_Reservation_System.c")
yaml_dir = Path("data/yaml/requirements/0-shot")

# 読み込み
df = pd.read_csv(csv_path)
with open(c_path, "r", encoding="utf-8") as f:
    c_lines = f.readlines()

for idx, row in df.iterrows():
    func_num = f"F{idx+1:03d}"
    func_name = row["関数名"]
    start = int(row["開始行"]) - 1
    end = int(row["終了行"])

    code = "".join(c_lines[start:end])
    yaml_path = find_yaml_by_func_name(yaml_dir, func_num, func_name)

    if yaml_path is None:
        print(f"❌ YAMLファイルが見つかりません: {func_num}_{func_name}.yaml")
        continue

    try:
        with open(yaml_path, "r", encoding="utf-8") as f:
            data = yaml.safe_load(f)

        data["code"] = code

        with open(yaml_path, "w", encoding="utf-8") as f:
            yaml.safe_dump(data, f, allow_unicode=True)

        print(f"✅ {yaml_path.name} に code を追加しました")
    except Exception as e:
        print(f"❌ {yaml_path.name} の処理中にエラー: {str(e)}")
