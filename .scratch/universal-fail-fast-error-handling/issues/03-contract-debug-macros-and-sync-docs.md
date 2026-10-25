# 03: Contract Debug & Logger Macros and Synchronize Architecture Documentation

Status: resolved
Type: task
Blocked by: 01, 02

**What to build:** Remove obsolete assertion and error-logging macros from the engine debug and logging headers, record an Architecture Decision Record for universal fail-fast error handling, and synchronize `CODING_STANDARDS.md`, `AGENTS.md`, and `GLOSSARY.md`:
1. In the debug header, remove `ORC_ASSERT`, `ORC_WARNING`, `ORC_ERROR`, and `ORC_ERROR_CHECK` from both the `#ifdef ORC_DEBUG_ASSERTS` and `#else` blocks, leaving only `ORC_FATAL` and `ORC_FATAL_CHECK`.
2. In the logger header, remove the standalone `ORC_LOG_ERROR` macro (leaving `ORC_LOG_INFO`, `ORC_LOG_WARNING`, and `ORC_LOG_FATAL`).
3. Create `docs/adr/0009-universal-fail-fast-error-handling.md` recording the architectural decision and rationale for universal fail-fast error handling (`ORC_FATAL_CHECK`, `ORC_FATAL`, `ORC_FMOD_CALL`, `ORC_FT_CALL`), removal of `ORC_ASSERT` and boolean/nullable failure returns, and elimination of defensive half-initialized state checks.
4. Update `CODING_STANDARDS.md` (including the Naming Conventions macro examples and Section 5 "Error Handling, Assertions & Logging"), `AGENTS.md` (Subsystem Context Pointers and Coding Standards summary), and `GLOSSARY.md` to reflect the unified fail-fast contract and reference ADR 0009.
5. Verify zero-warning compilation across both Debug (`linux-debug`) and Release (`linux-release`) presets and run the full verification gate (`./scripts/verify.sh`).

**Blocked by:** 01, 02

- [x] `Debug.hpp` defines only `ORC_FATAL` and `ORC_FATAL_CHECK` in both Debug and Release configurations
- [x] `Logger.hpp` removes `ORC_LOG_ERROR`, retaining `ORC_LOG_INFO`, `ORC_LOG_WARNING`, and `ORC_LOG_FATAL`
- [x] `docs/adr/0009-universal-fail-fast-error-handling.md` is created and linked from `AGENTS.md`
- [x] `CODING_STANDARDS.md` and `GLOSSARY.md` are updated to reflect the universal fail-fast error-handling model
- [x] Both `./scripts/build.sh --preset linux-release` and `./scripts/verify.sh` pass 100% with zero warnings or errors

## Answer

Contracted Debug.hpp and Logger.hpp macros, recorded ADR 0009, and synchronized CODING_STANDARDS.md, AGENTS.md, and GLOSSARY.md.
