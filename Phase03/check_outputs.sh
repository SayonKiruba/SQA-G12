#!/usr/bin/env bash
# Step 2: create one results.txt with each test case's actual and expected output.
# Usage: bash Phase03/check_outputs.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$SCRIPT_DIR/../Phase01/tests"
WORK_DIR="$SCRIPT_DIR/.test-work"
RESULTS_DIR="$SCRIPT_DIR/test-results"

if [ ! -d "$WORK_DIR" ]; then
    echo "No test outputs found. First run: bash Phase03/run_tests.sh" >&2
    exit 1
fi

mkdir -p "$RESULTS_DIR"

# Find the next available run number
run_number=1

while true; do
    RUN_DIR="$RESULTS_DIR/$(printf 'run-%03d' "$run_number")"

    if mkdir "$RUN_DIR" 2>/dev/null; then
        break
    fi

    run_number=$((run_number + 1))
done

# Create one result file for this run
RESULTS_FILE="$RUN_DIR/result.txt"
: > "$RESULTS_FILE"


count=0
full_passes=0
failed=0

for input_file in "$TESTS_DIR"/*/*/input.txt; do
    [ -f "$input_file" ] || continue
    case_dir="$(dirname "$input_file")"
    case_name="$(basename "$case_dir")"
    relative_case="${case_dir#"$TESTS_DIR"/}"
    actual="$WORK_DIR/$relative_case/actual_output.txt"
    expected="$case_dir/output.txt"
    exit_status_file="$WORK_DIR/$relative_case/exit_code.txt"

    # Compare actual output with expected output
    if [ -f "$actual" ] && [ -f "$expected" ] && \
    [ -f "$exit_status_file" ] && \
    [ "$(cat "$exit_status_file")" = "0" ] && \
    cmp -s "$actual" "$expected"; then

        full_passes=$((full_passes + 1))
    else
        failed=$((failed + 1))
    fi

    {
        printf '%s\n\n' "$case_name"
        printf 'actual:\n'
        if [ -f "$actual" ]; then
            cat "$actual"
        else
            printf '(actual output missing)\n'
        fi
        printf '\n\nexpected:\n'
        if [ -f "$expected" ]; then
            cat "$expected"
        else
            printf '(expected output missing)\n'
        fi
        printf '\n\n%s\n\n' '--------------------------------------------------'
    } >> "$RESULTS_FILE"
    count=$((count + 1))
done

# The temporary test data and transaction output aren't needed after reporting.
rm -rf "$WORK_DIR"
echo ""
echo "Test Summary"
echo "Total executed: $count"
echo "Full passes:    $full_passes"
echo "Failed:         $failed"
echo ""
echo "Results saved to: $RESULTS_FILE"
