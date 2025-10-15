
import os
import argparse
import pandas as pd
import matplotlib.pyplot as plt

def parse_levels(levels_arg: str):
    levels_arg = str(levels_arg).strip()
    if "-" in levels_arg:
        a, b = levels_arg.split("-", 1)
        return list(range(int(a), int(b) + 1))
    return [int(x) for x in levels_arg.split(",") if x.strip() != ""]

def read_total_elapsed_seconds(results_root: str, model: str, levels):
    """Read TOTAL_ELAPSED_TIME(sec) from outputs/mcqa_results/<model>/L<level>.csv."""
    model_dir = os.path.join(results_root, model.replace(":", "-"))
    out = {}
    for lv in levels:
        csv_path = os.path.join(model_dir, f"L{lv}.csv")
        sec = 0.0
        if os.path.exists(csv_path):
            try:
                with open(csv_path, encoding="utf-8") as f:
                    lines = [line.strip() for line in f if line.strip()]
                for line in reversed(lines[-5:]):
                    if line.startswith("TOTAL_ELAPSED_TIME(sec)"):
                        parts = line.split(",")
                        if len(parts) == 2:
                            sec = float(parts[1])
                        break
            except Exception:
                sec = 0.0
        out[lv] = sec
    return out

def main():
    ap = argparse.ArgumentParser(description="Plot Accuracy vs Total Time by Level")
    ap.add_argument("--model", required=True)
    ap.add_argument("--levels", default="0-7")
    ap.add_argument("--scored_root", default="outputs/mcqa_scored")
    ap.add_argument("--results_root", default="outputs/mcqa_results")
    ap.add_argument("--out_dir", default="outputs/mcqa_viz")
    args = ap.parse_args()

    levels = parse_levels(args.levels)
    model_sanitized = args.model.replace(":", "-")

    # summary.csv を読む（正答率）
    summary_path = os.path.join(args.scored_root, model_sanitized, "summary.csv")
    if not os.path.exists(summary_path):
        raise FileNotFoundError(f"Summary CSV not found: {summary_path}")
    df = pd.read_csv(summary_path)

    # level列を整形・ソート
    if "level" not in df.columns or "accuracy" not in df.columns:
        raise ValueError("summary.csv must have columns: level, accuracy")
    df["level"] = pd.to_numeric(df["level"], errors="coerce")
    df = df.dropna(subset=["level", "accuracy"]).sort_values("level")
    df = df[df["level"].isin(levels)]

    # 実行時間を results から取得（無ければ0.0）
    times_map = read_total_elapsed_seconds(args.results_root, model_sanitized, levels)
    times = [float(times_map.get(int(lv), 0.0)) for lv in df["level"].tolist()]

    # 図を作成（左=Accuracy 折れ線、右=Time 棒）
    fig, ax1 = plt.subplots(figsize=(8, 5))
    ax1.set_xlabel("Level")
    ax1.set_ylabel("Accuracy")
    ax1.plot(df["level"], df["accuracy"], marker="o")
    ax1.set_ylim(0, 1.0)
    ax1.grid(True)

    ax2 = ax1.twinx()
    ax2.set_ylabel("Total Time (sec)")
    ax2.bar(df["level"], times, alpha=0.3)

    plt.title(f"Accuracy vs Time by Level ({args.model})")

    out_dir = os.path.join(args.out_dir, model_sanitized)
    os.makedirs(out_dir, exist_ok=True)
    out_path = os.path.join(out_dir, "accuracy_vs_time.png")
    fig.tight_layout()
    plt.savefig(out_path)
    print(f"Saved: {out_path}")

if __name__ == "__main__":
    main()
