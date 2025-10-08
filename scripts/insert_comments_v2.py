import pandas as pd, pathlib, re, argparse
"""
RE-5枠コメントを挿入するスクリプト

# フルコメント（RE-5すべて）を挿入
python scripts/insert_comments.py --profile full

# 部分的に試す場合
python scripts/insert_comments.py --profile contract
python scripts/insert_comments.py --profile dataflow
python scripts/insert_comments.py --profile entities
python scripts/insert_comments.py --profile ui
"""

ROOT = pathlib.Path(__file__).resolve().parents[1]
SRC = ROOT/"data"/"original"/"Meeting_Room_Reservation_System.c"
DST_DIR = ROOT/"data"/"levels"
FUNC_CSV = ROOT/"data"/"csv"/"functions.csv"
TPL_CSV  = ROOT/"data"/"csv"/"comment_templates_v2.csv"

BEGIN_MARK = "// === RE5 BEGIN ==="
END_MARK   = "// === RE5 END ==="
PROFILE_CHOICES = ["contract","dataflow","entities","ui","full"]

def load_templates():
    df = pd.read_csv(TPL_CSV, encoding="utf-8-sig").set_index("function_name")
    return df.to_dict(orient="index")

def build_block(tpl: dict, profile: str) -> str:
    lines=[]
    def add(title, *items):
        items=[s for s in items if s and str(s).strip()]
        if items:
            lines.append(f"// {title}")
            for s in items: lines.append(f"// {s.strip()}")

    if profile in ("contract","full"):
        add("CONTRACT",
            f"@purpose {tpl.get('CONTRACT_purpose','')}",
            f"@pre     {tpl.get('CONTRACT_pre','')}",
            f"@post    {tpl.get('CONTRACT_post','')}",
            f"@errors  {tpl.get('CONTRACT_errors','')}",
        )
    if profile in ("dataflow","full"):
        add("DATAFLOW",
            f"@reads   {tpl.get('DATAFLOW_reads','')}",
            f"@writes  {tpl.get('DATAFLOW_writes','')}",
            f"@calls   {tpl.get('DATAFLOW_calls','')}",
        )
    if profile in ("entities","full"):
        add("ENTITIES & ER",
            f"@entity  {tpl.get('ENTITIES_entity','')}",
            f"@er      {tpl.get('ER_relation','')}",
        )
    if profile in ("ui","full"):
        add("UI & FLOW",
            f"@screen  {tpl.get('UI_screen','')}",
            f"@flow    {tpl.get('FLOW_main','')}",
        )

    if not lines: return ""
    return BEGIN_MARK+"\n"+("\n".join(lines))+"\n"+END_MARK+"\n"

def strip_blocks(text: str) -> str:
    pattern = re.escape(BEGIN_MARK)+r".*?"+re.escape(END_MARK)+r"\n?"
    return re.sub(pattern, "", text, flags=re.S)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--profile", required=True, choices=PROFILE_CHOICES)
    args = ap.parse_args()

    original = SRC.read_text(encoding="utf-8")
    text = strip_blocks(original)
    lines = text.splitlines(True)

    funcs = pd.read_csv(FUNC_CSV, encoding="utf-8-sig")
    tpl = load_templates()

    for _, r in funcs.iterrows():
        name = str(r["関数名"])
        start = int(r["開始行"]) - 1
        if name not in tpl:
            print(f"skip(no template): {name}")
            continue
        b = build_block(tpl[name], args.profile)
        lines[start] = b + lines[start]
        print(f"inserted: {name}")

    out = DST_DIR/args.profile/f"Meeting_Room_Reservation_System_{args.profile}.c"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("".join(lines), encoding="utf-8")
    print(f"wrote: {out}")

if __name__=="__main__":
    main()
