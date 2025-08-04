import subprocess
import sys

MODEL = "deepseek-r1:14b"
LEVELS = range(0, 8)

for level in LEVELS:
    print(f"\n=== Running MCQA for Level {level} with model {MODEL} ===\n")
    result = subprocess.run([
        sys.executable,
        "scripts/run_mcqa.py",
        "--level", str(level),
        "--model", MODEL,
        "--ollama_host", "http://172.23.203.28:11434"  # 明示的に指定
    ], capture_output=True, text=True)
    
    print(result.stdout)
    if result.stderr:
        print("ERROR:", result.stderr)
