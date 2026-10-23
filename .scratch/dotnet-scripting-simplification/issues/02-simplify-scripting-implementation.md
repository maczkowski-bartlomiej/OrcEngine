# 02: Simplify Scripting Implementation and Restore Macros

Status: ready-for-agent
Type: task
Blocked by: 01

**What to build:**
1. Update `ORC_FATAL` and `ORC_FATAL_CHECK` in `OrcEngine/include/Orc/Engine/Debug.hpp` to invoke `std::abort()` (in both `#ifdef ORC_DEBUG_ASSERTS` and `#else`), making fatal checks unconditionally abort on failure across the engine.
2. Simplify `OrcEngine/source/Orc/Script/Scripting.cpp`:
   - Start with `#include "OrcPch.hpp"` (only including `<dlfcn.h>` on non-Windows).
   - Restore the top-of-file platform macro block:
     - Windows: `ORC_LOAD_LIB(path)` -> `::LoadLibraryW(path)`, `ORC_GET_EXPORT(h, name)` -> `::GetProcAddress(static_cast<HMODULE>(h), name)`, `ORC_FREE_LIB(h)` -> `::FreeLibrary(static_cast<HMODULE>(h))`, `ORC_DOTNET_STR(s)` -> `L##s`
     - Linux: `ORC_LOAD_LIB(path)` -> `::dlopen(path, RTLD_LAZY)`, `ORC_GET_EXPORT(h, name)` -> `::dlsym(h, name)`, `ORC_FREE_LIB(h)` -> `::dlclose(h)`, `ORC_DOTNET_STR(s)` -> `s`
   - Store the resolved `.NET` function pointers (`hostfxr_initialize_for_runtime_config_fn`, `hostfxr_get_runtime_delegate_fn`, `hostfxr_close_fn`, `load_assembly_and_get_function_pointer_fn`) in an anonymous namespace inside `Scripting.cpp`.
   - Remove standalone wrapper functions (`loadLibrary`, `getExport`, `freeLibrary`, `toPalString`), template helpers (`getRuntimeDelegate`, `getManagedFunction`), and the custom `ORC_SCRIPTING_FATAL_CHECK` macro.
   - Use `ORC_FATAL_CHECK` from `Engine/Debug.hpp` directly without early-return boilerplate.
   - Simplify filesystem checks in `initializeRuntime()` by checking `std::filesystem::exists(...)` via `ORC_FATAL_CHECK` and calling `std::filesystem::canonical(...)` directly without `std::error_code` boilerplate.
   - Remove move constructor and move assignment definitions from `Scripting.cpp` (since both copy and move are `= delete` in `Scripting.hpp`).
   - Free `m_hostfxrLib` in `Scripting::~Scripting()` via `ORC_FREE_LIB`.

**Blocked by:** 01 (needs updated `Scripting.hpp` class definition)

- [ ] `ORC_FATAL` and `ORC_FATAL_CHECK` in `Engine/Debug.hpp` invoke `std::abort()` in both debug and release branches
- [ ] `Scripting.cpp` starts with `#include "OrcPch.hpp"` and restores the top-of-file platform macro block (`ORC_LOAD_LIB`, `ORC_GET_EXPORT`, `ORC_FREE_LIB`, `ORC_DOTNET_STR`)
- [ ] Resolved `hostfxr` and `load_assembly_and_get_function_pointer_fn` function pointers are stored in an anonymous namespace in `Scripting.cpp`
- [ ] Standalone helper functions (`loadLibrary`, `getExport`, `freeLibrary`, `toPalString`) and template helpers (`getRuntimeDelegate`, `getManagedFunction`) are eliminated
- [ ] Custom macro `ORC_SCRIPTING_FATAL_CHECK` is eliminated; uses `ORC_FATAL_CHECK` directly without early-return boilerplate
- [ ] `std::error_code` boilerplate around `std::filesystem::canonical` is removed in favor of `std::filesystem::exists` + `std::filesystem::canonical`
- [ ] Move constructor and move assignment definitions are removed from `Scripting.cpp`
- [ ] `Scripting::loadHostFxr()` and `Scripting::initializeRuntime()` operate directly on `Scripting` members and invoke `Class1.Hello()` during startup
- [ ] Destructor frees `m_hostfxrLib` via `ORC_FREE_LIB`
- [ ] Code compiles without warnings and passes conventions linter
