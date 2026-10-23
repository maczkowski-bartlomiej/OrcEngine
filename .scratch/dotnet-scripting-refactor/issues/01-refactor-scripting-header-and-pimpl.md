# 01: Refactor Scripting Header and PIMPL Interface

Status: resolved
Type: task
Blocked by: none

**What to build:** Refactor `Scripting.hpp` to conform to engine standards: pass `const FilePath& configPath`, enforce the Rule of 5 by explicitly deleting copy operations, and declare a private `Impl` pointer (`UniquePtr<Impl> m_impl`) so that native handles, function pointers, and .NET headers are completely hidden from public engine headers.

**Blocked by:** None (can start immediately)

- [x] `Scripting.hpp` includes `Core/Types.hpp` and `Core/Memory.hpp`
- [x] Constructor signature accepts `explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json")`
- [x] Copy constructor and copy assignment operator are deleted (`Rule of 5`)
- [x] Move constructor and move assignment operator are explicitly declared or defaulted
- [x] Private implementation uses PIMPL pattern (`struct Impl; UniquePtr<Impl> m_impl;`)
- [x] Public header contains zero platform headers (`<Windows.h>`, `<dlfcn.h>`) and zero .NET hosting headers (`<nethost.h>`, `<hostfxr.h>`, `<coreclr_delegates.h>`)
- [x] Header compiles cleanly and conforms to conventions linter

## Answer

Refactored Scripting.hpp to use PIMPL (UniquePtr<Impl> m_impl), Rule of 5 semantics with deleted copy operations and defaulted move operations, const FilePath& configPath, and encapsulated native handles within Scripting.cpp.
