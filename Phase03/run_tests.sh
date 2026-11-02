#!/usr/bin/env bash
# Step 1: run every Phase01 input and temporarily save the console outputs.
# Usage: bash Phase03/run_tests.sh [path/to/compiled/program]
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
TESTS_DIR="$PROJECT_ROOT/Phase01/tests"
WORK_DIR="$SCRIPT_DIR/.test-work"
RESULTS_DIR="$SCRIPT_DIR/test-results"

if [ "$#" -gt 1 ]; then
    echo "Usage: bash Phase03/run_tests.sh [path/to/compiled/program]" >&2
    exit 2
fi
if [ ! -d "$TESTS_DIR/data" ]; then
    echo "Missing test data: $TESTS_DIR/data" >&2
    exit 1
fi

# Start with clean, temporary test outputs. The final results will be in results.txt.
rm -rf "$WORK_DIR"
mkdir -p "$WORK_DIR" "$RESULTS_DIR"

if [ "$#" -eq 1 ]; then
    PROGRAM="$1"
    if [ ! -x "$PROGRAM" ]; then
        echo "Program not executable: $PROGRAM" >&2
        exit 1
    fi
else
    if ! command -v g++ >/dev/null 2>&1; then
        echo "g++ not found. Install g++ or pass a compiled program." >&2
        exit 1
    fi
    PROGRAM="$WORK_DIR/SQA_G12.exe"
    echo "Compiling the C++ application..."
    g++ -std=c++14 -I"$PROJECT_ROOT/Phase02" \
        "$PROJECT_ROOT/Phase02/main.cpp" \
        "$PROJECT_ROOT/Phase02/FrontEnd.cpp" \
        "$PROJECT_ROOT/Phase02/TransactionManager.cpp" \
        "$PROJECT_ROOT/Phase02/FileManager.cpp" \
        "$PROJECT_ROOT/Phase02/User.cpp" \
        -o "$PROGRAM"
fi

count=0
errors=0
for input_file in "$TESTS_DIR"/*/*/input.txt; do
    [ -f "$input_file" ] || continue
    case_dir="$(dirname "$input_file")"
    relative_case="${case_dir#"$TESTS_DIR"/}"
    case_work="$WORK_DIR/$relative_case"
    mkdir -p "$case_work/data"

    # Each test runs with a fresh copy of the supplied Phase01 data.
    cp "$TESTS_DIR"/data/*.txt "$case_work/data/"

    # The program requires a transaction output path, but we do not report it.
    exit_code=0
    "$PROGRAM" "$case_work/data" "$case_work/unused-transactions.txt" \
        < "$input_file" > "$case_work/actual_output.txt" \
        2> "$case_work/stderr.txt" || exit_code=$?
        printf '%s\n' "$exit_code" > "$case_work/exit_code.txt"
    if [ "$exit_code" -ne 0 ]; then
        errors=$((errors + 1))
    fi
    count=$((count + 1))
done

if [ "$count" -eq 0 ]; then
    echo "No test cases found under Phase01/tests." >&2
    exit 1
fi

echo "Ran $count test cases ($errors returned a nonzero exit code)."
echo "Now run: bash Phase03/check_outputs.sh"