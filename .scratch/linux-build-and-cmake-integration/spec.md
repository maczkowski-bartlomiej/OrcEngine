# Spec: Linux Build and CMake Windows Integration

Status: ready-for-agent
Triage: ready-for-agent

## Problem Statement

Developers and automated agents building OrcEngine across various Linux distributions (such as Ubuntu, Debian, Fedora, Arch) encounter immediate compilation, scripting, and configuration failures:

1. **Variadic Macro Preprocessor Error**: The logging and assertion macros (`ORC_LOG_INFO`, `ORC_LOG_WARNING`, `ORC_LOG_ERROR`, `ORC_LOG_FATAL`, `ORC_ASSERT`, `ORC_WARNING`, `ORC_ERROR`, `ORC_FATAL`) fail to compile under GCC and Clang when called without optional variadic format arguments (e.g. `ORC_LOG_INFO("Loading fonts.");`). Under the legacy preprocessor syntax `message, __VA_ARGS__`, GCC and Clang preserve the trailing comma before empty arguments, yielding `error: expected primary-expression before ')' token`. On Windows, the legacy MSVC preprocessor masked this bug by automatically suppressing the comma.
2. **Missing Script Executable Permissions**: POSIX automation scripts (`scripts/*.sh`) are checked into git with non-executable permissions (`100644`), leading to `Permission denied` errors when developers or agents invoke them directly on Linux distributions.
3. **Rigid Dependency & SDK Discovery**: CMake build presets and FMOD discovery fail if environment variables are not pre-configured manually in the shell. While Windows workflows often rely on global environment settings, Linux developers frequently place tools and SDKs in standard locations (such as `$HOME/vcpkg`, `/opt/fmod`, or `$HOME/fmod`) without exporting explicit environment variables in every shell session.
4. **Conventions Linter Divergence**: Core debug macro headers do not conform to codebase standards (tab indentation and namespace encapsulation), triggering convention lint errors on strict build pipelines.

## Solution

Provide a frictionless, robust, and standards-compliant cross-platform CMake build and developer experience across Linux distributions and Windows:

1. **Portable C++20 Variadic Macros**: Standardize all logging and assertion preprocessor macros on the ISO C++20 `__VA_OPT__(,) __VA_ARGS__` construct, eliminating trailing commas across all conforming compilers. Pair this with the `/Zc:preprocessor` compiler flag for MSVC on Windows to ensure strict preprocessor standards compliance across GCC, Clang, and MSVC.
2. **Executable Automation Scripts**: Update repository filemode metadata so that all POSIX shell scripts under `scripts/` possess the executable bit (`100755`), allowing immediate execution on Linux distributions.
3. **Resilient vcpkg Auto-Discovery**: Retain vcpkg manifest mode as the authoritative single source of truth for third-party libraries across both platforms, while equipping Linux environment and build scripts with fallback auto-detection of `$HOME/vcpkg` and actionable diagnostics when unset.
4. **Multi-Tier FMOD Discovery**: Broaden CMake FMOD discovery to automatically inspect common Linux and Windows system and user directories (e.g. `/opt/fmod`, `/usr/local/fmod`, `$HOME/fmod`, `$HOME/dev/libs/cpp/fmod`, `C:/Program Files/...`) in addition to explicit environment variables.
5. **Code Standards Alignment**: Ensure all macro headers adhere to tab indentation and namespace conventions, achieving zero errors across convention linter runs.

## User Stories

1. As a Linux game engine developer, I want to call logging macros with plain string literals without format arguments, so that my code compiles cleanly under GCC and Clang without preprocessor syntax errors.
2. As a Windows developer using Visual Studio or MSVC, I want the codebase to use standard-compliant preprocessor flags, so that macro expansion behaves identically to GCC and Clang without hidden platform divergence.
3. As a developer cloning the engine on a Linux distribution, I want to run `./scripts/build.sh` and `./scripts/check-env.sh` directly from my terminal, so that I am not blocked by `Permission denied` filemode errors.
4. As an audio programmer, I want CMake to discover the FMOD Studio and Core SDK from standard installation paths on Linux and Windows, so that I do not have to manually define `FMOD_ROOT` across every new shell or terminal.
5. As an engine maintainer, I want third-party libraries managed via a single unified manifest across both Linux and Windows, so that version mismatches, binary incompatibilities, and distro-specific packaging variances are completely avoided.
6. As a Linux developer who installed vcpkg in `$HOME/vcpkg`, I want build and diagnostic scripts to automatically locate vcpkg, so that I can configure and build immediately without configuring shell environment variables.
7. As an automated agent or CI runner, I want `./scripts/check-env.sh` to output clear pass/fail diagnostics for compilers, CMake, Ninja, vcpkg, and FMOD, so that missing prerequisites are identified in seconds.
8. As a developer modifying engine assertion macros, I want assertion failures to cleanly log with source locations and break into the debugger on both Linux (via `SIGTRAP`) and Windows (via `__debugbreak()`), so that invariant bugs can be caught instantly during development.
9. As a contributor running `./scripts/lint-conventions.sh`, I want all engine headers to comply with tab-indentation and namespace rules, so that convention checks pass cleanly without false-positive failures on macro headers.
10. As a gameplay programmer creating a demo layer, I want `build.sh --run` to compile the engine, deploy runtime dynamic libraries, and launch the demo executable with a timeout, so that I can smoke-test my changes in a single command.
11. As a systems programmer, I want CMake target definitions to export correct platform macros (`ORC_PLATFORM_LINUX` and `ORC_PLATFORM_WINDOWS`), so that engine runtime code correctly branches on platform-specific behavior.
12. As a cross-platform developer switching between Windows and Linux/WSL, I want CMake presets to have consistent naming and behavior (`linux-debug`, `linux-release`, `windows-debug`, `windows-release`), so that my build workflow is uniform across machines.

## Implementation Decisions

- **C++20 Preprocessor Standardization**: All variadic macros for logging and debugging will adopt the ISO C++20 `__VA_OPT__(,) __VA_ARGS__` expansion. This replaces legacy non-standard comma suppression patterns while guaranteeing strict compliance with the ISO C++20 standard.
- **MSVC Conforming Preprocessor Activation**: The engine build configuration will enable MSVC's modern conforming preprocessor flag (`/Zc:preprocessor`) for all targets when compiled with MSVC. This ensures that MSVC accurately evaluates `__VA_OPT__` and adheres to standard preprocessor semantics.
- **Executable Bit in Git Index**: All POSIX script files in the automation directory will be recorded with git filemode `100755`. This ensures that freshly cloned repositories on Linux and macOS have directly runnable scripts without requiring manual `chmod` commands.
- **Unified Package Management via Manifest Mode**: Third-party C++ libraries will remain strictly governed by the root manifest file. On Linux distributions, scripts will inspect `$HOME/vcpkg` if the primary environment variable is unset, auto-exporting it for the active session and providing clear setup instructions if missing.
- **Multi-Location SDK Discovery Hints**: The custom CMake SDK discovery module will augment its search hints with canonical user and system directory paths for both Linux (including standard user directories and system opt directories) and Windows (standard program file locations and user directories), while preserving explicit user overrides as top priority.
- **Conventions & Namespace Alignment**: All engine headers, including macro-only definition files, will be brought into compliance with repository standards by converting space indentation to tabs and enclosing declarations within the canonical engine namespace.

## Testing Decisions

- **External Behavior Verification at Highest Seam**: Testing will occur at the highest available seam: the automated end-to-end build runner script (`build.sh --run --timeout 5` on Linux, and `build.ps1 -Run -TimeoutSeconds 5` on Windows).
- **Smoke Harness Execution**: The smoke harness runs the demo executable, which exercises the full initialization cascade: logging subsystem, window creation, OpenGL context loading, unified dynamic renderer initialization, FMOD Studio and bus setup, asset manifest deserialization, input polling, layer lifecycle hooks, and safe shutdown.
- **Conventions & Linter Validation**: Testing will enforce zero errors on the repository conventions linter (`lint-conventions.sh --changed-only` and full scan).
- **No Intrusive Unit Mocks**: No mock preprocessors or synthetic test wrappers will be introduced; testing relies purely on real compiler invocations and executable smoke runs.

## Out of Scope

- Replacing CMake with alternate build systems (e.g. Meson, Premake, or Makefiles).
- Replacing vcpkg manifest mode with native Linux distribution package managers (e.g. APT, DNF, Pacman) or unmanaged git submodules.
- Altering the rendering pipeline, shader compilation, or audio playback runtime architecture.
- Distributing proprietary third-party binary SDKs directly within the repository git history.

## Further Notes

- The changes maintain complete backwards compatibility with existing Windows development workflows while enabling full parity for Linux distributions.
- All modifications strictly comply with the architectural rules and conventions defined in `AGENTS.md`.
