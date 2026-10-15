#!/usr/bin/env bash
# scripts/check-env.sh: OrcEngine Agent Environment Diagnostic for Linux/macOS/WSL

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

ALL_OK=0
WARNINGS=0

pass() {
    printf "\033[32m  [ PASS ] \033[0m\033[1m%s: \033[0m\033[37m%s\033[0m\n" "$1" "$2"
}

warn() {
    printf "\033[33m  [ WARN ] \033[0m\033[1m%s: \033[0m\033[33m%s\033[0m\n" "$1" "$2"
    if [ -n "$3" ]; then
        printf "           \033[33mRemediation: %s\033[0m\n" "$3"
    fi
    WARNINGS=$((WARNINGS + 1))
}

fail() {
    printf "\033[31m  [ FAIL ] \033[0m\033[1m%s: \033[0m\033[31m%s\033[0m\n" "$1" "$2"
    if [ -n "$3" ]; then
        printf "           \033[31mRemediation: %s\033[0m\n" "$3"
    fi
    ALL_OK=1
}

printf "\n\033[36m=== OrcEngine Agent Environment Diagnostic ===\033[0m\n"
printf "\033[90mRepository root: %s\033[0m\n\n" "$REPO_ROOT"

# 1. CMake Check
if command -v cmake >/dev/null 2>&1; then
    CMAKE_VER_STR=$(cmake --version | head -n1)
    CMAKE_VER=$(echo "$CMAKE_VER_STR" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
    pass "CMake" "$CMAKE_VER_STR"
else
    fail "CMake" "cmake not found in PATH." "Install CMake 3.25+ (e.g. sudo apt install cmake)."
fi

# 2. Ninja Check
if command -v ninja >/dev/null 2>&1; then
    NINJA_VER=$(ninja --version 2>&1 | head -n1)
    pass "Ninja" "Version $NINJA_VER ($(command -v ninja))"
else
    fail "Ninja" "ninja not found in PATH." "Install Ninja (e.g. sudo apt install ninja-build)."
fi

# 3. Compiler Check
if command -v g++ >/dev/null 2>&1; then
    GXX_VER=$(g++ --version | head -n1)
    pass "Compiler" "GCC detected: $GXX_VER"
elif command -v clang++ >/dev/null 2>&1; then
    CLANG_VER=$(clang++ --version | head -n1)
    pass "Compiler" "Clang detected: $CLANG_VER"
else
    fail "Compiler" "No C++20 compiler (g++ / clang++) found." "Install g++ 11+ or clang++ 13+."
fi

# 4. VCPKG_ROOT Check
if [ -z "$VCPKG_ROOT" ]; then
    if [ -d "$HOME/vcpkg" ] && [ -f "$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
        export VCPKG_ROOT="$HOME/vcpkg"
        pass "VCPKG_ROOT" "$VCPKG_ROOT (auto-detected, toolchain verified)"
    else
        fail "VCPKG_ROOT" "Environment variable VCPKG_ROOT is not set." "Export VCPKG_ROOT pointing to your vcpkg checkout or install vcpkg at \$HOME/vcpkg."
    fi
else
    if [ -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
        pass "VCPKG_ROOT" "$VCPKG_ROOT (toolchain verified)"
    else
        fail "VCPKG_ROOT" "VCPKG_ROOT is set to '$VCPKG_ROOT', but scripts/buildsystems/vcpkg.cmake is missing." "Verify vcpkg directory."
    fi
fi

# 5. FMOD_ROOT Check
if [ -z "$FMOD_ROOT" ]; then
    for candidate in "$HOME/fmod" "$HOME/dev/libs/cpp/fmod" "/opt/fmod" "/usr/local/fmod" "/usr/local/opt/fmod"; do
        if [ -d "$candidate/api/core/inc" ]; then
            export FMOD_ROOT="$candidate"
            break
        fi
    done
fi

if [ -n "$FMOD_ROOT" ]; then
    if [ -f "$FMOD_ROOT/api/core/inc/fmod.hpp" ] && [ -f "$FMOD_ROOT/api/studio/inc/fmod_studio.hpp" ]; then
        pass "FMOD_ROOT" "$FMOD_ROOT (Core & Studio headers verified)"
    else
        warn "FMOD_ROOT" "FMOD_ROOT is set to '$FMOD_ROOT', but api headers are missing." "Check README.md FMOD SDK layout."
    fi
else
    fail "FMOD_ROOT" "Environment variable FMOD_ROOT is not set." "Export FMOD_ROOT pointing to your FMOD installation or install to /opt/fmod or \$HOME/fmod."
fi

# 6. Git Status Check
if command -v git >/dev/null 2>&1; then
    BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
    MODIFIED_COUNT=$(git status --porcelain 2>/dev/null | wc -l | tr -d ' ')
    pass "Git" "Branch '$BRANCH', $MODIFIED_COUNT modified/untracked files"
else
    warn "Git" "git not found in PATH." "Install git."
fi

# 7. CMakePresets.json Check
if [ -f "CMakePresets.json" ]; then
    pass "Presets" "CMakePresets.json is present"
else
    fail "Presets" "CMakePresets.json not found in repository root." "Ensure you are in the repository root."
fi

printf "\n"
if [ "$ALL_OK" -eq 0 ]; then
    if [ "$WARNINGS" -gt 0 ]; then
        printf "\033[33mDiagnostic status: OK WITH WARNINGS (%d warning(s)).\033[0m\n" "$WARNINGS"
    else
        printf "\033[32mDiagnostic status: ALL CHECKS PASSED. Environment is fully ready for building.\033[0m\n"
    fi
    exit 0
else
    printf "\033[31mDiagnostic status: FAILED. Please resolve the critical items above before building.\033[0m\n"
    exit 1
fi
