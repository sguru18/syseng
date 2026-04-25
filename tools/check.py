#!/usr/bin/env python3
"""
Shared test runner for syseng workbook.

Usage from C test binaries:
    The C test harness (common.h) handles pass/fail directly.
    This script wraps execution for timing, color, and summary.

Usage:
    python3 check.py ./test_binary [args...]
    python3 check.py --valgrind ./test_binary [args...]
"""

import subprocess
import sys
import time
import os

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
BOLD = "\033[1m"
RESET = "\033[0m"

def run_test(cmd, use_valgrind=False):
    if use_valgrind:
        cmd = ["valgrind", "--leak-check=full", "--error-exitcode=99"] + cmd

    start = time.time()
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=30,
        )
    except subprocess.TimeoutExpired:
        elapsed = time.time() - start
        print(f"{RED}{BOLD}TIMEOUT{RESET} after {elapsed:.1f}s")
        return 1
    except FileNotFoundError:
        print(f"{RED}Error: {cmd[0]} not found. Did you run 'make'?{RESET}")
        return 1

    elapsed = time.time() - start

    if result.stdout:
        print(result.stdout, end="")
    if result.stderr:
        print(f"{YELLOW}{result.stderr}{RESET}", end="")

    print(f"\n{CYAN}Completed in {elapsed:.3f}s{RESET}")

    if result.returncode == 0:
        print(f"{GREEN}{BOLD}ALL TESTS PASSED{RESET}")
    elif use_valgrind and result.returncode == 99:
        print(f"{RED}{BOLD}MEMORY ERRORS DETECTED{RESET}")
    else:
        print(f"{RED}{BOLD}SOME TESTS FAILED (exit code {result.returncode}){RESET}")

    return result.returncode


def main():
    args = sys.argv[1:]
    use_valgrind = False

    if "--valgrind" in args:
        args.remove("--valgrind")
        use_valgrind = True

    if not args:
        print(f"Usage: {sys.argv[0]} [--valgrind] ./test_binary [args...]")
        sys.exit(1)

    sys.exit(run_test(args, use_valgrind))


if __name__ == "__main__":
    main()
