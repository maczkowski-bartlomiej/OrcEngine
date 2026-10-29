#!/usr/bin/env bash
# scripts/verify.sh: Unified Verification Gate for OrcEngine
# Executes convention linting, build runner, and demo smoke test in strict sequence.

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

CHECK_ALL=0
FIX=0
TIMEOUT=5
PRESET="linux-debug"

while [[ $# -gt 0 ]]; do
	case "$1" in
		--all|--full)
			CHECK_ALL=1
			shift
			;;
		--fix)
			FIX=1
			shift
			;;
		--preset)
			PRESET="$2"
			shift 2
			;;
		--timeout)
			TIMEOUT="$2"
			shift 2
			;;
		*)
			printf "Unknown option: %s\n" "$1"
			printf "Usage: ./scripts/verify.sh [--all|--full] [--fix] [--preset <preset>] [--timeout <sec>]\n"
			exit 1
			;;
	esac
done

printf "\n\033[1;36m====================================================\033[0m\n"
printf "\033[1;36m       OrcEngine Unified Verification Gate          \033[0m\n"
printf "\033[1;36m====================================================\033[0m\n\n"

# Stage 1: Conventions Lint
printf "\033[1;33m[Stage 1/3] Running Conventions Linter...\033[0m\n"
LINT_ARGS=()
if [ "$CHECK_ALL" -eq 0 ]; then
	LINT_ARGS+=("--changed-only")
fi
if [ "$FIX" -eq 1 ]; then
	LINT_ARGS+=("--fix")
fi

if ! "$SCRIPT_DIR/lint-conventions.sh" "${LINT_ARGS[@]}"; then
	printf "\n\033[1;31m[FAILED] Stage 1 (Conventions Lint) failed.\033[0m\n"
	exit 1
fi
printf "\033[1;32m[PASSED] Stage 1: Conventions check clean.\033[0m\n\n"

# Stage 2: Compilation & Linkage
printf "\033[1;33m[Stage 2/3] Building Engine & TestProject (Preset: %s)...\033[0m\n" "$PRESET"
if ! "$SCRIPT_DIR/build.sh" --preset "$PRESET"; then
	printf "\n\033[1;31m[FAILED] Stage 2 (Build) failed.\033[0m\n"
	exit 2
fi
printf "\033[1;32m[PASSED] Stage 2: Build succeeded with zero errors.\033[0m\n\n"

# Stage 3: Runtime Demo Smoke Test
printf "\033[1;33m[Stage 3/3] Running Demo Smoke Test (Timeout: %ss)...\033[0m\n" "$TIMEOUT"
if ! "$SCRIPT_DIR/run-demo.sh" --preset "$PRESET" --timeout "$TIMEOUT" --fail-on-error-log; then
	printf "\n\033[1;31m[FAILED] Stage 3 (Demo Smoke Test) failed.\033[0m\n"
	exit 3
fi
printf "\033[1;32m[PASSED] Stage 3: Demo app executed and exited cleanly.\033[0m\n\n"

printf "\033[1;32m====================================================\033[0m\n"
printf "\033[1;32m       ALL VERIFICATION GATES PASSED (100%%)         \033[0m\n"
printf "\033[1;32m====================================================\033[0m\n\n"
