#!/usr/bin/env bash
# scripts/setup-git-hooks.sh: Install OrcEngine Git Pre-Commit Hook

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
HOOKS_DIR="$REPO_ROOT/.git/hooks"

if [ ! -d "$HOOKS_DIR" ]; then
	printf "\033[1;31mError: .git/hooks directory not found at '%s'.\033[0m\n" "$HOOKS_DIR"
	exit 1
fi

PRE_COMMIT_FILE="$HOOKS_DIR/pre-commit"

cat << 'EOF' > "$PRE_COMMIT_FILE"
#!/usr/bin/env bash
# OrcEngine Pre-Commit Hook: Validates conventions on changed files

set -eo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
LINT_SCRIPT="$ROOT_DIR/scripts/lint-conventions.sh"

if [ -f "$LINT_SCRIPT" ]; then
	printf "\n\033[1;36m[Pre-Commit] Running OrcEngine conventions check on changed files...\033[0m\n"
	if ! "$LINT_SCRIPT" --changed-only; then
		printf "\n\033[1;31m[Pre-Commit Error] Conventions check failed. Fix violations or run with --fix before committing.\033[0m\n\n"
		exit 1
	fi
fi
EOF

chmod +x "$PRE_COMMIT_FILE"

printf "\033[1;32mSuccessfully installed OrcEngine pre-commit hook to '%s'!\033[0m\n" "$PRE_COMMIT_FILE"
