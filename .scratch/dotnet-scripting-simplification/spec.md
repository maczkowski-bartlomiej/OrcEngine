# Spec: .NET 10 Scripting Subsystem Simplification

## Problem Statement

The scripting subsystem was recently refactored with unnecessary architectural complexity that degraded developer ergonomics, readability, and consistency with engine conventions:

1. **Unnecessary PIMPL Indirection**: The public scripting header introduced an opaque `Impl` struct and `UniquePtr<Impl>` to hide platform headers. However, storing an untyped dynamic library pointer (`void*`) and a configuration path requires zero platform or .NET hosting headers. Using PIMPL added dynamic heap allocation on engine boot, forced out-of-line destructor and move definitions, and added verbose pointer chasing (`m_impl->...`) without providing tangible encapsulation benefits.
2. **Reinvented Error Handling**: The implementation rolled its own private fatal check macro (`ORC_SCRIPTING_FATAL_CHECK`) that duplicated functionality belonging in the engine's canonical `Debug.hpp` module (`ORC_FATAL_CHECK` and `ORC_FATAL`).
3. **Scattered Platform Branching**: Platform OS loading and string conversion logic were split into separate functions (`loadLibrary`, `getExport`, `freeLibrary`, `toPalString`), scattering `#if defined(ORC_PLATFORM_WINDOWS)` directives across multiple function bodies rather than isolating platform differences in a clean, unified macro block at the top of the file.
4. **Template Over-Engineering**: Internal delegate retrieval and entry-point binding were wrapped in generic template functions (`getRuntimeDelegate<T>`, `getManagedFunction<T>`) inside a private translation unit. This added template signatures, type parameters, and extra indirection over what are straightforward, linear C-API calls.

## Solution

Simplify and streamline the scripting subsystem into clean, direct, and idiomatic C++20:

1. **Flatten the Scripting Class & Delete Copy/Move**: Remove `struct Impl` and `m_impl`. The `Scripting` class will directly hold its configuration path and native library handle (`void* m_hostfxrLib{nullptr}`), deleting both copy and move operations (`= delete`) like `Audio`. The public header will declare its lifecycle methods (`loadHostFxr()`, `initializeRuntime()`) directly as private members without importing OS or .NET hosting headers.
2. **Standardize on Canonical Engine Debug Facilities**: Make `ORC_FATAL` and `ORC_FATAL_CHECK` in `Engine/Debug.hpp` call `std::abort()` (in both Debug and Release configurations), and remove the custom `ORC_SCRIPTING_FATAL_CHECK` macro from `Scripting.cpp` in favor of single-line `ORC_FATAL_CHECK` calls.
3. **Consolidate Platform Macros & Include `OrcPch.hpp`**: Start `Scripting.cpp` with `#include "OrcPch.hpp"` and replace the scattered helper functions with a localized platform preprocessor block (`ORC_LOAD_LIB`, `ORC_GET_EXPORT`, `ORC_FREE_LIB`, `ORC_DOTNET_STR`) at the top of the implementation file.
4. **Inline Hostfxr C-API Calls**: Store resolved `hostfxr` and CoreCLR function pointers in an anonymous namespace in `Scripting.cpp`, eliminate the template wrapper functions, and call `hostfxr_get_runtime_delegate` and `loadAssemblyAndGetFunctionPointer` directly in sequence with simplified `std::filesystem::exists` + `std::filesystem::canonical` path checks.
5. **Preserve Managed Entry Point Baseline**: Maintain the execution of `OrcEngine.Scripting.Class1.Hello()` on engine startup to prove runtime bridge integrity.
6. **Documentation Alignment**: Update `AGENTS.md` and ADR 0008 to reflect the simplified, direct, non-PIMPL architecture.

## Implementation Decisions

- **Direct Class Structure**:
  - The `Scripting` class declares `FilePath m_configPath` and `void* m_hostfxrLib{nullptr}` as direct private member variables.
  - Internal initialization logic is structured into private member methods: `void loadHostFxr()` and `void initializeRuntime()`.
  - Both copy and move operations are deleted (`= delete`) to prevent duplicate handles or double-freeing `m_hostfxrLib`.
  - No heap allocation (`make_unique` / `createUniquePtr`) is performed for `Scripting` internals.
  - The type shape:
    ```cpp
    class Scripting
    {
    public:
    	explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json");
    	~Scripting();

    	Scripting(const Scripting&) = delete;
    	Scripting& operator=(const Scripting&) = delete;
    	Scripting(Scripting&&) = delete;
    	Scripting& operator=(Scripting&&) = delete;

    private:
    	void loadHostFxr();
    	void initializeRuntime();

    	FilePath m_configPath;
    	void* m_hostfxrLib{nullptr};
    };
    ```

- **Canonical Debug Integration**:
  - Update `ORC_FATAL` and `ORC_FATAL_CHECK` in `OrcEngine/include/Orc/Engine/Debug.hpp` to invoke `std::abort()` in both debug and release branches.
  - Eliminate the custom `ORC_SCRIPTING_FATAL_CHECK` macro from `Scripting.cpp`.
  - Replace custom abort checks with `ORC_FATAL_CHECK(condition, message, ...)` directly without early returns.

- **Localized Preprocessor Macros**:
  - Include `"OrcPch.hpp"` as the first header in `Scripting.cpp`.
  - Group all platform-specific library loading, export querying, freeing, and wide-character string prefixing into a single `#if defined(ORC_PLATFORM_WINDOWS) ... #else ... #endif` block at the top of `Scripting.cpp`:
    - Windows:
      - `#define ORC_LOAD_LIB(path) ::LoadLibraryW(path)`
      - `#define ORC_GET_EXPORT(h, name) ::GetProcAddress(static_cast<HMODULE>(h), name)`
      - `#define ORC_FREE_LIB(h) ::FreeLibrary(static_cast<HMODULE>(h))`
      - `#define ORC_DOTNET_STR(s) L##s`
    - Linux:
      - `#define ORC_LOAD_LIB(path) ::dlopen(path, RTLD_LAZY)`
      - `#define ORC_GET_EXPORT(h, name) ::dlsym(h, name)`
      - `#define ORC_FREE_LIB(h) ::dlclose(h)`
      - `#define ORC_DOTNET_STR(s) s`
  - Delete `loadLibrary`, `getExport`, `freeLibrary`, and `toPalString` helper functions.

- **Direct Non-Templated Hosting Flow**:
  - Delete `getRuntimeDelegate<T>` and `getManagedFunction<T>`.
  - Store resolved `hostfxr` and CoreCLR function pointers in an anonymous namespace inside `Scripting.cpp`:
    - `hostfxr_initialize_for_runtime_config_fn`
    - `hostfxr_get_runtime_delegate_fn`
    - `hostfxr_close_fn`
    - `load_assembly_and_get_function_pointer_fn`
  - Simplify path validation to `ORC_FATAL_CHECK(std::filesystem::exists(...), ...)` followed by `std::filesystem::canonical(...)` without `std::error_code` boilerplate.
  - In `initializeRuntime()`, query `hdt_load_assembly_and_get_function_pointer` directly and invoke `loadAssemblyAndGetFunctionPointer` directly with `UNMANAGEDCALLERSONLY_METHOD` and `ORC_DOTNET_STR(...)` to acquire `Class1.Hello`.

- **Documentation & ADR Update**:
  - Update `AGENTS.md` Section 4.8 to describe the direct class architecture, deleted copy/move semantics, top-level platform macro structure, and `ORC_FATAL_CHECK` usage.
  - Update `docs/adr/0008-hostfxr-dotnet-coreclr-scripting.md` to remove references to PIMPL and template helpers, capturing the decision to favor direct class layout and canonical `Debug.hpp` macros.

## Testing Decisions

- **Definition of Good Test**:
  - Only test external behavioral outcomes and subsystem contracts, never internal memory layout or private member pointers.
  - A good test verifies that the engine initializes, boots .NET 10 CoreCLR via hostfxr, executes `Class1.Hello()`, and shuts down cleanly without memory leaks or crashes.
- **Modules Tested**:
  - `Scripting` subsystem
  - `Engine` lifecycle coordination
  - `TestProject` application integration
- **Testing Seam**:
  - The single, highest testing seam is the engine initialization lifecycle in `Engine::Engine(const Config&)` invoked by `TestProject` (and executed via `scripts/verify.ps1`).
