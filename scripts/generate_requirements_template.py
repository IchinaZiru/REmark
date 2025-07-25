import csv
import os
import yaml

CSV_FILE = 'data/csv/functions.csv'
OUTPUT_DIR = 'data/yaml/requirements/0-shot'

def normalize_args(arg_str):
    return [arg.strip() for arg in arg_str.split(',') if arg.strip()] if arg_str else []

def generate_yaml(row):
    data = {
        'id': row['ID'],
        'function': row['関数名'],
        'range': [int(row['開始行']), int(row['終了行'])],
        'args': normalize_args(row['引数']),
        'return': row['戻り値'],
        'requirement': {
            'trigger': '',
            'action': '',
            'effect': '',
            'constraint': ''
        }
    }
    return data

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    with open(CSV_FILE, 'r', encoding='utf-8-sig') as f:
        reader = csv.DictReader(f)
        for row in reader:
            yaml_data = generate_yaml(row)
            file_name = f"{yaml_data['id']}_{yaml_data['function']}.yaml"
            path = os.path.join(OUTPUT_DIR, file_name)
            with open(path, 'w', encoding='utf-8') as yf:
                yaml.dump(yaml_data, yf, allow_unicode=True, sort_keys=False)

    print(f"YAMLテンプレートを {OUTPUT_DIR} に出力しました。")

if __name__ == '__main__':
    main()
