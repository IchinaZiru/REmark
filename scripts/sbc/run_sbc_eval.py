import os
import glob
import yaml
import argparse
import pandas as pd
import unicodedata
from janome.tokenizer import Tokenizer
from sklearn.metrics import f1_score
from sentence_transformers import SentenceTransformer, util
from nltk.translate.bleu_score import sentence_bleu, SmoothingFunction

# -------------------------------
# 引数とパス初期化
# -------------------------------
parser = argparse.ArgumentParser()
parser.add_argument("--level", type=int, required=True, help="コメントレベル L0〜L7")
args = parser.parse_args()
LEVEL = args.level

REF_DIR = "data/yaml/requirements"
GEN_DIR = f"data/yaml/requirements/generated_taec/L{LEVEL}"
OUT_CSV = f"outputs/sbc_results/L{LEVEL}.csv"
DEBUG_CSV = f"outputs/sbc_results/L{LEVEL}_debug.csv"
os.makedirs("outputs/sbc_results", exist_ok=True)

# -------------------------------
# モデル・トークナイザ初期化
# -------------------------------
tokenizer = Tokenizer()
model = SentenceTransformer("sonoisa/sentence-bert-base-ja-mean-tokens")
smoothie = SmoothingFunction().method1

# -------------------------------
# ユーティリティ関数
# -------------------------------
def tokenize(text):
    if not text:
        return []
    return [token.surface for token in tokenizer.tokenize(text)]

def calc_bleu(ref, gen):
    ref_tokens = tokenize(ref)
    gen_tokens = tokenize(gen)
    if not ref_tokens or not gen_tokens:
        return 0.0
    return sentence_bleu([ref_tokens], gen_tokens, weights=(0.25, 0.25, 0.25, 0.25), smoothing_function=smoothie)

def calc_completeness(ref, gen):
    ref_set = set(tokenize(ref))
    gen_set = set(tokenize(gen))
    if not ref_set:
        return 0.0
    return len(ref_set & gen_set) / len(ref_set)

def calc_semantic_similarity(ref, gen):
    if not ref or not gen:
        return 0.0
    ref_vec = model.encode(ref, convert_to_tensor=True)
    gen_vec = model.encode(gen, convert_to_tensor=True)
    return float(util.cos_sim(ref_vec, gen_vec).item())

def clean_text(val):
    if isinstance(val, str):
        return unicodedata.normalize("NFKC", val).encode("utf-8", "ignore").decode("utf-8")
    return val

# -------------------------------
# 評価処理
# -------------------------------
records = []
debug_records = []

for ref_path in glob.glob(os.path.join(REF_DIR, "*.yaml")):
    with open(ref_path, encoding="utf-8") as f:
        ref_data = yaml.safe_load(f)

    func_id = ref_data.get("id")
    func_name = ref_data.get("function")
    ref_taec = ref_data.get("requirement", {})

    gen_path = os.path.join(GEN_DIR, f"{func_id}_{func_name}.yaml")
    if not os.path.exists(gen_path):
        print(f"[!] 生成結果が見つかりません: {gen_path}")
        continue

    with open(gen_path, encoding="utf-8") as f:
        gen_data = yaml.safe_load(f)
    gen_taec = gen_data.get("taec", {})

    for key in ["trigger", "action", "effect", "constraint"]:
        ref = ref_taec.get(key, "")
        gen = gen_taec.get(key, "")

        sem_sim = calc_semantic_similarity(ref, gen)
        bleu = calc_bleu(ref, gen)
        completeness = calc_completeness(ref, gen)
        sbc = round(0.7 * sem_sim + 0.1 * bleu + 0.2 * completeness, 4)

        records.append({
            "id": func_id,
            "function": func_name,
            "field": key,
            "semantic_similarity": sem_sim,
            "bleu": bleu,
            "completeness": completeness,
            "sbc": sbc
        })

        debug_records.append({
            "id": func_id,
            "function": func_name,
            "field": key,
            "ref": ref,
            "gen": gen,
            "ref_tokens": " ".join(tokenize(ref)),
            "gen_tokens": " ".join(tokenize(gen)),
            "semantic_similarity": sem_sim,
            "bleu": bleu,
            "completeness": completeness,
            "sbc": sbc
        })

# -------------------------------
# 出力処理
# -------------------------------
df = pd.DataFrame(records).applymap(clean_text)
df.to_csv(OUT_CSV, index=False, encoding="utf-8-sig")

debug_df = pd.DataFrame(debug_records).applymap(clean_text)
debug_df.to_csv(DEBUG_CSV, index=False, encoding="utf-8-sig")

print(f"[✓] SBC(フルスコア)を保存しました → {OUT_CSV}")
print(f"[✓] デバッグ情報を保存しました → {DEBUG_CSV}")
