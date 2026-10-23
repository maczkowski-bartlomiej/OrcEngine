# Spec: .NET 10 CoreCLR Scripting Subsystem Refactoring & Hardening

## Problem Statement

OrcEngine recently migrated its managed scripting backend to .NET 10 CoreCLR using Microsoft's `hostfxr` hosting APIs. While the basic bridge successfully executes managed code, the current implementation has several architectural and reliability shortcomings from a game engine developer's perspective:

1. **Fragile and Permissive Error Handling**: The initialization flow in the scripting implementation logs warnings or errors and returns `false` (or skips steps) when discovering the hostfxr library, loading the runtime config, resolving delegates, or locating assemblies. Rather than failing immediately, execution continues until it encounters a null function pointer crash or an uninitialized runtime state.
2. **Interface Leakage and Incomplete Class Semantics**: The public scripting header accepts standard filesystem paths by value, lacks copy protection (leaving it susceptible to double-freeing native library handles), and exposes raw untyped pointers. Meanwhile, the implementation relies on global mutable function pointers in an anonymous namespace, preventing clean encapsulation and risking state bleed.
3. **Low Readability and Preprocessor Macro Clutter**: The implementation uses legacy C-style preprocessor macros for loading native libraries, querying exports, freeing handles, and wide-character string conversion, mixed with repeated, verbose `reinterpret_cast` expressions that obscure the core hosting sequence.
4. **Stale Nomenclature**: The central engine coordinator still refers to the scripting subsystem using legacy Mono identifiers.
5. **Missing Domain Documentation**: The ubiquitous language glossary, agent architectural documentation, and architectural decision records do not reflect the modern .NET 10 CoreCLR hostfxr architecture.

## Solution

Refactor and harden the scripting subsystem into a resilient, highly readable, and deeply encapsulated engine module:

1. **Unconditional Fail-Fast Crash Philosophy**: Enforce a strict "error -> crash" contract across the entire hosting pipeline. Any failure—locating hostfxr, loading the hostfxr shared library, resolving hosting delegates, reading the runtime configuration, loading the managed assembly, or resolving entry points—immediately halts the process with a fatal log, a debugger break in debug configurations, and an abort.
2. **PIMPL Encapsulation & Robust Value Semantics**: Refactor the public scripting header to use the PIMPL idiom, completely shielding engine consumers from native operating system handles, hostfxr function pointers, and .NET hosting headers. Enforce the Rule of 5 by explicitly deleting copy operations. Accept file paths via the engine's canonical path alias passed by `const&`.
3. **Type-Safe Static C++ Helpers**: Replace all preprocessor macros and repeated raw casts with typed static helper functions for dynamic library loading, symbol resolution, cross-platform host string conversions, and template-based delegate/function pointer extraction.
4. **Engine Nomenclature Alignment**: Clean up stale Mono member variable names in the engine coordinator to consistently use the canonical Scripting nomenclature.
5. **Domain Documentation & ADR**: Synchronize `GLOSSARY.md` with the canonical Scripting definition, expand `AGENTS.md` with the dedicated Scripting subsystem lifecycle and hosting model, and record an Architectural Decision Record capturing the migration from Mono to .NET 10 CoreCLR via hostfxr.
6. **Example Preservation**: Keep the verified `OrcEngine.Scripting.Class1.Hello()` managed entry point loaded and executed during engine startup to prove bridge integrity.

## User Stories

1. As a game engine developer, I want the scripting subsystem to fail fast with an informative fatal error when `hostfxr` is not installed on the system, so that I immediately know why the engine cannot initialize instead of chasing obscure segmentation faults.
2. As a game engine developer, I want missing or corrupt runtime configuration files (`.runtimeconfig.json`) to halt the engine immediately with fatal logs, so that configuration bugs are caught at launch time.
3. As a game engine developer, I want missing managed script assemblies (`.dll`) to trigger an immediate fatal crash, so that broken compilation outputs or bad deployment paths are never silently ignored.
4. As a game engine developer, I want failed entry point lookups to abort with the exact type and method name in the error message, so that missing or mismatched `[UnmanagedCallersOnly]` attributes are diagnosed instantly.
5. As a developer including engine headers, I want `Scripting.hpp` to be completely free of platform-specific headers (`Windows.h`, `dlfcn.h`) and .NET hosting headers (`nethost.h`, `hostfxr.h`, `coreclr_delegates.h`), so that my compilation times stay fast and my global namespace is not polluted.
6. As an engine maintainer, I want the `Scripting` class to delete its copy constructor and copy assignment operator, so that native dynamic library handles are never inadvertently copied or double-freed.
7. As an engine maintainer, I want `Scripting` methods to accept file paths using `const FilePath&`, so that the scripting API follows the engine's universal argument passing conventions.
8. As a developer reading the scripting implementation, I want dynamic library loading and symbol lookup to be expressed through clean static C++ helper functions rather than preprocessor macros, so that the code is type-safe and idiomatic C++20.
9. As a developer reading the scripting implementation, I want hostfxr delegate queries to use a typed template helper, so that redundant `reinterpret_cast<void**>` expressions and error checks are eliminated from the hosting flow.
10. As a developer reading the scripting implementation, I want string conversions between standard engine strings and host `char_t` strings to be handled by a cross-platform helper function, so that wide-character logic on Windows and UTF-8 logic on Linux is cleanly isolated.
11. As an engine coordinator developer, I want the member representing the scripting subsystem in the main engine class to be named `m_scripting` instead of `m_mono`, so that the codebase accurately reflects modern .NET 10 CoreCLR.
12. As a game developer running the engine, I want `Class1.Hello()` in `OrcEngine.Scripting` to continue executing on startup, so that I have a living baseline confirming the managed runtime bridge works.
13. As an autonomous agent working in the codebase, I want `GLOSSARY.md` to define **Scripting** and specify deprecated terms to avoid (`Mono`, `ScriptEngine`, `DotNetManager`), so that my domain terminology remains consistent across tickets.
14. As an autonomous agent working in the codebase, I want `AGENTS.md` to detail the Scripting Subsystem lifecycle and hosting architecture, so that future scripting extensions adhere to established patterns.
15. As a software architect, I want an Architectural Decision Record (ADR) documenting why OrcEngine chose .NET 10 CoreCLR hostfxr over Mono, so that future contributors understand the trade-offs and design rationale.
16. As a CI/CD pipeline, I want the unified verification gate (`verify.ps1` / `verify.sh`) to pass with 100% success across conventions linting, compilation, and the smoke test, so that changes maintain total stability.

## Implementation Decisions

- **PIMPL Interface Separation**:
  - The public `Scripting` class provides only the public lifecycle interface (`Scripting(const FilePath& configPath)`, destructor, move semantics, deleted copy operations).
  - All native platform library handles (`HMODULE` / `void*`), hostfxr function pointers, and delegate pointers are held inside an internal `Impl` struct defined exclusively within the implementation translation unit.
  - The type shape established in our prototype:
    ```cpp
    class Scripting {
    public:
        explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json");
        ~Scripting();
        Scripting(const Scripting&) = delete;
        Scripting& operator=(const Scripting&) = delete;
        Scripting(Scripting&&) noexcept;
        Scripting& operator=(Scripting&&) noexcept;
    private:
        struct Impl;
        UniquePtr<Impl> m_impl;
    };
    ```

- **Fail-Fast Crash Policy**:
  - All internal initialization methods return `void`.
  - A dedicated fatal check macro evaluates conditions and, on failure, logs an `ORC_LOG_FATAL` message, triggers `ORC_DEBUGBREAK()`, and invokes `std::abort()`.
  - The policy applies to every hosting stage:
    1. Locating the hostfxr library via `get_hostfxr_path`.
    2. Loading the hostfxr shared library.
    3. Resolving required exports (`hostfxr_initialize_for_runtime_config`, `hostfxr_get_runtime_delegate`, `hostfxr_close`).
    4. Existence and canonicalization of the runtime configuration path.
    5. Initializing the hostfxr context.
    6. Acquiring the `load_assembly_and_get_function_pointer` runtime delegate.
    7. Existence of the target assembly DLL.
    8. Resolving the unmanaged caller function pointer.

- **Encapsulated C++ Helpers**:
  - Remove `#define ORC_LOAD_LIB`, `#define ORC_GET_EXPORT`, `#define ORC_FREE_LIB`, and `#define ORC_DOTNET_STR`.
  - Implement static C++ functions:
    - `loadLibrary(const FilePath& path) -> void*`
    - `getExport(void* lib, const char* symbol) -> void*`
    - `freeLibrary(void* lib) -> void`
    - `toPalString(const std::string& str) -> pal_string`
    - `template<typename T> T getRuntimeDelegate(hostfxr_handle context, hostfxr_delegate_type type)`
    - `template<typename T> T getManagedFunction(load_assembly_and_get_function_pointer_fn loader, const FilePath& assemblyPath, const std::string& typeName, const std::string& methodName)`

- **Engine Coordinator Renaming**:
  - Rename `m_mono` to `m_scripting` in `Engine.hpp` and `Engine.cpp`.
  - Maintain the exact instantiation order: logging -> scripting -> layer manager -> window -> renderer -> gui -> audio -> input -> resources -> context.

- **Preservation of Example**:
  - Maintain `OrcEngine.Scripting.Class1.Hello()` in `OrcEngine.Scripting/Class1.cs`.
  - Retain the invocation of `Hello()` during runtime initialization, preserving the `[Managed .NET 10] Hello from C# Scripting inside OrcEngine!` console log.

## Testing Decisions

- **Definition of Good Test**:
  - Only test external behavioral outcomes and subsystem contracts, never private implementation details or memory layout.
  - A good test verifies that when the engine is initialized with valid configuration and assemblies, .NET CoreCLR starts, delegates bind, and managed entry points execute cleanly without error logs or exceptions.
  - A good negative test verifies that missing dependencies immediately abort the process rather than entering undefined behavior.
- **Modules Tested**:
  - `Scripting` subsystem
  - `Engine` lifecycle coordination
  - `TestProject` application integration
- **Testing Seam**:
  - The single, highest testing seam is the engine initialization lifecycle in `Engine::Engine(const Config&)` invoked by `TestProject` (and executed via `scripts/run-demo.ps1` and `scripts/verify.ps1`).
- **Prior Art**:
  - The unified verification gate (`scripts/verify.ps1`) runs conventions linting, compiles the target preset, and executes the 5-second smoke test while validating that no error logs or assertion failures occur.

## Out of Scope

- Adding custom C# game layers or a C# scripting API for entities and components (reserved for upcoming scripting feature epics).
- Dynamic hot-reloading or unloading of .NET AssemblyLoadContexts at runtime.
- Complex interop marshalling layers (e.g. passing math vectors, textures, or engine delegates to C#).
- Modifying CMake build orchestration for `OrcEngine.Scripting.csproj` (the existing custom target already builds the managed assembly cleanly into the executable directory).

## Further Notes

- All changes must strictly pass `.\scripts\verify.ps1` with 100% success before any issue is considered resolved.
- Adheres to ADR 0006 (`Core` modularization) by relying on `Core/Types.hpp` and `Core/Memory.hpp`.
