# 01: Bootstrap C# SDK & Consumer Assembly Topology, Single Load-Context Handshake, Fail-Fast Routing & Logger Binding

Status: ready-for-agent
Type: task
Blocked by: none

**What to build:** Establish the managed build topology, single-load-context assembly hosting, ABI-safe bidirectional function-pointer handshake, fail-fast exception and thread checks, and `Orc.Logger` binding between `OrcEngine`, `OrcEngine.Scripting`, and `TestProject.Scripts`. See spec §1–§3, §6, and §7.
- **Build & cleanup:** Delete the legacy bootstrap C# files and committed `.idea/` directory. Configure CMake and `.csproj` files so `OrcEngine.Scripting` and `TestProject.Scripts` compile with zero warnings and code-style enforcement into the `TestProject` executable directory.
- **Hosting:** Load both `OrcEngine.Scripting.dll` and `TestProject.Scripts.dll` (via `Config::scriptAssemblyPath`) into the default `AssemblyLoadContext` using `hdt_load_assembly` + `hdt_get_function_pointer`.
- **Handshake & fail-fast:** Replace the bootstrap `Hello` call with size-validated `NativeApiTable` / `ManagedLifecycleTable` structs, POD interop mirrors, main-thread validation, and `try/catch` → `ORC_FATAL` routing on every `[UnmanagedCallersOnly]` entry point.
- **Logger:** Expose `Orc.Logger` (`Info`, `Warning`, `Fatal`) forwarding C# caller file, line, and member names to native `Logger`.
- **Verification & docs:** Enhance `run-demo` to scan the full appended log section for expected markers, require `Scripting: managed runtime initialized` in `scripts/verify`, and land the C# coding standards and initial ADR 0012.

## Acceptance Criteria

- [ ] **Topology & cleanup**
  - `OrcEngine.Scripting` is the single Engine C# SDK (`namespace Orc` for public API, `namespace Orc.Internal` for `internal` glue; no `OrcEngine.ScriptingGlue` project).
  - A separate `TestProject/TestProject.Scripts/TestProject.Scripts.csproj` project references `OrcEngine.Scripting`.
  - CMake builds `TestProject.Scripts` (and transitively `OrcEngine.Scripting`) into `$<TARGET_FILE_DIR:TestProject>` and fails configuration (`FATAL_ERROR`) if `DOTNET_EXECUTABLE` is not found.
  - Legacy `OrcEngine.Scripting/Class1.cs`, `Script.cs`, `ScriptHost.cs`, and `OrcEngine.Scripting/.idea/` are deleted, and `.idea/` is ignored in `.gitignore`.
- [ ] **C# build & style gate**
  - Both `.csproj` files enable `<Nullable>enable</Nullable>`, `<TreatWarningsAsErrors>true</TreatWarningsAsErrors>`, and `<EnforceCodeStyleInBuild>true</EnforceCodeStyleInBuild>` (and remove the invalid `<AllowUnsafe>` property).
  - `OrcEngine.Scripting` declares `[assembly: DisableRuntimeMarshalling]`.
  - A repository `.editorconfig` encodes the C# conventions from spec §7 (tabs, Allman braces, file-scoped namespaces, `PascalCase` types/methods/properties, `m_camelCase` instance fields, `s_camelCase` static fields).
- [ ] **Config extension:** `orc::Config` includes `FilePath scriptAssemblyPath` (empty by default; set to `"TestProject.Scripts.dll"` in `TestProject`) and passes it to `Scripting` during `Engine` construction.
- [ ] **Single default `AssemblyLoadContext`**
  - `Scripting::initializeRuntime()` uses `hdt_load_assembly` and `hdt_get_function_pointer` to load `OrcEngine.Scripting.dll` into the default load context and resolve `Orc.Internal.ScriptRuntime.Initialize`.
  - `LoadScriptAssembly` loads `scriptAssemblyPath` via `AssemblyLoadContext.Default.LoadFromAssemblyPath(canonicalPath)`, so `TestProject.Scripts` binds to the already-loaded `OrcEngine.Scripting` assembly.
- [ ] **ABI-safe handshake**
  - `NativeApiTable` and `ManagedLifecycleTable` both start with `uint32_t structSize`, validated on both sides with `static_assert`s and runtime fail-fast checks.
  - Native glue functions are `static` functions in an anonymous namespace inside private `OrcEngine/source/Orc/Script/` sources (no `extern "C"`, avoiding MSVC C4190).
  - A private interop header defines POD mirror structs (`InteropVector2f`, `InteropVector3f`, `InteropColor`, `InteropFloatRect`, `InteropSpecialKeys`) with `static_assert`s on `sizeof`, `std::is_trivial`, and `std::is_standard_layout`.
  - Zero `[DllImport]` / `[LibraryImport]` P/Invoke declarations exist.
- [ ] **Fail-fast & main-thread contract**
  - `NativeApiTable` includes `fatal(const char* message)` implemented via `ORC_FATAL("{}", message)`.
  - Every `[UnmanagedCallersOnly]` method wraps its body in `try { ... } catch (Exception exception) { NativeBindings.Fatal(exception.ToString()); }`.
  - `loadScriptAssembly` returns `void` and fails fast if the assembly file does not exist or fails to load.
  - `ScriptRuntime` records `Environment.CurrentManagedThreadId` at initialization and fails fast if any `NativeBindings` call is made from another thread.
- [ ] **Logger binding**
  - C++ `orc::Logger` adds a non-template overload accepting raw `const char* file`, `int32_t line`, and `const char* function` values.
  - `Orc.Logger.Info` and `Orc.Logger.Warning` forward `[CallerFilePath]`, `[CallerLineNumber]`, and `[CallerMemberName]` across `NativeApiTable.log`.
  - `Orc.Logger.Fatal` (`[DoesNotReturn]`) logs at `Fatal` level with caller location and terminates via `fatal`.
  - During `ScriptRuntime.Initialize`, C# logs `Scripting: managed runtime initialized` through `Orc.Logger.Info`.
- [ ] **Log verification in `run-demo` & `verify`**
  - `scripts/run-demo.ps1` and `scripts/run-demo.sh` inspect the entire log slice appended during the current run (using the recorded initial log byte offset rather than only the last 20 lines) and accept one or more `-ExpectLogPattern` / `--expect-log-pattern` arguments.
  - `scripts/verify.ps1` and `scripts/verify.sh` pass `Scripting: managed runtime initialized` as an expected log pattern.
- [ ] **Documentation (Ticket 01 deliverables)**
  - `CODING_STANDARDS.md` defines the formal C# Coding Standards section (spec §7) and removes the "will be formally defined" placeholder.
  - `docs/adr/0012-csharp-module-bindings-and-script-lifecycle.md` is created covering the single-SDK + consumer assembly topology in one default load context, size-validated function pointer tables, POD interop structs with `DisableRuntimeMarshalling`, and fail-fast exception/thread routing.
  - `docs/adr/0008-hostfxr-dotnet-coreclr-scripting.md` is annotated to note that `load_assembly_and_get_function_pointer` is superseded by ADR 0012.
  - `AGENTS.md` is updated with the C# toolchain, `Scripting` loading the consumer assembly in the initialization order, and the ADR 0012 pointer.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning C++ and C# build, and 5-second `TestProject` smoke test verifying `Scripting: managed runtime initialized`).

## Comments
