# Map: .NET 10 Scripting Subsystem Simplification

## Overview
Simplify and streamline the .NET 10 CoreCLR / hostfxr scripting integration by eliminating PIMPL, deleting both copy and move operations, restoring top-of-file platform macros, storing hostfxr function pointers in an anonymous namespace, inlining direct C-API calls without template abstractions, and updating `ORC_FATAL` / `ORC_FATAL_CHECK` in `Debug.hpp` to call `std::abort()`.

## Decisions So Far
- Drop PIMPL: `Scripting` directly holds `FilePath m_configPath` and `void* m_hostfxrLib{nullptr}`.
- Delete both copy and move operations on `Scripting` (`= delete` for all four), preventing raw `void*` handle duplication or double-freeing without move boilerplate. (Sweeping deleted move operations across all other engine classes is parked in `.scratch/ideas/issues/06-delete-move-operations-across-engine-classes.md`).
- Store resolved `hostfxr` and CoreCLR function pointers in an anonymous namespace inside `Scripting.cpp`.
- Start `Scripting.cpp` with `#include "OrcPch.hpp"` and restore top-of-file platform macros (`ORC_LOAD_LIB`, `ORC_GET_EXPORT`, `ORC_FREE_LIB`, `ORC_DOTNET_STR`).
- Remove generic template helpers (`getRuntimeDelegate`, `getManagedFunction`) and `std::error_code` path boilerplate in favor of direct C-API calls.
- Make `ORC_FATAL` and `ORC_FATAL_CHECK` in `Engine/Debug.hpp` invoke `std::abort()` and use `ORC_FATAL_CHECK` directly in `Scripting.cpp` without early returns.
- Preserve `OrcEngine.Scripting.Class1.Hello()` baseline execution on engine startup.
- Update ADR 0008 and `AGENTS.md` to reflect simplified architecture.

## Tickets
- [01: Flatten Scripting Header and Remove PIMPL](issues/01-flatten-scripting-header.md) (Status: ready-for-agent)
- [02: Simplify Scripting Implementation and Restore Macros](issues/02-simplify-scripting-implementation.md) (Status: ready-for-agent, Blocked by: 01)
- [03: Update AGENTS.md and ADR 0008 for Direct Scripting Architecture](issues/03-update-agents-md-and-adr-0008.md) (Status: ready-for-agent, Blocked by: 02)
