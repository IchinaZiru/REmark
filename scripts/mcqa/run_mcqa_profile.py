import argparse, shutil, subprocess, sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent

# --- Robust project root detection ---
if (SCRIPT_DIR / "data").exists() and (SCRIPT_DIR / "run_mcqa.py").exists():
    # Script is at project root
    PROJ = SCRIPT_DIR
elif (SCRIPT_DIR.parent / "data").exists():
    # Script is inside <proj>/scripts/
    PROJ = SCRIPT_DIR.parent
else:
    # Fallback: assume parent is project root
    PROJ = SCRIPT_DIR.parent

RUN_MCQA_CANDIDATES = [
    PROJ / "run_mcqa.py",
    PROJ / "scripts" / "run_mcqa.py",
]

INTERNAL_LEVEL = 99  # hidden numeric level for run_mcqa.py

def find_run_mcqa() -> Path:
    for p in RUN_MCQA_CANDIDATES:
        if p.exists():
            return p
    raise FileNotFoundError("run_mcqa.py not found. Checked:\n  " + "\n  ".join(str(p) for p in RUN_MCQA_CANDIDATES))

def ensure_dir(p: Path):
    p.mkdir(parents=True, exist_ok=True)

def resolve_profile_source(profile: str) -> Path:
    p1 = PROJ / "data" / "levels" / profile / f"Meeting_Room_Reservation_System_{profile}.c"
    if p1.exists():
        return p1
    raise FileNotFoundError(
        f"Could not find C file for profile '{profile}'. Tried:\n  {p1}\n"
        "Hint: generate with insert_comments.py --profile {profile} or copy your C file there."
    )

def stage_for_run(profile_src: Path, level: int = INTERNAL_LEVEL) -> Path:
    dst_dir = PROJ / "data" / "levels" / f"L{level}"
    ensure_dir(dst_dir)
    dst = dst_dir / f"Meeting_Room_Reservation_System_L{level}.c"
    shutil.copyfile(profile_src, dst)
    return dst

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("--profile", required=True,
                    help="Profile name(s): full,contract,dataflow,entities,ui (comma-separated allowed)")
    ap.add_argument("--model", required=True, help="Model name passed to run_mcqa.py")
    ap.add_argument("--python", default=sys.executable, help="Python interpreter for run_mcqa.py")
    ap.add_argument("--extra", default="", help="Extra CLI args (single string) to append to run_mcqa.py")
    args = ap.parse_args()

    run_mcqa = find_run_mcqa()
    profiles = [p.strip().lower() for p in args.profile.split(",") if p.strip()]

    for p in profiles:
        src = resolve_profile_source(p)
        staged = stage_for_run(src, INTERNAL_LEVEL)

        cmd = [args.python, str(run_mcqa), "--level", str(INTERNAL_LEVEL), "--model", args.model]
        if args.extra:
            cmd += args.extra.split()

        print(f"[INFO] Project root: {PROJ}")
        print(f"[INFO] Profile '{p}' -> staged at {staged}")
        print(f"[INFO] Running: {' '.join(cmd)}")
        ret = subprocess.run(cmd)
        if ret.returncode != 0:
            print(f"[ERROR] run_mcqa.py exited with code {ret.returncode} for profile '{p}'", file=sys.stderr)
            sys.exit(ret.returncode)
        else:
            print(f"[OK] Finished profile '{p}'.")
            
            out_dir = PROJ / "outputs" / "mcqa_results" / args.model.replace("/", "_")
            out_csv = out_dir / f"L{INTERNAL_LEVEL}.csv"

            if out_csv.exists():
                # extra 引数から limit / offset を抜いてファイル名に付ける
                import re
                limit = re.search(r"--limit\s+(\d+)", args.extra)
                offset = re.search(r"--offset\s+(\d+)", args.extra)
                limit_val = limit.group(1) if limit else "all"
                offset_val = offset.group(1) if offset else "0"

                new_name = out_dir / f"L{INTERNAL_LEVEL}_off{offset_val}_lim{limit_val}.csv"
                shutil.copyfile(out_csv, new_name)
                print(f"[INFO] Saved copy as {new_name}")

if __name__ == "__main__":
    main()
