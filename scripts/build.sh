#!/usr/bin/env bash
# scripts/build.sh: OrcEngine Build and Test Runner for Linux/macOS

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

PRESET="linux-debug"
TARGET=""
CLEAN=0
CONFIGURE_ONLY=0
BUILD_ONLY=0
RUN=0
TIMEOUT_SEC=0
EXTRA_CMAKE_ARGS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --preset|-Preset)
            PRESET="$2"
            shift 2
            ;;
        --target|-Target)
            TARGET="$2"
            shift 2
            ;;
        --clean|-Clean)
            CLEAN=1
            shift
            ;;
        --configure-only|-ConfigureOnly)
            CONFIGURE_ONLY=1
            shift
            ;;
        --build-only|-BuildOnly)
            BUILD_ONLY=1
            shift
            ;;
        --run|-Run)
            RUN=1
            shift
            ;;
        --timeout|-TimeoutSeconds)
            TIMEOUT_SEC="$2"
            shift 2
            ;;
        *)
            EXTRA_CMAKE_ARGS+=("$1")
            shift
            ;;
    esac
done

printf "\n\033[36m=== OrcEngine Build Runner ===\033[0m\n"
printf "Preset: \033[1m%s\033[0m\n\n" "$PRESET"

# Check Prerequisites
if [ -z "$VCPKG_ROOT" ]; then
    if [ -d "$HOME/vcpkg" ] && [ -f "$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
        export VCPKG_ROOT="$HOME/vcpkg"
        printf "\033[32mAuto-detected VCPKG_ROOT at: %s\033[0m\n" "$VCPKG_ROOT"
    else
        printf "\033[33mWarning: VCPKG_ROOT is not set. Configure may fail. Export VCPKG_ROOT or install vcpkg at \$HOME/vcpkg.\033[0m\n"
    fi
fi
if [ -z "$FMOD_ROOT" ]; then
    printf "\033[90mNote: FMOD_ROOT is not set. CMake will use multi-tier SDK discovery.\033[0m\n"
fi

BUILD_DIR="$REPO_ROOT/build/$PRESET"

if [ "$CLEAN" -eq 1 ] && [ -d "$BUILD_DIR" ]; then
    printf "\033[33mCleaning build directory: %s\033[0m\n" "$BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

NEEDS_CONFIG=0
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ] || [ "$CLEAN" -eq 1 ]; then
    NEEDS_CONFIG=1
fi

if [ "$BUILD_ONLY" -eq 0 ] && [ "$NEEDS_CONFIG" -eq 1 ]; then
    printf "\033[36m>> Configuring preset '%s'...\033[0m\n" "$PRESET"
    cmake --preset "$PRESET" "${EXTRA_CMAKE_ARGS[@]}"
fi

if [ "$CONFIGURE_ONLY" -eq 1 ]; then
    printf "\n\033[32mConfigure completed successfully.\033[0m\n"
    exit 0
fi

printf "\n\033[36m>> Building preset '%s'...\033[0m\n" "$PRESET"
BUILD_CMD=(cmake --build --preset "$PRESET")
if [ -n "$TARGET" ]; then
    BUILD_CMD+=(--target "$TARGET")
fi

"${BUILD_CMD[@]}"

printf "\n\033[32mBuild succeeded!\033[0m\n"

if [ "$RUN" -eq 1 ]; then
    EXE="$BUILD_DIR/TestProject"
    if [ ! -f "$EXE" ]; then
        printf "\033[31mError: Executable not found at '%s'\033[0m\n" "$EXE"
        exit 1
    fi

    printf "\n\033[36m>> Launching TestProject from %s...\033[0m\n" "$BUILD_DIR"
    (
        cd "$BUILD_DIR"
        if [ "$TIMEOUT_SEC" -gt 0 ]; then
            timeout "$TIMEOUT_SEC" ./TestProject || true
        else
            ./TestProject
        fi
    )

    LOG_FILE="$BUILD_DIR/logs/TestProject.log"
    [ -f "$LOG_FILE" ] || LOG_FILE="$REPO_ROOT/logs/TestProject.log"
    if [ -f "$LOG_FILE" ]; then
        printf "\n\033[90mRecent entries from %s:\033[0m\n" "$LOG_FILE"
        tail -n 10 "$LOG_FILE"
    fi
fi
