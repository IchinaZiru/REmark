
# scripts/generate_comment_templates.py
import os, re, json, yaml, pathlib, pandas as pd
from dotenv import load_dotenv

"""
RE-5枠コメント（CONTRACT / DATAFLOW / ENTITIES&ER / UI&FLOW）を
OpenAI API + 簡易RAGで関数ごとに生成し、CSVに出力します。

期待する入力:
  - data/original/Meeting_Room_Reservation_System.c
  - data/csv/functions.csv                 # 列: 関数名, 開始行, 終了行, 行数, 引数, 戻り値
  - data/csv/REmark_Function.csv           # 機能要件（列: id, 分類, 機能名, 概要, 入力, 出力, 利用者）
  - data/csv/REmark_Table.csv              # テーブル定義（列: id, テーブル名, 項目名, データ形式, 制約, 初期値, 外部キー, 概要）
  - data/csv/REmark_Screen.csv             # 画面一覧（列: id, 分類, 画面名, 概要, 構成要素, 操作手順, 利用者）
  - data/seq/*.md                          # 任意（Mermaid/PlantUMLのテキスト）
  - config/map.yaml                        # 任意（関数名→画面名 上書きマップ）

出力:
  - data/csv/comment_templates.csv         # 列は下の OUT_COLUMNS を参照
"""

# ==== パス設定 ====
ROOT = pathlib.Path(__file__).resolve().parents[1]
SRC = ROOT/"data"/"original"/"Meeting_Room_Reservation_System.c"
FUNC_CSV = ROOT/"data"/"csv"/"functions.csv"

# Gear.indigo 由来のCSV（Excelから手動変換した想定）
FUNC_REQ_CSV = ROOT/"data"/"csv"/"REmark_Function.csv"
TAB_CSV      = ROOT/"data"/"csv"/"REmark_Table.csv"
SCR_CSV      = ROOT/"data"/"csv"/"REmark_Screen.csv"

SEQ_DIR  = ROOT/"data"/"seq"
MAP_YAML = ROOT/"config"/"map.yaml"
OUT_CSV  = ROOT/"data"/"csv"/"comment_templates_v2.csv"

OUT_COLUMNS = [
    "function_name",
    "CONTRACT_purpose","CONTRACT_pre","CONTRACT_post","CONTRACT_errors",
    "DATAFLOW_reads","DATAFLOW_writes","DATAFLOW_calls",
    "ENTITIES_entity","ER_relation",
    "UI_screen","FLOW_main",
]

# ==== OpenAI ====
load_dotenv()
from openai import OpenAI
OPENAI_MODEL = os.getenv("OPENAI_MODEL", "gpt-4o-mini")
client = OpenAI(
    api_key=os.getenv("OPENAI_API_KEY"),
    base_url=os.getenv("OPENAI_BASE_URL") or None,
)

# ==== 簡易RAGユーティリティ ====
def read_text(p: pathlib.Path) -> str:
    return p.read_text(encoding="utf-8") if p.exists() else ""

def load_seq_texts() -> str:
    if not SEQ_DIR.exists(): return ""
    return "\n\n".join(read_text(p) for p in SEQ_DIR.glob("*.md"))

def tokenize_keywords(fname: str) -> list[str]:
    # CamelCase/SnakeCase/数字をほどよく分割
    parts = re.findall(r"[A-Z]?[a-z]+|[A-Z]+(?![a-z])|\d+", fname)
    return [p.lower() for p in parts if p]

def score_text_by_keys(text: str, keys: list[str]) -> int:
    s = text.lower()
    return sum(s.count(k) for k in keys)

def df_pick_rows(df: pd.DataFrame, keys: list[str], columns_priority: list[str] | None, topn=3, clip=600) -> str:
    """指定キーに基づいて関連行をスコアリングし上位を連結。"""
    if df is None or df.empty: return ""
    rows = []
    for _, r in df.iterrows():
        if columns_priority:
            fields = [str(r[c]) for c in columns_priority if c in r and pd.notna(r[c])]
        else:
            fields = [str(x) for x in r.values if pd.notna(x)]
        txt = " / ".join(fields)
        sc = score_text_by_keys(txt, keys)
        if sc>0:
            rows.append((sc, txt))
    if not rows: return ""
    rows.sort(key=lambda x: x[0], reverse=True)
    merged = " / ".join(t for _, t in rows[:topn])
    return merged[:clip]

def parse_args(arg_str: str) -> list[str]:
    if not isinstance(arg_str, str) or not arg_str.strip(): return []
    return [p.strip() for p in arg_str.split(",") if p.strip()]

def guess_reads_from_args(args_list: list[str]) -> str:
    names = []
    for a in args_list:
        toks = a.replace("*"," * ").split()
        if toks: names.append(toks[-1])
    return ", ".join(names)

def guess_writes_from_args_return(args_list: list[str], ret_type: str) -> str:
    writes=[]
    for a in args_list:
        low=a.lower()
        if "*" in a or "ptr" in low or "&" in a:
            var = a.replace("*"," * ").split()[-1]
            writes.append(var)
    if isinstance(ret_type,str) and ret_type.strip() and ret_type.strip().lower()!="void":
        writes.append(f"return {ret_type.strip()}")
    return ", ".join(writes)

def domain_by_name(fname: str) -> str:
    f=fname.lower()
    if "user" in f: return "User"
    if "room" in f: return "Room"
    if "reserv" in f: return "Reservation"
    if "menu" in f or "screen" in f or "display" in f: return "UI"
    return "Generic"

def build_entities_and_er(df_tab: pd.DataFrame):
    """REmark_Table.csv から代表的なエンティティ記述を構築。無ければデフォルトにフォールバック。"""
    default_entities = {
        "User": "User(id PK, 名前, メールアドレス)",
        "Room": "Room(id PK, 部屋名, 収容人数, 説明)",
        "Reservation": ("Reservation(id PK, userId FK->User.id, roomId FK->Room.id, "
                        "開始日時, 終了日時, ステータス)")
    }
    default_er = "Reservation.userId→User.id; Reservation.roomId→Room.id"

    if df_tab is None or df_tab.empty:
        return default_entities, default_er

    # 簡易: テーブル名ごとに代表列を拾って1行にまとめる
    entities = {}
    for tname in ["ユーザー","User","会議室","Room","予約","Reservation"]:
        sub = df_tab[df_tab["テーブル名"].astype(str).str.lower().str.contains(tname.lower(), na=False)]
        if not sub.empty:
            cols = []
            for _, r in sub.iterrows():
                col = r.get("項目名", "")
                key = r.get("制約", "")
                if isinstance(col, str) and col:
                    cols.append(f"{col} {f'({key})' if isinstance(key,str) and key else ''}".strip())
            label = "User" if "user" in tname.lower() or "ユーザー" in tname else \
                    "Room" if "room" in tname.lower() or "会議室" in tname else \
                    "Reservation"
            if cols:
                entities[label] = f"{label}(" + ", ".join(cols[:8]) + ")"
    # ERはFK欄を走査して主な対応を見つける
    er_pairs = []
    if "外部キー" in df_tab.columns and "テーブル名" in df_tab.columns and "項目名" in df_tab.columns:
        for _, r in df_tab.iterrows():
            fk = str(r.get("外部キー",""))
            if "fk" in fk.lower() or "->" in fk or "." in fk:
                src_table = str(r.get("テーブル名",""))
                src_col   = str(r.get("項目名",""))
                # fkとして "FK(Room.id)" のような記述を想定
                m = re.search(r"\(?\s*FK\s*\(\s*([A-Za-z_ぁ-んァ-ヶ一-龥]+)\.([A-Za-z_]\w*)\s*\)\s*\)?", fk, re.I)
                if m:
                    ref_table, ref_col = m.group(1), m.group(2)
                    er_pairs.append(f"{src_table}.{src_col}→{ref_table}.{ref_col}")
    er = " ; ".join(er_pairs) if er_pairs else default_er

    # デフォルト補完
    for k,v in default_entities.items():
        entities.setdefault(k, v)
    return entities, er

def screen_hint(df_screen: pd.DataFrame, fname: str, map_yaml: dict) -> str:
    if map_yaml and fname in map_yaml:
        s = map_yaml[fname].get("screen")
        if s: return s
    if df_screen is not None and "画面名" in df_screen.columns:
        m = df_screen[df_screen["画面名"].astype(str).str.contains(fname, case=False, na=False)]
        if not m.empty: return str(m["画面名"].iloc[0])
    return ""

def flow_hint_from_seq(seq_texts: str, keys: list[str]) -> str:
    if not seq_texts: return ""
    lines = []
    for ln in seq_texts.splitlines():
        l = ln.lower()
        if any(k in l for k in keys):
            lines.append(ln.strip())
    return " / ".join(lines[:8])[:600]

def extract_code_by_range(lines: list[str], start_1b: int, end_1b: int, pad=0) -> str:
    s = max(0, start_1b-1-pad); e = min(len(lines), end_1b+pad)
    return "".join(lines[s:e])

def fallback(a, b):
    return a if (isinstance(a,str) and a.strip()) else b

# ==== プロンプト ====
PROMPT_TMPL = """You are given:
1) C function: {fname}
   - return: {ret_type}
   - args: {arg_list}
   - loc: {loc_count} lines
2) C source (full function):
```
{code}
```
3) Requirements/notes (Japanese):
{req}

4) Entities and ER (Japanese):
- {entity_user}
- {entity_room}
- {entity_res}
- ER: {er}
   (domain hint: {domain})

5) Screen hint (Japanese): {screen}
6) Sequence hints (Japanese): {flow}

Output a single JSON object with this schema (Japanese values):
{{
  "CONTRACT": {{"purpose": "...", "pre": "...", "post": "...", "errors": "..."}},
  "DATAFLOW": {{"reads": "...", "writes": "...", "calls": "..."}},
  "ENTITIES_ER": {{"entity": "...", "er": "..."}},
  "UI_FLOW": {{"screen": "...", "flow": "..."}}
}}
No explanations, JSON only.
"""

def call_llm(prompt: str) -> str:
    resp = client.chat.completions.create(
        model=OPENAI_MODEL,
        messages=[
            {"role":"system","content":"Return ONLY valid JSON. Keep answers concise."},
            {"role":"user","content":prompt}
        ],
        temperature=0.2,
    )
    text = resp.choices[0].message.content
    # JSON抽出（保険）
    m = re.search(r"\{.*\}\s*$", text, re.S)
    return m.group(0) if m else text

def main():
    # 入力読み込み
    df_funcs = pd.read_csv(FUNC_CSV, encoding="utf-8-sig")
    df_req   = pd.read_csv(FUNC_REQ_CSV, encoding="utf-8-sig") if FUNC_REQ_CSV.exists() else None
    df_tab   = pd.read_csv(TAB_CSV,      encoding="utf-8-sig") if TAB_CSV.exists() else None
    df_scr   = pd.read_csv(SCR_CSV,      encoding="utf-8-sig") if SCR_CSV.exists() else None
    map_yaml = yaml.safe_load(read_text(MAP_YAML)) if MAP_YAML.exists() else {}
    seq_texts = load_seq_texts()

    code_lines = read_text(SRC).splitlines(True)
    entities, er_default = build_entities_and_er(df_tab)

    out_rows = []

    for _, row in df_funcs.iterrows():
        fname = str(row["関数名"])
        start = int(row["開始行"])
        end   = int(row["終了行"])
        loc   = int(row.get("行数", max(0, end-start+1)))

        arg_str  = str(row["引数"]) if pd.notna(row.get("引数")) else ""
        ret_type = str(row["戻り値"]) if pd.notna(row.get("戻り値")) else "void"
        args_list = parse_args(arg_str)

        # ヒューリスティック初期値
        reads_guess  = guess_reads_from_args(args_list)
        writes_guess = guess_writes_from_args_return(args_list, ret_type)
        domain = domain_by_name(fname)
        keys = tokenize_keywords(fname) + [domain.lower()]

        # コード・簡易RAG
        code   = extract_code_by_range(code_lines, start, end, pad=0)

        # REmark_Function.csv から要件を抽出（列優先度を指定）
        req_cols_priority = ["機能名","概要","入力","出力","分類","利用者"]
        req    = df_pick_rows(df_req, keys, req_cols_priority, topn=3, clip=900) if df_req is not None else ""

        # 画面ヒント
        screen = screen_hint(df_scr, fname, map_yaml)

        # シーケンス（テキスト）からのヒント
        flow   = flow_hint_from_seq(seq_texts, keys)

        # エンティティ/ER（テーブル定義から構築）
        ent_user = entities.get("User", "User(id PK, ...)")
        ent_room = entities.get("Room", "Room(id PK, ...)")
        ent_res  = entities.get("Reservation", "Reservation(id PK, ...)")
        er_chain = er_default

        prompt = PROMPT_TMPL.format(
            fname=fname, ret_type=ret_type or "(void)", arg_list=arg_str or "(none)",
            loc_count=loc, code=code, req=req,
            entity_user=ent_user, entity_room=ent_room, entity_res=ent_res,
            er=er_chain, domain=domain,
            screen=screen, flow=flow
        )

        # LLM 実行（JSONを期待）
        try:
            raw = call_llm(prompt)
            parsed = json.loads(raw)
        except Exception:
            # LLM失敗でも最低限埋める
            parsed = {
              "CONTRACT":{"purpose":"","pre":"","post":"","errors":""},
              "DATAFLOW":{"reads":reads_guess,"writes":writes_guess,"calls":""},
              "ENTITIES_ER":{"entity":ent_res, "er":er_chain},
              "UI_FLOW":{"screen":screen, "flow":flow}
            }

        out_rows.append({
            "function_name": fname,
            "CONTRACT_purpose": parsed.get("CONTRACT",{}).get("purpose",""),
            "CONTRACT_pre":     parsed.get("CONTRACT",{}).get("pre",""),
            "CONTRACT_post":    parsed.get("CONTRACT",{}).get("post",""),
            "CONTRACT_errors":  parsed.get("CONTRACT",{}).get("errors",""),
            "DATAFLOW_reads":   (parsed.get("DATAFLOW",{}).get("reads","")  or reads_guess),
            "DATAFLOW_writes":  (parsed.get("DATAFLOW",{}).get("writes","") or writes_guess),
            "DATAFLOW_calls":   parsed.get("DATAFLOW",{}).get("calls",""),
            "ENTITIES_entity":  parsed.get("ENTITIES_ER",{}).get("entity", ent_res),
            "ER_relation":      parsed.get("ENTITIES_ER",{}).get("er", er_chain),
            "UI_screen":        parsed.get("UI_FLOW",{}).get("screen", screen),
            "FLOW_main":        parsed.get("UI_FLOW",{}).get("flow", flow),
        })

    df_out = pd.DataFrame(out_rows, columns=OUT_COLUMNS)
    OUT_CSV.parent.mkdir(parents=True, exist_ok=True)
    df_out.to_csv(OUT_CSV, index=False, encoding="utf-8-sig")
    print(f"wrote: {OUT_CSV}")

if __name__ == "__main__":
    main()
