# 02: Implement Fail-Fast Crash Policy and Helper Functions in Scripting.cpp

Status: resolved
Type: task
Blocked by: 01

**What to build:** Implement `Scripting::Impl` in `Scripting.cpp`. Enforce the unconditional "error -> crash" philosophy across all hosting steps (finding hostfxr, loading library, resolving exports, loading runtimeconfig, querying delegates, loading assembly, resolving entry points). Replace C macros with type-safe static C++ helper functions for library loading, export querying, host string conversions, and template-based delegate resolution. Ensure `OrcEngine.Scripting.Class1.Hello()` is invoked.

**Blocked by:** 01 (needs PIMPL definition and `Scripting.hpp` interface)

- [x] Unconditional fatal check macro evaluates conditions and triggers `ORC_LOG_FATAL` + `ORC_DEBUGBREAK` + `std::abort()` on any failure
- [x] Internal initialization methods return `void` and abort immediately on failure instead of returning `bool` or logging warnings
- [x] Hostfxr function pointers and library handles are encapsulated in `Scripting::Impl` without global mutable variables
- [x] Static C++ helper functions replace `#define ORC_LOAD_LIB`, `#define ORC_GET_EXPORT`, `#define ORC_FREE_LIB`, and `#define ORC_DOTNET_STR`
- [x] Template helper `getRuntimeDelegate` encapsulates typed delegate acquisition
- [x] Helper `getManagedFunction` encapsulates assembly loading and `[UnmanagedCallersOnly]` entry point resolution
- [x] `Class1.Hello()` invocation remains intact and executes during startup
- [x] Conforms to conventions linter (tab indentation, `#pragma once`, namespaces)

## Answer

Implemented Scripting::Impl with fail-fast crash policy, replaced preprocessor macros with static C++ helpers (loadLibrary, getExport, freeLibrary, toPalString, getRuntimeDelegate, getManagedFunction), encapsulated hostfxr function pointers, and verified Class1.Hello() startup execution.
