# 01: Relocate Logger & Debug (Fail-Fast Crash Contract) to Core, Seal spdlog, and Inline FontUtility

Status: resolved
Type: task
Blocked by: none

**What to build:** Relocate the `Logger` and `Debug` (`Fail-Fast Crash Contract`) modules from `Engine` into Layer 0 (`Core`), seal `spdlog` completely behind `Logger`'s implementation seam so public headers never include `<spdlog/...>`, collapse the duplicated `#ifdef ORC_DEBUG_ASSERTS` macro blocks in `Debug`, and fold the single-caller `ORC_FT_CALL` macro directly into the `Font` implementation.

## Acceptance Criteria

- [x] `Logger` and `Debug` are relocated from `Engine` to `Core` and exported through the `Core` and top-level umbrella headers, with the precompiled header and all subsystem callers (`Events`, `Graphics`, `Audio`, `Scripting`, `Engine`) updated to include them from `Core`.
- [x] `Logger::log` delegates through a non-template `Logger::logImpl(Level level, std::source_location sourceLocation, fmt::string_view format, fmt::format_args args)` seam so `<spdlog/logger.h>` and `spdlog::logger` are completely removed from public headers and isolated inside `Logger`'s implementation.
- [x] `spdlog` is linked `PRIVATE` (and `fmt` `PUBLIC`) on `OrcEngine` in CMake, and `Logger`'s header no longer pulls in GLM math headers.
- [x] `Debug` defines `ORC_FATAL` and `ORC_FATAL_CHECK` once without `#ifdef ORC_DEBUG_ASSERTS` / `#else` duplication, relying on `ORC_DEBUGBREAK()` expanding to a no-op when debug asserts are disabled.
- [x] `ORC_FT_CALL` is defined directly inside `Font`'s implementation file, the single-caller `FontUtility` header is deleted, and the `*FontUtility.hpp` exemption is removed from the convention linter scripts.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments

## Answer

Relocated Logger and Debug to Core, sealed spdlog behind Logger::logImpl with spdlog linked PRIVATE, collapsed ORC_FATAL/ORC_FATAL_CHECK duplication, inlined ORC_FT_CALL into Font.cpp, and deleted FontUtility.hpp.
