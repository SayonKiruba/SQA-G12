
#!/usr/bin/env bash
# ============================================================
# SQA Group 12 - Automated Test Results
#
# Compares actual test outputs with expected Phase01 results.
# Saves the comparison results and a pass/fail status for each test.
#
# Usage (from project root): bash Phase03/check_outputs.sh
# ============================================================

set -euo pipefail

# Define the directories used for test results and comparisons.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$SCRIPT_DIR/../Phase01/tests"
WORK_DIR="$SCRIPT_DIR/.test-work"
RESULTS_DIR="$SCRIPT_DIR/test-results"

# Check that the test outputs exist before comparing results.
if [ ! -d "$WORK_DIR" ]; then
    echo "No test outputs found. First run: bash Phase03/run_tests.sh" >&2
    exit 1
fi

mkdir -p "$RESULTS_DIR"

# Create a new numbered folder for each test run.
run_number=1

while true; do
    RUN_DIR="$RESULTS_DIR/$(printf 'run-%03d' "$run_number")"

    if mkdir "$RUN_DIR" 2>/dev/null; then
        break
    fi

    run_number=$((run_number + 1))
done

# Create a result file to store all test comparisons.
RESULTS_FILE="$RUN_DIR/result.txt"
: > "$RESULTS_FILE"

count=0
full_passes=0
failed=0

# Remove extra whitespace and line-ending differences before comparing.
normalize_output() {
    sed 's/\r$//' "$1" | awk 'NF { $1=$1; print }'
}

# Go through each Phase01 test and locate its actual and expected outputs.
for input_file in "$TESTS_DIR"/*/*/input.txt; do
    [ -f "$input_file" ] || continue

    case_dir="$(dirname "$input_file")"
    case_name="$(basename "$case_dir")"
    relative_case="${case_dir#"$TESTS_DIR"/}"
    actual="$WORK_DIR/$relative_case/actual_output.txt"
    expected="$case_dir/output.txt"
    exit_status_file="$WORK_DIR/$relative_case/exit_code.txt"

    # Compare the outputs and record whether the test passed or failed.
    if [ -f "$actual" ] && [ -f "$expected" ] && \
       [ -f "$exit_status_file" ] && \
       [ "$(cat "$exit_status_file")" = "0" ] && \
       diff -q <(normalize_output "$actual") \
               <(normalize_output "$expected") > /dev/null; then

        full_passes=$((full_passes + 1))
        status="PASS"
    else
        failed=$((failed + 1))
        status="FAIL"
    fi

    # Write each test's status and outputs to the result file.
    {
        printf '%s\n' "$case_name"
        printf 'Status: %s\n\n' "$status"

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

        # Include a breakdown of the differences for failed tests.
        if [ "$status" = "FAIL" ]; then
            printf '\n\nDifferences:\n'

            if [ -f "$actual" ] && [ -f "$expected" ]; then
                diff -u \
                    <(normalize_output "$expected") \
                    <(normalize_output "$actual") || true
            else
                printf 'One or more output files are missing.\n'
            fi

            # Report execution errors if the program did not exit successfully.
            if [ -f "$exit_status_file" ]; then
                exit_code="$(cat "$exit_status_file")"

                if [ "$exit_code" != "0" ]; then
                    printf '\nExecution error (exit code: %s)\n' "$exit_code"
                fi
            else
                printf '\nExit code file missing.\n'
            fi
        fi

        printf '\n\n%s\n\n' '--------------------------------------------------'

    } >> "$RESULTS_FILE"

    count=$((count + 1))
done

# Remove temporary test files after saving the results.
rm -rf "$WORK_DIR"

# Display the final test summary and the location of the result file.
echo ""
echo "Test Summary"
echo "Total executed: $count"
echo "Full passes:    $full_passes"
echo "Failed:         $failed"
echo ""
echo "Results saved to: $RESULTS_FILE"
