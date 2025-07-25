import os
import pandas as pd

# === コメントテンプレートの読み込み ===
def load_templates(path="data/csv/comment_templates.csv"):
    df = pd.read_csv(path, encoding="utf-8-sig")
    return df.set_index("function_name").to_dict(orient="index")

# === コメント生成（L1〜L7） ===
def generate_comment_block(template: dict, level: int) -> str:
    keys = [
        "Logical", "Precise", "Unambiguous",
        "Exhaustive", "Troubleshooting",
        "Contextualizing", "Condensing"
    ]
    lines = []
    for i in range(level):
        key = keys[i]
        if key in template and isinstance(template[key], str):
            lines.append(f"// [{key}] {template[key].strip()}")
    return "\n".join(lines) + "\n"

# === 関数へのコメント挿入 ===
def insert_comments_to_code(source_path, output_path, templates_df, level):
    with open(source_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    functions_df = pd.read_csv("data/csv/functions.csv", encoding="utf-8-sig")

    for _, row in functions_df.iterrows():
        name = row["関数名"]
        start = int(row["開始行"]) - 1

        if name not in templates_df:
            print(f"⚠️ テンプレートが見つかりません: {name}")
            continue

        comment = generate_comment_block(templates_df[name], level)
        lines[start] = comment + lines[start]
        print(f"コメント挿入完了: {name}")

    with open(output_path, "w", encoding="utf-8") as f:
        f.writelines(lines)

    print(f"\n出力完了: {output_path}")

# === CLI引数対応 ===
if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--level", type=int, required=True, help="コメントレベル (1〜7)")
    args = parser.parse_args()

    level = args.level
    assert 1 <= level <= 7, "レベルは1〜7で指定してください"

    templates = load_templates()

    src = "data/original/Meeting_Room_Reservation_System.c"
    dst = f"data/levels/L{level}/Meeting_Room_Reservation_System_L{level}.c"
    os.makedirs(os.path.dirname(dst), exist_ok=True)

    insert_comments_to_code(
        source_path=src,
        output_path=dst,
        templates_df=templates,
        level=level
    )
