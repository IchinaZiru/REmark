
import os
import glob
import argparse
import pandas as pd
import matplotlib.pyplot as plt

# Supported code extensions to try when resolving "<function>.<ext>"
CODE_EXTS = [".c", ".cpp", ".h", ".hpp", ".py", ".java", ".js", ".ts", ".cs", ".go", ".rs"]

def parse_levels(levels_arg: str):
    levels_arg = str(levels_arg).strip()
    if "-" in levels_arg:
        a, b = levels_arg.split("-", 1)
        return list(range(int(a), int(b) + 1))
    return [int(x) for x in levels_arg.split(",") if x.strip() != ""]

def read_text(path: str) -> str:
    try:
        with open(path, encoding="utf-8") as f:
            return f.read()
    except Exception:
        try:
            with open(path, encoding="cp932", errors="ignore") as f:
                return f.read()
        except Exception:
            return ""

def count_tokens(text: str, mode: str = "words") -> int:
    if not text:
        return 0
    if mode == "chars":
        return len(text)
    return len(text.split())  # whitespace split

def find_code_file(levels_root: str, level: int, func: str):
    """Find a code file in levels_root/L{level}/ named like <func>.<ext> where ext in CODE_EXTS.
       If not found, fallback to any file in that level folder that contains func in the basename."""
    lvl_dir = os.path.join(levels_root, f"L{level}")
    # 1) exact name match with known extensions
    for ext in CODE_EXTS:
        p = os.path.join(lvl_dir, f"{func}{ext}")
        if os.path.exists(p):
            return p
    # 2) fuzzy: any file whose stem contains func
    for ext in CODE_EXTS:
        for p in glob.glob(os.path.join(lvl_dir, f"*{func}*{ext}")):
            if os.path.exists(p):
                return p
    # 3) as a last resort, if the folder has a single code file, use it
    candidates = []
    for ext in CODE_EXTS:
        candidates.extend(glob.glob(os.path.join(lvl_dir, f"*{ext}")))
    if len(candidates) == 1:
        return candidates[0]
    return None

def compute_avg_tokens_per_level(scored_root: str, model: str, levels_root: str, levels, token_mode: str = "words"):
    """For each level, read L<level>_scored.csv to get function names, open code files under levels_root/L<level>,
       and compute average token counts of the *actual code with comments*."""
    model_dir = os.path.join(scored_root, model.replace(":", "-"))
    rows = []
    for lv in levels:
        scored_csv = os.path.join(model_dir, f"L{lv}_scored.csv")
        total = 0
        n = 0
        missing = 0
        funcs = []
        if os.path.exists(scored_csv):
            try:
                df = pd.read_csv(scored_csv)
                funcs = [str(x).strip() for x in df.get("function", []) if str(x).strip()]
            except Exception:
                funcs = []
        # Deduplicate to avoid counting the same function twice
        funcs = list(dict.fromkeys(funcs))
        for func in funcs:
            code_path = find_code_file(levels_root, lv, func)
            if not code_path:
                missing += 1
                continue
            text = read_text(code_path)
            total += count_tokens(text, mode=token_mode)
            n += 1
        avg_tok = (total / n) if n > 0 else 0.0
        rows.append({
            "level": lv,
            "avg_tokens": avg_tok,
            "num_functions": n,
            "num_missing": missing
        })
    return pd.DataFrame(rows)

def main():
    ap = argparse.ArgumentParser(description="Visualize accuracy & token counts from *level-specific* code files")
    ap.add_argument("--model", required=True, help="Model name (e.g., gpt-4o or llama3.1:8b-instruct-fp16)")
    ap.add_argument("--levels", default="0-7", help="e.g. 0-7 or 0,2,4,6")
    ap.add_argument("--scored_root", default="outputs/mcqa_scored")
    ap.add_argument("--levels_root", default="data/levels", help="Root folder containing L0/, L1/, ...")
    ap.add_argument("--out_dir", default="outputs/mcqa_viz")
    ap.add_argument("--token_mode", choices=["words","chars"], default="words")
    args = ap.parse_args()

    levels = parse_levels(args.levels)
    model_sanitized = args.model.replace(":", "-")

    # 1) Load accuracy by level from summary
    summary_path = os.path.join(args.scored_root, model_sanitized, "summary.csv")
    if not os.path.exists(summary_path):
        raise FileNotFoundError(f"Summary CSV not found: {summary_path}")
    sm = pd.read_csv(summary_path)
    if "level" not in sm.columns or "accuracy" not in sm.columns:
        raise ValueError("summary.csv must have columns: level, accuracy")
    sm["level"] = pd.to_numeric(sm["level"], errors="coerce")
    sm = sm.dropna(subset=["level","accuracy"]).sort_values("level")
    sm = sm[sm["level"].isin(levels)]

    # 2) Compute average tokens per level from *actual* level files
    tok = compute_avg_tokens_per_level(args.scored_root, model_sanitized, args.levels_root, levels, token_mode=args.token_mode)

    # 3) Merge
    merged = pd.merge(sm[["level","accuracy"]], tok, on="level", how="left").sort_values("level")

    # 4) Output directory
    out_dir = os.path.join(args.out_dir, model_sanitized, f"levels_tokens_{args.token_mode}")
    os.makedirs(out_dir, exist_ok=True)

    # 5) Plot: accuracy by level
    plt.figure(figsize=(7,5))
    plt.plot(merged["level"], merged["accuracy"], marker="o")
    plt.xlabel("Comment Level")
    plt.ylabel("Accuracy")
    plt.ylim(0,1.0)
    plt.grid(True)
    plt.title(f"Accuracy by Comment Level ({args.model})")
    plt.tight_layout()
    path1 = os.path.join(out_dir, "accuracy_by_level.png")
    plt.savefig(path1); plt.close()
    print(f"Saved: {path1}")

    # 6) Plot: avg tokens by level (from actual files)
    plt.figure(figsize=(7,5))
    plt.plot(merged["level"], merged["avg_tokens"], marker="o")
    plt.xlabel("Comment Level")
    plt.ylabel(f"Avg Tokens in Code ({args.token_mode})")
    plt.grid(True)
    plt.title(f"Avg Token Length by Level (from code files) ({args.model})")
    plt.tight_layout()
    path2 = os.path.join(out_dir, "avg_tokens_by_level.png")
    plt.savefig(path2); plt.close()
    print(f"Saved: {path2}")

    # 7) Plot: tokens vs accuracy (scatter)
    plt.figure(figsize=(7,5))
    x = merged["avg_tokens"].tolist()
    y = merged["accuracy"].tolist()
    plt.scatter(x, y)
    for xv, yv, lv in zip(x, y, merged["level"].tolist()):
        plt.text(xv, yv, f"L{lv}", ha="left", va="bottom")
    plt.xlabel(f"Avg Tokens in Code per Level ({args.token_mode})")
    plt.ylabel("Accuracy")
    plt.ylim(0,1.0)
    plt.grid(True)
    plt.title(f"Accuracy vs Avg Tokens (from code files) ({args.model})")
    plt.tight_layout()
    path3 = os.path.join(out_dir, "accuracy_vs_tokens.png")
    plt.savefig(path3); plt.close()
    print(f"Saved: {path3}")

    # 8) Also save merged CSV for reference
    merged_path = os.path.join(out_dir, "level_accuracy_tokens_merged.csv")
    merged.to_csv(merged_path, index=False, encoding="utf-8")
    print(f"Saved: {merged_path}")

if __name__ == "__main__":
    main()
