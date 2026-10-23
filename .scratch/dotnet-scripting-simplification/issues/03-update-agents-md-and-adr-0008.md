# 03: Update AGENTS.md and ADR 0008 for Direct Scripting Architecture

Status: ready-for-agent
Type: task
Blocked by: 02

**What to build:** Synchronize domain documentation and architectural decision records: update Section 4.8 of `AGENTS.md` and ADR `0008-hostfxr-dotnet-coreclr-scripting.md` to reflect the direct class layout, deleted copy and move operations (`= delete`), anonymous-namespace function pointers, top-of-file platform macro localization, and `ORC_FATAL_CHECK` with `std::abort()` in `Debug.hpp` rather than PIMPL and template helpers. Run the unified verification gate `verify.ps1`.

**Blocked by:** 02 (needs completed implementation of simplified Scripting)

- [ ] `AGENTS.md` Section 4.8 updated to describe direct class layout, deleted copy/move operations, top-of-file platform macros, and `ORC_FATAL_CHECK` without PIMPL
- [ ] `docs/adr/0008-hostfxr-dotnet-coreclr-scripting.md` updated to document the simplification decisions and trade-offs
- [ ] Unified verification gate `.\scripts\verify.ps1` passes with 100% success across linting, compilation, and smoke test
