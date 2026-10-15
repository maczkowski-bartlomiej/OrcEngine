#!/usr/bin/env bash
# scripts/run-demo.sh: OrcEngine Demo Runner for Linux/macOS

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

PRESET="linux-debug"
TIMEOUT_SEC=0
FAIL_ON_ERROR_LOG=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --preset|-Preset)
            PRESET="$2"
            shift 2
            ;;
        --timeout|-TimeoutSeconds)
            TIMEOUT_SEC="$2"
            shift 2
            ;;
        --fail-on-error|-FailOnErrorLog)
            FAIL_ON_ERROR_LOG=1
            shift
            ;;
        *)
            shift
            ;;
    esac
done

BUILD_DIR="$REPO_ROOT/build/$PRESET"
EXE="$BUILD_DIR/TestProject"

printf "\n\033[36m=== OrcEngine Demo Runner ===\033[0m\n"
printf "\033[90mWorking Directory: %s\033[0m\n\n" "$BUILD_DIR"

if [ ! -f "$EXE" ]; then
    printf "\033[31mExecutable 'TestProject' not found in %s\033[0m\n" "$BUILD_DIR"
    printf "\033[33mPlease build first using: ./scripts/build.sh --preset %s\033[0m\n" "$PRESET"
    exit 1
fi

LOG_FILE="$BUILD_DIR/logs/TestProject.log"
[ -f "$LOG_FILE" ] || LOG_FILE="$REPO_ROOT/logs/TestProject.log"

printf "\033[32mLaunching TestProject...\033[0m\n"
(
    cd "$BUILD_DIR"
    if [ "$TIMEOUT_SEC" -gt 0 ]; then
        printf "\033[90mRunning with %d second(s) timeout...\033[0m\n" "$TIMEOUT_SEC"
        timeout "$TIMEOUT_SEC" ./TestProject || true
    else
        ./TestProject
    fi
)

HAS_LOG_ERRORS=0
if [ -f "$LOG_FILE" ]; then
    printf "\n\033[36m=== TestProject Log Output ===\033[0m\n"
    tail -n 20 "$LOG_FILE" | while IFS= read -r line; do
        if echo "$line" | grep -iqE '\[(error|critical)\]|fatal|crash|assert'; then
            printf "\033[31m  %s\033[0m\n" "$line"
        else
            printf "\033[90m  %s\033[0m\n" "$line"
        fi
    done

    if tail -n 50 "$LOG_FILE" | grep -iqE '\[(error|critical)\]|fatal|crash|assert'; then
        HAS_LOG_ERRORS=1
    fi
fi

if [ "$FAIL_ON_ERROR_LOG" -eq 1 ] && [ "$HAS_LOG_ERRORS" -eq 1 ]; then
    printf "\n\033[31mErrors found in TestProject.log!\033[0m\n"
    exit 1
fi

printf "\n\033[32mDemo run completed successfully.\033[0m\n"
exit 0
