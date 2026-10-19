#!/usr/bin/env bash
# scripts/lint-conventions.sh: OrcEngine C++ Conventions Linter for Linux/macOS

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

CHANGED_ONLY=0
FIX=0
TARGET_PATHS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --changed-only|-ChangedOnly)
            CHANGED_ONLY=1
            shift
            ;;
        --fix|-Fix)
            FIX=1
            shift
            ;;
        *)
            TARGET_PATHS+=("$1")
            shift
            ;;
    esac
done

if [ ${#TARGET_PATHS[@]} -eq 0 ]; then
    TARGET_PATHS=("OrcEngine" "TestProject")
fi

FILES=()

if [ "$CHANGED_ONLY" -eq 1 ]; then
    if command -v git >/dev/null 2>&1; then
        while IFS= read -r f; do
            if [[ "$f" =~ \.(hpp|cpp|inl)$ ]] && [ -f "$f" ]; then
                FILES+=("$f")
            fi
        done < <(git status --porcelain -uall | awk '{print $2}')
    fi
    if [ ${#FILES[@]} -eq 0 ]; then
        printf "\033[32mNo changed C++ files found in git working tree.\033[0m\n"
        exit 0
    fi
else
    for p in "${TARGET_PATHS[@]}"; do
        if [ -d "$p" ]; then
            while IFS= read -r f; do
                FILES+=("$f")
            done < <(find "$p" -type f \( -name "*.hpp" -o -name "*.cpp" -o -name "*.inl" \))
        elif [ -f "$p" ]; then
            FILES+=("$p")
        fi
    done
fi

printf "\n\033[36m=== OrcEngine C++ Conventions Linter ===\033[0m\n"
printf "\033[90mChecking %d file(s)...\033[0m\n\n" "${#FILES[@]}"

TOTAL_ERRORS=0
TOTAL_WARNINGS=0

for f in "${FILES[@]}"; do
    FILE_ERRORS=0
    FILE_WARNINGS=0
    EXT="${f##*.}"

    # 1. Check #pragma once in headers
    if [ "$EXT" = "hpp" ] || [ "$EXT" = "inl" ]; then
        if ! head -n 10 "$f" | grep -qE '^[[:space:]]*#pragma[[:space:]]+once\b'; then
            if [ "$FIX" -eq 1 ]; then
                printf "#pragma once\n\n%s" "$(cat "$f")" > "$f"
                printf "%s\n  \033[32mLine 1 [FIXED]: Added missing '#pragma once'\033[0m\n" "$f"
            else
                printf "%s\n  \033[31mLine 1 [ERROR]: Missing '#pragma once' near top of header\033[0m\n" "$f"
                FILE_ERRORS=$((FILE_ERRORS + 1))
            fi
        fi
    fi

    # 2. Check namespace orc in OrcEngine/
    if [[ "$f" =~ ^OrcEngine/ ]] && [[ "$f" != *"OrcPch.hpp" ]] && [[ "$f" != *"EntryPoint.cpp" ]] && [[ "$f" != *"StbImage.cpp" ]] && [[ "$f" != *"Debug.hpp" ]] && [[ "$f" != *"Orc.hpp" ]]; then
        if ! grep -qE '\bnamespace[[:space:]]+orc\b' "$f"; then
            printf "%s\n  \033[31mLine 1 [ERROR]: Engine code must be enclosed in 'namespace orc'\033[0m\n" "$f"
            FILE_ERRORS=$((FILE_ERRORS + 1))
        fi
    fi

    # 3. Check tab indentation (flag 4+ leading spaces)
    SPACE_LINES=$(grep -nE '^[ ]{4,}[^ *]' "$f" | head -n 1 || true)
    if [ -n "$SPACE_LINES" ]; then
        LINE_NUM="${SPACE_LINES%%:*}"
        printf "%s\n  \033[33mLine %s [WARN]: Space-based indentation detected. OrcEngine standard is tab indentation.\033[0m\n" "$f" "$LINE_NUM"
        FILE_WARNINGS=$((FILE_WARNINGS + 1))
    fi

    TOTAL_ERRORS=$((TOTAL_ERRORS + FILE_ERRORS))
    TOTAL_WARNINGS=$((TOTAL_WARNINGS + FILE_WARNINGS))
done

printf "\n"
if [ "$TOTAL_ERRORS" -eq 0 ] && [ "$TOTAL_WARNINGS" -eq 0 ]; then
    printf "\033[32mAll files passed conventions check!\033[0m\n"
    exit 0
elif [ "$TOTAL_ERRORS" -eq 0 ]; then
    printf "\033[33mPassed with %d warning(s).\033[0m\n" "$TOTAL_WARNINGS"
    exit 0
else
    printf "\033[31mFailed with %d error(s) and %d warning(s).\033[0m\n" "$TOTAL_ERRORS" "$TOTAL_WARNINGS"
    exit 1
fi
