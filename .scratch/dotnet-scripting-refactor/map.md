# Map: .NET 10 CoreCLR Scripting Subsystem Refactoring & Hardening

## Overview
Refactor the newly added .NET 10 CoreCLR / hostfxr scripting integration to enforce an unconditional fail-fast crash policy, implement PIMPL encapsulation, eliminate preprocessor macros in favor of typed C++ helpers, clean up engine nomenclature, and synchronize domain documentation.

## Decisions So Far
- [04-sync-documentation-glossary-and-adr](issues/04-sync-documentation-glossary-and-adr.md): Synchronized GLOSSARY.md with canonical Scripting definition and deprecated terms, added Section 4.8 to AGENTS.md documenting Scripting subsystem lifecycle and hosting model, and recorded ADR 0008.
- [03-clean-up-engine-nomenclature](issues/03-clean-up-engine-nomenclature.md): Renamed m_mono to m_scripting in Engine.hpp and Engine.cpp, ensuring consistent canonical Scripting nomenclature across engine lifecycle coordination.
- [02-implement-fail-fast-crash-and-helpers](issues/02-implement-fail-fast-crash-and-helpers.md): Implemented Scripting::Impl with fail-fast crash policy, replaced preprocessor macros with static C++ helpers (loadLibrary, getExport, freeLibrary, toPalString, getRuntimeDelegate, getManagedFunction), encapsulated hostfxr function pointers, and verified Class1.Hello() startup execution.
- [01-refactor-scripting-header-and-pimpl](issues/01-refactor-scripting-header-and-pimpl.md): Refactored Scripting.hpp to use PIMPL (UniquePtr<Impl> m_impl), Rule of 5 semantics with deleted copy operations and defaulted move operations, const FilePath& configPath, and encapsulated native handles within Scripting.cpp.
- Adopted PIMPL pattern for `Scripting` to hide native dynamic library handles and hostfxr function pointers from public engine headers.
- Standardized file path parameters to `const FilePath&` (from `Core/Types.hpp`).
- Enforced unconditional crash (`ORC_LOG_FATAL` + `ORC_DEBUGBREAK` + `std::abort()`) on any hosting or binding failure.
- Preserved `OrcEngine.Scripting.Class1.Hello()` invocation on startup as validation baseline.
- Renamed `m_mono` to `m_scripting` in `Engine`.
- Documented Scripting in `GLOSSARY.md`, `AGENTS.md`, and ADR 0008.

## Tickets
- [01: Refactor Scripting Header and PIMPL Interface](issues/01-refactor-scripting-header-and-pimpl.md) (Status: ready-for-agent)
- [02: Implement Fail-Fast Crash Policy and Helper Functions in Scripting.cpp](issues/02-implement-fail-fast-crash-and-helpers.md) (Status: ready-for-agent, Blocked by: 01)
- [03: Clean Up Engine Nomenclature](issues/03-clean-up-engine-nomenclature.md) (Status: ready-for-agent, Blocked by: 02)
- [04: Sync Documentation, Glossary, and Record ADR 0008](issues/04-sync-documentation-glossary-and-adr.md) (Status: ready-for-agent, Blocked by: 03)
