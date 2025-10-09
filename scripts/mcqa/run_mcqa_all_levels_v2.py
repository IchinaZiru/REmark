
import sys
import os
import re
import time
import json
import argparse
import subprocess
from datetime import datetime
from pathlib import Path

PROGRESS_DIR = Path("outputs/progress")
PROGRESS_DIR.mkdir(parents=True, exist_ok=True)

def parse_overall(stdout_text: str):
    """Parse aggregated info from run_mcqa.py stdout after it finishes."""
    total_q = None
    elapsed = None
    out_csv = None

    # e.g., "L0 用に 48 問のMCQAを gpt-4o で評価中..."
    m_total = re.search(r"L(\d+)\s*用に\s*(\d+)\s*問のMCQAを\s*(.+?)\s*で評価中", stdout_text)
    if m_total:
        total_q = int(m_total.group(2))

    # e.g., "モデル gpt-4o (L0) の総処理時間: 123.45s"
    m_elapsed = re.search(r"総処理時間:\s*([0-9.]+)s", stdout_text)
    if m_elapsed:
        elapsed = float(m_elapsed.group(1))

    # e.g., "回答収集完了 → outputs\mcqa_results\gpt-4o\L0.csv"
    m_csv = re.search(r"回答収集完了\s*→\s*([^\r\n]+)", stdout_text)
    if m_csv:
        out_csv = m_csv.group(1).strip()

    return total_q, elapsed, out_csv

def run_one_level(level: int, model: str, pool_dir: str=None, ollama_host: str=None, llamacpp_host: str=None):
    cmd = [sys.executable, "-u", "scripts/run_mcqa.py", "--level", str(level), "--model", model]
    if pool_dir:
        cmd += ["--pool_dir", pool_dir]
    if ollama_host:
        cmd += ["--ollama_host", ollama_host]
    if llamacpp_host:
        cmd += ["--llamacpp_host", llamacpp_host]

    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
        universal_newlines=True
    )

    stdout_accum = []

    # 行単位で逐次表示
    while True:
        line = proc.stdout.readline()
        if line:
            print(line, end="")
            stdout_accum.append(line)
        elif proc.poll() is not None:
            # プロセス終了。残りをまとめて読み出す
            rest_out = proc.stdout.read() or ""
            if rest_out:
                print(rest_out, end="")
                stdout_accum.append(rest_out)
            break
        else:
            time.sleep(0.02)

    returncode = proc.returncode
    stdout_text = "".join(stdout_accum)

    # 集計情報を抽出（あなたの parse 関数をそのまま利用）
    total_q, elapsed, out_csv = parse_overall(stdout_text)

    status = "ok" if returncode == 0 else "error"
    return {
        "level": level,
        "model": model,
        "total_questions": total_q if total_q is not None else 0,
        "elapsed_sec_printed": elapsed,
        "out_csv": out_csv,
        "returncode": returncode,
        "status": status,
        "stderr_snippet": "",   # ← 統合のため空でOK
    }


def main():
    ap = argparse.ArgumentParser(description="Run MCQA across multiple levels with live per-function progress")
    ap.add_argument("--model", required=True, type=str, help="モデル名（例: gpt-4o）")
    ap.add_argument("--levels", default="0-7", type=str, help="レベル指定（例: 0-7 または 0,2,4,6）")
    ap.add_argument("--pool_dir", type=str, default=None, help="MCQAプールのディレクトリ（scripts/run_mcqa.pyへ引き継ぎ）")
    ap.add_argument("--ollama_host", type=str, default=None, help="Ollama host（必要な場合）")
    ap.add_argument("--llamacpp_host", type=str, default=None, help="llama.cpp host（必要な場合）")
    ap.add_argument("--sleep_each", type=float, default=0.3, help="各レベル実行間のスリープ秒")
    args = ap.parse_args()

    # Parse level list
    if "-" in args.levels:
        a, b = args.levels.split("-", 1)
        level_list = list(range(int(a), int(b) + 1))
    else:
        level_list = [int(x) for x in args.levels.split(",") if x.strip() != ""]

    model = args.model
    tstamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    progress_log = PROGRESS_DIR / f"progress_{model.replace(':','-')}_{tstamp}.jsonl"

    print(f"=== Start sweep | model={model} | levels={level_list} ===")
    summary = []

    for lv in level_list:
        print(f"\n=== Level {lv} ===")
        rec = run_one_level(
            level=lv,
            model=model,
            pool_dir=args.pool_dir,
            ollama_host=args.ollama_host,
            llamacpp_host=args.llamacpp_host,
        )
        summary.append(rec)
        with open(progress_log, "a", encoding="utf-8") as f:
            f.write(json.dumps(rec, ensure_ascii=False) + "\n")

        # quick status line
        q = rec['total_questions']
        time_info = f"{rec['elapsed_sec_printed']}s" if rec['elapsed_sec_printed'] is not None else "-"
        print(f"[L{lv}] status={rec['status']}  questions={q}  time={time_info}  csv={rec['out_csv']}")
        time.sleep(args.sleep_each)

    # Final summary table
    print("\n=== Summary ===")
    cols = ["level", "model", "total_questions", "elapsed_sec_printed", "status", "out_csv"]
    header = " | ".join([c.center(20) for c in cols])
    print(header)
    print("-" * len(header))
    for r in summary:
        row = " | ".join([str(r.get(c, "")).ljust(20) for c in cols])
        print(row)

    print(f"\nProgress log written to: {progress_log}")

if __name__ == "__main__":
    main()
