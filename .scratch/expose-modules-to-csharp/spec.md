# Expose Engine Modules to C# Scripting

Status: ready-for-agent

## Problem Statement

While `OrcEngine` embeds the .NET 10 CoreCLR runtime via `hostfxr` inside the `Scripting` subsystem (per ADR 0008), its current C# integration is only a minimal bootstrap harness that invokes a single static `Hello()` method at startup. Managed C# code has no way to participate in the `Engine` frame loop (`onAttach`, `onDetach`, `onUpdate`, `onRender`), no scoped access to engine subsystems (`Renderer`, `Input`, `Audio`, `Camera`, `TextureResources`, `FontResources`), and no managed representations of 2D drawables (`Sprite`, `Rectangle`, `Circle`, `Text`), audio buses (`Bus`), or `Core` primitives (`Logger`, math vectors, `Color`, `Rect`).

At the same time, introducing C# bindings raises four structural challenges:
1. **Managed Assembly Topology**: Splitting low-level native interop into a separate `OrcEngine.ScriptingGlue` project would create a shallow pass-through assembly, force `InternalsVisibleTo` or public raw pointers, and require `Scripting` to coordinate multiple engine assemblies—whereas game/demo scripts in `TestProject` *do* belong in a separate consumer assembly from the reusable `OrcEngine.Scripting` SDK. Both assemblies must share **one** `AssemblyLoadContext`, otherwise the consumer binds to a second copy of the SDK (split type identity, uninitialized native function table).
2. **Static Library Interop Seam**: Because `OrcEngine` is compiled as a static library linked directly into the host executable, traditional `[DllImport]` / `[LibraryImport]` P/Invoke calls from C# into `OrcEngine` would require executable symbol exports and custom `NativeLibrary` resolvers rather than direct unmanaged function pointer dispatch. The seam must also be ABI-exact on both MSVC x64 and SysV x64 (by-value struct returns, `bool` width).
3. **Layer Lifecycle & Singleton-Free Subsystem Access**: Under ADR 0004, `OrcEngine` eliminated ambient `Engine` singletons in favor of `LayerContext` constructor injection on `GameLayer`. Managed scripts attached to a `GameLayer` need a matching singleton-free `ScriptContext` and automatic lifecycle dispatch (`OnAttach`, `OnDetach`, `OnUpdate`, `OnRender`, `OnDestroy`) without requiring derived C++ `GameLayer` classes to manually invoke base `GameLayer::on*` methods.
4. **Deterministic Native Lifetime Under a Garbage Collector**: Managed wrappers of native objects that hold `Ref<Texture>` / `Ref<Font>` must never release native or OpenGL resources from the .NET finalizer thread, and leaks must uphold the `Fail-Fast Crash Contract` (ADR 0009) instead of being silently tolerated.

## Solution

Expand `OrcEngine.Scripting` into the single reusable Engine C# SDK (`namespace Orc`) backed by internal bidirectional unmanaged function pointer tables, load it and the dedicated `TestProject.Scripts` consumer assembly into the default `AssemblyLoadContext`, attach managed `Script` instances (`std::vector<Script>`) to C++ `GameLayer`s with automatic engine-driven lifecycle dispatch and an automatic `Scene Scope` around script rendering, and expose `Core` (`Logger`, `Vector2f`, `Vector3f`, `Color`, `FloatRect`), `Renderer` (`SetClearColor`, `Draw`, `DrawLine`), the owning layer's `Camera`, `Transformable`, `Texturable`, `Sprite`, `Rectangle`, `Circle`, `Text`, `Input` (`Key`, `SpecialKeys`, `MouseButton`), `Audio` (`Bus`), and read-only `TextureResources` / `FontResources` lookups through a singleton-free `ScriptContext`.

Specifically:
1. **Single Engine SDK Assembly (`OrcEngine.Scripting`) + Consumer Script Assembly (`TestProject.Scripts`) in One Load Context**:
   - Keep both `internal` native interop glue (`NativeApiTable`, `ManagedLifecycleTable`, `NativeBindings`, `ScriptRuntime`) and the `public` idiomatic C# API (`namespace Orc`) inside `OrcEngine.Scripting`—avoiding a redundant `OrcEngine.ScriptingGlue` assembly.
   - Load both into the default `AssemblyLoadContext` (`hdt_load_assembly` + `hdt_get_function_pointer`), so `TestProject.Scripts` binds to the already-loaded SDK.
2. **Bidirectional `[UnmanagedCallersOnly]` & `delegate* unmanaged[Cdecl]` Function Pointer Tables with an ABI-Exact Seam**:
   - Preserve `OrcEngine` as a static library with zero P/Invoke dependencies.
   - Size-validated tables, POD interop mirror structs, `[assembly: DisableRuntimeMarshalling]`, and fail-fast exception routing at every managed entry point.
3. **`GameLayer` Script Ownership & Engine-Driven Lifecycle with Automatic `Scene Scope`**:
   - `GameLayer` owns `std::vector<Script>`; `GameLayerManager` and `Engine::run()` drive `attachScripts()`, `detachScripts()`, `updateScripts()`, and `renderScripts()`.
   - `renderScripts()` always brackets script rendering in `beginScene(m_camera)` / `endScene()`; scripts never open or close scenes themselves.
4. **Deterministic Native Ownership with Fail-Fast Leak Detection**:
   - Owning wrappers (`Sprite`, `Rectangle`, `Circle`, `Text`, `Bus`) are `IDisposable` **without finalizers**; a live-wrapper registry fails fast at shutdown if any wrapper or script is still alive.
5. **Singleton-Free `ScriptContext` Available From the Script Constructor**.

## User Stories

1. As an engine architect, I want low-level native interop (`delegate* unmanaged[Cdecl]` pointers and `[UnmanagedCallersOnly]` entry points) encapsulated as `internal` types inside `OrcEngine.Scripting` rather than in a separate `OrcEngine.ScriptingGlue` project, so that the engine ships a single managed SDK assembly without pass-through wrapper overhead or leaked raw pointers.
2. As a game developer working in `TestProject`, I want my game and demo scripts to live in a dedicated `TestProject.Scripts` C# project that references `OrcEngine.Scripting`, so that reusable engine SDK code and consumer game scripts are cleanly separated.
3. As an engine maintainer, I want the SDK and the consumer script assembly loaded into the same default `AssemblyLoadContext`, so that there is exactly one copy of `Orc.Script` (one type identity) and one initialized `NativeApiTable` at runtime.
4. As an engine maintainer, I want CMake to automatically build `TestProject.Scripts` and `OrcEngine.Scripting` into the `TestProject` executable directory with C# warnings treated as errors, and to fail configuration if the `dotnet` SDK is missing, so that the unified build and verification scripts enforce the zero-warning gate for C# too and never produce a runtime-only scripting failure.
5. As an engine maintainer, I want C# to call into `OrcEngine` via a `NativeApiTable` of unmanaged function pointers registered at `Scripting` startup rather than `[DllImport]` / `[LibraryImport]`, so that `OrcEngine` remains a static C++ library without DLL export macros or runtime P/Invoke symbol resolution.
6. As an engine maintainer, I want every value crossing the native/managed seam to use POD interop structs with compile-time layout assertions, 1-byte `bool`s under `[assembly: DisableRuntimeMarshalling]`, and size-validated function tables, so that by-value returns and boolean results are ABI-exact on both MSVC x64 and SysV x64 and table drift fails fast instead of calling the wrong function.
7. As a game developer configuring `OrcEngine`, I want `orc::Config` to include a `scriptAssemblyPath` field, so that `Scripting` automatically loads my game's compiled C# script assembly at engine startup.
8. As an engine maintainer, I want `LayerContext` to expose a reference to `Scripting`, so that any `GameLayer` can instantiate and manage C# scripts without a global `Scripting` singleton.
9. As a game developer writing a C++ `GameLayer`, I want to attach one or more C# scripts to my layer by calling `addScript("Namespace.ClassName")`, so that `GameLayer` manages their lifetime in its internal `std::vector<Script>` collection.
10. As a game developer subclassing `orc::GameLayer` in C++, I want `GameLayerManager` and `Engine::run()` to invoke `attachScripts()`, `detachScripts()`, `updateScripts(deltaTime)`, and `renderScripts()` automatically alongside `onAttach()`, `onDetach()`, `onUpdate(deltaTime)`, and `onRender()`, so that overriding `onUpdate` or `onRender` in a C++ layer never requires calling base `GameLayer::on*` methods or risks breaking attached C# scripts.
11. As a C# game developer, I want my script's `OnRender()` to always execute inside a `Scene Scope` bound to the owning layer's `Camera`, so that I can submit draw calls immediately without managing `BeginScene` / `EndScene` myself.
12. As an engine maintainer, I want `Renderer::beginScene` to fail fast when a `Scene Scope` is already active, so that an accidental nested scene can never silently discard batched geometry.
13. As an engine maintainer, I want each C++ `orc::Script` instance to keep its corresponding C# `Orc.Script` object alive via a normal (non-pinned) `GCHandle` allocated on creation and freed on destruction, so that the .NET garbage collector never collects live scripts and cleanup is deterministic.
14. As an engine maintainer, I want any failure to locate or load `scriptAssemblyPath`, any request to instantiate an unknown, abstract, or non-`Orc.Script` class, and any unhandled exception escaping a managed entry point to log the full error/exception text through the native `Logger` and abort via `ORC_FATAL`, so that C# scripting upholds the engine's `Fail-Fast Crash Contract` without boolean or null error returns.
15. As an engine maintainer, I want every engine call from C# to fail fast when made off the main thread, so that `async` / `await` continuations or `Task.Run` in scripts can never race the renderer or audio system.
16. As a C# game developer, I want to subclass an abstract `Orc.Script` class and override `OnAttach()`, `OnDetach()`, `OnUpdate(float deltaTime)`, `OnRender()`, and `OnDestroy()`, so that I can write frame-driven gameplay and rendering logic in idiomatic C# and deterministically dispose what my script created.
17. As a C# game developer, I want `Orc.Script.Context` (`Orc.ScriptContext`) to be valid already inside my script's constructor and to give scoped access to `Renderer`, `Input`, `Audio`, `Camera`, `TextureResources`, and `FontResources`, so that I can initialize fields in the constructor without global singletons (mirroring ADR 0004).
18. As a C# game developer, I want to call `Logger.Info(...)`, `Logger.Warning(...)`, and `Logger.Fatal(...)` from C# with my C# file, line, and member name appearing in the log, so that managed diagnostics route through the native `Logger` with accurate source locations; `Logger.Fatal` terminates the process like `ORC_FATAL`.
19. As a C# game developer, I want blittable value structs `Vector2f`, `Vector3f`, `Color`, and `FloatRect` with arithmetic operators and the geometric helpers that exist in C++ (`Position`, `Size`, `Intersects`), so that 2D math in C# allocates zero managed heap memory.
20. As a C# game developer, I want to query `Context.Input.IsKeyPressed(Key)`, `WasKeyJustPressed(Key)`, `WasKeyJustReleased(Key)`, and `Context.Input.SpecialKeys` (`Alt`, `Shift`, `System`, `Control`, `NumLock`, `CapsLock`), so that I can handle both continuous movement and single-frame keyboard edges in `OnUpdate`.
21. As a C# game developer, I want to query `Context.Input.IsMouseButtonPressed(MouseButton)`, `WasMouseButtonJustPressed(MouseButton)`, `WasMouseButtonJustReleased(MouseButton)`, `MousePosition`, `MouseDelta`, and `MouseScrollDelta`, so that I can build mouse-driven camera controls, aiming, and interactions in C#.
22. As a C# game developer, I want to trigger FMOD audio events via `Context.Audio.Play(eventPath)` and control mixer buses via `using Bus bus = Context.Audio.GetBus(busName)` (`Stop()`, `Pause()`, `Resume()`, `Volume`, `IsPaused`), so that C# gameplay scripts control sound effects and music buses.
23. As a C# game developer, I want to read and mutate the owning layer's `Camera` via `Context.Camera` (`Position`, `Rotation`, `Zoom`, `Move`, `Rotate`, `ApplyZoom`), so that scripts can pan, rotate, and zoom the active view while the engine keeps the camera viewport synchronized with the window (ADR 0007).
24. As a C# game developer, I want `Context.Renderer` to expose `SetClearColor(Color)`, `Draw(Sprite | Rectangle | Circle | Text)`, and `DrawLine(...)`, so that I can render 2D primitives, sprites, shapes, text, and lines from C# in natural `Submission Order`.
25. As a C# game developer, I want `Transformable` and `Texturable` base classes in C# exposing `Position`, `Rotation`, `Scale`, `Origin`, `Move(...)`, `Rotate(...)`, `ApplyScale(...)`, `LocalRect`, `GlobalRect`, `Texture`, and `TextureRect`, so that `Sprite`, `Rectangle`, `Circle`, and `Text` share the same ergonomic transform and bounds API as C++.
26. As a C# game developer, I want to construct, configure, and dispose `Rectangle` (`Size`, `FillColor`, `BorderColor`, `BorderThickness`) and `Circle` (`Radius`, `InnerRadius`, `FillColor`, `BorderColor`, `BorderThickness`) instances in C# and submit them to `Context.Renderer.Draw(...)`, so that I can render geometric 2D shapes from managed code.
27. As a C# game developer, I want to look up pre-loaded `Texture` and `Font` handles via `Context.TextureResources.GetResource(name)` and `Context.FontResources.GetResource(name)`, so that I can construct `Sprite` (`Color`, `Texture`, `TextureRect`) and `Text` (`Font`, `Color`, `TextString`, `LocalRect`, `GlobalRect`) instances in C# and draw them via `Context.Renderer.Draw(...)`.
28. As an engine maintainer, I want `Sprite`, `Rectangle`, `Circle`, `Text`, and `Bus` to implement `IDisposable` without finalizers, and the engine to fail fast at shutdown if any script or owning wrapper is still alive, so that native and OpenGL resources are only ever released on the main thread and leaks are caught instead of tolerated.
29. As an engine maintainer, I want `Menu` to attach a non-interactive `SmokeTestScript` from `TestProject.Scripts` that exercises every bound module and logs a lifecycle marker that `scripts/verify` requires, so that the 5-second smoke test validates the C# binding surface end-to-end on every run.
30. As a game developer, I want a `Window::close()` method that shuts the engine down gracefully through the normal `Event::WindowClosed` path, so that games can implement Quit buttons and automated runs can exit cleanly.
31. As an engine maintainer, I want the smoke test to end with a graceful shutdown (via `Window::close()`) and a clean exit code instead of a hard kill, so that `OnDetach`, `OnDestroy`, and the `verifyShutdown` leak check run on every verification.
32. As an engine maintainer, I want `CODING_STANDARDS.md` (formal C# coding conventions) and ADR 0012 created with the first ticket, and `GLOSSARY.md` / `AGENTS.md` updated by each ticket that introduces the corresponding concept, so that documentation reflects the C# scripting architecture before any C# code is written against it (AGENTS invariant #4).

## Implementation Decisions

### 1. Project & Build Topology
- **`OrcEngine.Scripting` (Engine Managed SDK)**:
  - Targets `net10.0` with `<AllowUnsafeBlocks>true</AllowUnsafeBlocks>`, `<EnableDynamicLoading>true</EnableDynamicLoading>`, `<Nullable>enable</Nullable>`, `<TreatWarningsAsErrors>true</TreatWarningsAsErrors>`, and `<EnforceCodeStyleInBuild>true</EnforceCodeStyleInBuild>`. The non-existent `<AllowUnsafe>` property is removed.
  - Declares `[assembly: DisableRuntimeMarshalling]`.
  - Public API types reside in `namespace Orc`; native interop types (`NativeApiTable`, `ManagedLifecycleTable`, `NativeBindings`, `ScriptRuntime`, interop structs) reside in `namespace Orc.Internal` with `internal` visibility. No `OrcEngine.ScriptingGlue` project is created.
  - Legacy bootstrap files `Class1.cs`, `Script.cs`, `ScriptHost.cs` (old `OrcEngine.Scripting` namespace) and the committed `OrcEngine.Scripting/.idea/` directory are deleted; `.idea/` is ignored in `.gitignore`.
- **`TestProject.Scripts` (Consumer Script Assembly)**:
  - Located under `TestProject/TestProject.Scripts/` targeting `net10.0`, referencing `OrcEngine.Scripting` via `<ProjectReference>`, with the same warning/style settings.
  - CMake builds `TestProject.Scripts` (and transitively `OrcEngine.Scripting`) into `$<TARGET_FILE_DIR:TestProject>` so `OrcEngine.Scripting.runtimeconfig.json`, `OrcEngine.Scripting.dll`, and `TestProject.Scripts.dll` sit next to the `TestProject` executable.
  - CMake fails configuration (`FATAL_ERROR`) when `DOTNET_EXECUTABLE` is not found, instead of silently skipping the managed build.
- **C# Style Enforcement**: A repository `.editorconfig` encodes the C# conventions (§7); `EnforceCodeStyleInBuild` + `TreatWarningsAsErrors` make the zero-warning build enforce them. `scripts/lint-conventions` remains C++-only.
- **`Config` & `LayerContext` Extensions**:
  - Add `FilePath scriptAssemblyPath` to `orc::Config` (empty by default; set to `"TestProject.Scripts.dll"` in `TestProject`). Relative paths resolve against the working directory, consistent with asset paths.
  - Pass `config.scriptAssemblyPath` to `Scripting` during `Engine` construction. An empty path means no consumer assembly; any later `addScript` then fails fast.
  - Add `Scripting& scripting` to `orc::LayerContext`.

### 2. Hosting & Assembly Loading (Single Default Load Context)
- `Scripting::initializeRuntime()` replaces `load_assembly_and_get_function_pointer` (which loads into an isolated `AssemblyLoadContext`) with `hdt_load_assembly` (loads `OrcEngine.Scripting.dll` into the default context) and `hdt_get_function_pointer` to resolve `Orc.Internal.ScriptRuntime.Initialize` (`UNMANAGEDCALLERSONLY_METHOD`). This supersedes the entry-point binding described in ADR 0008.
- `LoadScriptAssembly` loads the consumer via `AssemblyLoadContext.Default.LoadFromAssemblyPath(canonicalPath)`, so its reference to `OrcEngine.Scripting` binds to the already-loaded SDK.
- Script types are resolved with `m_scriptAssembly.GetType(typeName)` (never `Type.GetType`, which only searches the calling assembly and CoreLib); a missing, abstract, or non-`Orc.Script` type fails fast.
- A future hot-reload path can load the consumer into a collectible `AssemblyLoadContext` that falls back to the default context for the SDK; this is out of scope here.

### 3. Bidirectional Native ↔ Managed Interop Seam
- **Handshake**: `Orc.Internal.ScriptRuntime.Initialize(NativeApiTable* apiTable, ManagedLifecycleTable* outLifecycleTable)`. Both tables start with `uint32_t structSize`. C# fails fast if `apiTable->structSize != sizeof(NativeApiTable)`; C++ fails fast if the returned lifecycle table size does not match. C++ also `static_assert`s both table sizes.
- **Glue Location**: Native glue functions live in private `OrcEngine/source/Orc/Script/` sources as `static` functions in an anonymous namespace (C++ linkage, no `extern "C"`, avoiding MSVC C4190), assigned into `NativeApiTable`.
- **ABI Rules**:
  - `glm` vectors and `orc::Color` are never passed or returned by value across the seam (they have user-defined constructors and are returned via hidden pointer on MSVC x64). A private interop header defines POD mirror structs—`InteropVector2f { float x, y; }`, `InteropVector3f`, `InteropColor { uint8_t r, g, b, a; }`, `InteropFloatRect`, `InteropSpecialKeys { bool alt, shift, system, control, numLock, capsLock; }`—each with `static_assert`s on `sizeof`, `std::is_trivial`, and `std::is_standard_layout`, converted to/from engine types at the boundary. C# mirrors them as `[StructLayout(LayoutKind.Sequential)]` structs.
  - `bool` crosses as 1 byte (`DisableRuntimeMarshalling` on the managed side, C++ `bool` on the native side). Enums cross as `int32_t`.
  - Strings cross as UTF-8 null-terminated `const char*`; C# uses `stackalloc` buffers for short strings with a heap fallback for long ones.
- **Fail-Fast Routing**:
  - `NativeApiTable` includes `fatal(const char* message)`, implemented as `ORC_FATAL("{}", message)`.
  - Every `[UnmanagedCallersOnly]` entry point wraps its body in `try { ... } catch (Exception exception) { NativeBindings.Fatal(exception.ToString()); }`.
  - No boolean or null error returns: `loadScriptAssembly` returns `void`; `createScript` never returns null.
- **Main-Thread Contract**: `ScriptRuntime` records `Environment.CurrentManagedThreadId` at `Initialize`; every `NativeBindings` call fails fast when invoked from any other thread.
- **`NativeApiTable` Contents** (grown per ticket; handles are typed native pointers):
  - **Core**: `structSize`, `fatal(const char*)`, `log(int32_t level, const char* file, int32_t line, const char* function, const char* message)`.
  - **Renderer**: `setClearColor(Renderer*, InteropColor)`, `drawSprite(Renderer*, const Sprite*)`, `drawRectangle(Renderer*, const Rectangle*)`, `drawCircle(Renderer*, const Circle*)`, `drawText(Renderer*, const Text*)`, `drawLine2D(Renderer*, InteropVector2f, InteropVector2f, InteropColor, float)`, `drawLine3D(Renderer*, InteropVector3f, InteropVector3f, InteropColor, float)`. `clear`, `setViewport`, `beginScene`, `endScene`, and `flush` are intentionally **not** exposed (see §4 and Out of Scope).
  - **Input**: `isKeyPressed(const Input*, int32_t)`, `wasKeyJustPressed(...)`, `wasKeyJustReleased(...)`, `getSpecialKeys(const Input*) -> InteropSpecialKeys`, `isMouseButtonPressed(const Input*, int32_t)`, `wasMouseButtonJustPressed(...)`, `wasMouseButtonJustReleased(...)`, `getMousePosition(const Input*) -> InteropVector2f`, `getMouseDelta(...)`, `getMouseScrollDelta(...)`.
  - **Audio & Bus**: `audioPlay(Audio*, const char*)`, `audioGetBus(Audio*, const char*) -> Bus*` (released from the returned `UniquePtr<Bus>`), `busDestroy(Bus*)`, `busStop(Bus*)`, `busPause(Bus*)`, `busResume(Bus*)`, `busGetVolume(const Bus*)`, `busSetVolume(Bus*, float)`, `busIsPaused(const Bus*)`.
  - **Resources**: `textureResourcesGet(const TextureResources*, const char*) -> const Ref<Texture>*`, `fontResourcesGet(const FontResources*, const char*) -> const Ref<Font>*`, `textureGetSize(const Ref<Texture>*) -> InteropVector2f`.
  - **Camera** (owning layer's camera only): `cameraSetZoom`, `cameraSetRotation`, `cameraSetPosition(Camera*, InteropVector2f)`, `cameraZoom`, `cameraRotate`, `cameraMove(Camera*, InteropVector2f)`, `cameraGetZoom`, `cameraGetRotation`, `cameraGetPosition`. No create, destroy, or `setViewportSize` (ADR 0007 owns viewport synchronization).
  - **Transformable** (via `Transformable*`): get/set `Position`, `Rotation`, `Scale`, `Origin`; `move`, `rotate`, `scale` (apply); `transformableDestroy(Transformable*)` (virtual destructor) releases every drawable type.
  - **Texturable** (via `Texturable*`): `texturableSetTexture(Texturable*, const Ref<Texture>*)`, `texturableSetTextureRect`, `texturableGetTextureRect`, `texturableGetLocalRect`, `texturableGetGlobalRect`.
  - **Sprite**: `spriteCreate(InteropDrawableHandles* out)`, `spriteCreateWithTexture(const Ref<Texture>*, InteropVector2f, InteropDrawableHandles* out)`, `spriteSetColor`, `spriteGetColor`.
  - **Rectangle**: `rectangleCreate(InteropVector2f position, InteropVector2f size, InteropColor fillColor, InteropDrawableHandles* out)`, get/set `FillColor`, `BorderColor`, `Size`, `BorderThickness`.
  - **Circle**: `circleCreate(InteropVector2f position, float radius, InteropColor fillColor, InteropDrawableHandles* out)`, get/set `FillColor`, `BorderColor`, `Radius`, `InnerRadius`, `BorderThickness`.
  - **Text**: `textCreate(const Ref<Font>*, const char*, InteropDrawableHandles* out)`, `textSetFont(Text*, const Ref<Font>*)`, `textSetColor`, `textGetColor`, `textSetString(Text*, const char*)`, `textGetLocalRect`, `textGetGlobalRect`.
- **Polymorphic Handles**: `InteropDrawableHandles { void* object; Transformable* transformable; Texturable* texturable; }` is filled by each create function using real C++ upcasts (`texturable` is null for `Text`). C# uses `object` for type-specific calls and the base pointers for shared `Transformable` / `Texturable` calls and destruction. C++ never casts `void*` directly to a base class.
- **Resource Handles (`const Ref<T>*` into the Resource Map)**: `Resources<T>::getResource` returns a reference to the `Ref<T>` stored in its `unordered_map` node; the node address is stable (rehashing never relocates elements) and valid until `Resources::clear()`, which runs only after the shutdown leak check. Glue functions copy that `Ref<T>` into `Sprite` / `Rectangle` / `Circle` / `Text`, giving correct shared ownership with **no changes to `Texture` / `Font`** and no `std::enable_shared_from_this`. C# `Texture` / `Font` are non-owning, non-disposable handles; C# drawables cache the managed `Texture` / `Font` and the `TextString` they were given, so those getters never cross the seam.
- **`ManagedLifecycleTable`** (`[UnmanagedCallersOnly]` entry points returned to C++):
  - `structSize`
  - `loadScriptAssembly(const char* assemblyPath)`
  - `createScript(const char* typeName, const NativeScriptContext* context) -> void*` (normal `GCHandle`, never null)
  - `destroyScript(void* scriptHandle)` (calls `OnDestroy()`, then frees the handle)
  - `onAttach(void*)`, `onDetach(void*)`, `onUpdate(void*, float deltaTime)`, `onRender(void*)`
  - `verifyShutdown()` (fails fast listing type names and counts if any script or owning wrapper is still alive)
- **`NativeScriptContext`**: `Renderer*`, `Input*`, `Audio*`, `Camera*` (owning layer's `m_camera`), `const TextureResources*`, `const FontResources*`.

### 4. `orc::Script`, `orc::GameLayer` & Engine Lifecycle Integration
- **`orc::Script` Wrapper**: move-only C++ class (`Script(const Script&) = delete`, `Script(Script&&) noexcept`, `~Script()`) holding `Scripting*` and the `void*` handle; destruction calls `destroyScript` when the handle is non-null; moved-from instances hold null. Exposes `onAttach()`, `onDetach()`, `onUpdate(float deltaTime)`, `onRender()`.
- **Context Before Constructor**: `ScriptRuntime` stores the new `ScriptContext` in a `[ThreadStatic]` pending slot immediately before `Activator.CreateInstance` and clears it afterward. The protected `Orc.Script()` constructor consumes the pending slot and fails fast if it is empty, so `Context` is non-null inside derived constructors and scripts cannot be constructed with `new` outside the engine.
- **`Orc.Script`**: abstract; virtual `OnAttach()`, `OnDetach()`, `OnUpdate(float deltaTime)`, `OnRender()`, `OnDestroy()`; `protected ScriptContext Context { get; }`.
- **`orc::GameLayer` Script Management**:
  - Holds `std::vector<Script> m_scripts;` and `bool m_isAttached = false;`.
  - `void addScript(std::string_view typeName)` creates a `Script` via `m_layerContext.scripting.createScript(typeName, m_layerContext, m_camera)` and appends it; if already attached, it also calls `script.onAttach()`. A plain `GameLayer` (no subclass) can host scripts.
  - Engine-invoked hooks (public, documented as engine-only): `attachScripts()` (sets `m_isAttached`, calls `onAttach`), `detachScripts()` (calls `onDetach`, clears `m_isAttached`), `updateScripts(float deltaTime)`, `renderScripts()`.
  - `renderScripts()`: if `!m_scripts.empty()`, calls `renderer.beginScene(m_camera)`, every `script.onRender()`, then `renderer.endScene()`. Script output is therefore submitted after (drawn over) the C++ layer's `onRender()` output.
- **`Renderer::beginScene`** gains `ORC_FATAL_CHECK(!m_isSceneActive, ...)` so nested scenes fail fast for C++ and C# alike.
- **Call Sites**:
  - `GameLayerManager::setActiveLayer`: outgoing `detachScripts()` → `onDetach()`; incoming `syncCameraWithWindow()` → `onAttach()` → `attachScripts()`.
  - `GameLayerManager::clear`: `detachScripts()` → `onDetach()`.
  - `Engine::run`: copies the active layer `Ref<GameLayer>` by value (so a layer switch inside `onUpdate` cannot redirect script dispatch mid-frame), then `onUpdate(elapsed)` → `updateScripts(elapsed)` → `onRender()` → `renderScripts()`.
  - `Engine::~Engine`: `m_gameLayerManager->clear()` (detaches and destroys layers, which destroys scripts and runs `OnDestroy`) → `m_scripting->verifyShutdown()` → resource `clear()` → remaining subsystem teardown → `m_scripting.reset()` last.

### 5. Native Wrapper Ownership
- **Owning (`IDisposable`, no finalizer)**: `Sprite`, `Rectangle`, `Circle`, `Text`, `Bus`. `Dispose()` is idempotent and releases the native object on the calling (main) thread; any use after `Dispose()` throws `ObjectDisposedException`, which the entry-point boundary turns into a fatal crash.
- **Live-Wrapper Registry**: `ScriptRuntime` counts live owning wrappers per type (incremented on create, decremented on `Dispose`) and live scripts. A wrapper garbage-collected without `Dispose()` stays counted, so `verifyShutdown()` reports it.
- **Non-Owning, Non-Disposable**: `Renderer`, `Input`, `Audio`, `Camera` (`Context.Camera`), `TextureResources`, `FontResources`, `Texture`, `Font`.

### 6. Logger Binding
- C++ gains a non-template `Logger` overload that accepts raw `file`, `line`, and `function` values (since `std::source_location` cannot be constructed from runtime data) and forwards to the existing spdlog seam.
- `Orc.Logger.Info(string message, [CallerFilePath] ..., [CallerLineNumber] ..., [CallerMemberName] ...)` and `Logger.Warning(...)` log at the corresponding level with the C# location.
- `Orc.Logger.Fatal(...)` (`[DoesNotReturn]`) logs at `Fatal` level and terminates via `fatal`, equivalent to `ORC_FATAL`.

### 7. C# Coding Conventions (`CODING_STANDARDS.md`)
- **Indentation & Braces**: Tabs for indentation and Allman braces (matching C++); file-scoped namespaces.
- **Naming**: `PascalCase` for namespaces (`Orc`), classes, structs, interfaces, enums, enum members, public/protected methods, and properties; `m_camelCase` for private/protected instance fields; `s_camelCase` for private static fields; `camelCase` for parameters and locals; `UPPER_SNAKE_CASE` for `const` constants.
- **Properties vs. Methods**: State getters/setters are C# properties (`Position`, `Rotation`, `Scale`, `Origin`, `Color`, `FillColor`, `BorderColor`, `BorderThickness`, `Size`, `Radius`, `InnerRadius`, `Texture`, `TextureRect`, `Font`, `TextString`, `LocalRect`, `GlobalRect`, `Zoom`, `Volume`, `IsPaused`, `MousePosition`, `MouseDelta`, `MouseScrollDelta`, `SpecialKeys`, `Width`, `Height`); operations and parameterized queries are methods (`Move`, `Rotate`, `ApplyScale`, `ApplyZoom`, `Draw`, `DrawLine`, `SetClearColor`, `Play`, `GetBus`, `Stop`, `Pause`, `Resume`, `GetResource`, `IsKeyPressed`, `WasKeyJustPressed`, `WasKeyJustReleased`, `IsMouseButtonPressed`, `WasMouseButtonJustPressed`, `WasMouseButtonJustReleased`).
- **Interop Rules**: No finalizers on native wrappers; owning wrappers are disposed deterministically (`using` or `OnDestroy`); no engine calls from background threads or `async` continuations; interop structs only in `Orc.Internal`.
- Encoded in `.editorconfig` and enforced by the build (§1).

### 8. Graceful Shutdown (`Window::close`) & Smoke-Test Exit
- **`Window::close()`**: a new public method that dispatches `Event::WindowClosed` through the window's event callback, the same path the GLFW close callback uses. `Engine::onEvent` then clears `m_running`, the current frame finishes, and `Engine::~Engine` runs the full teardown sequence (§4). `Window::close()` is C++ only; `Window` stays unexposed to C#.
- **`TestProject` Timed Exit**: `Menu` reads the environment variable `ORC_SMOKE_TEST_SECONDS` once at construction. When it is set to a positive number, `Menu::onUpdate` accumulates elapsed time and calls `m_layerContext.window.close()` once that duration is reached. When it is unset, behavior is unchanged.
- **`run-demo` Contract**: when `-TimeoutSeconds` / `--timeout` is positive, `run-demo` sets `ORC_SMOKE_TEST_SECONDS` to `max(1, timeout - 2)` and requires the process to exit **by itself** with code `0`. Reaching the hard-kill timeout is a failure (the kill remains only as a backstop against hangs). Because `ORC_FATAL` ends in `std::abort()`, any fail-fast during teardown (including `verifyShutdown`) yields a non-zero exit code and fails the gate.

## Testing Decisions

- **What Makes a Good Test**:
  - Tests validate external behavior across the public C++ ↔ C# boundary (`GameLayer::addScript`, `Orc.Script` lifecycle hooks, `ScriptContext` subsystem queries and mutations, rendered output, and fail-fast shutdown checks in `TestProject`), never asserting on private function pointer table layouts or internal `GCHandle` structures.
- **Primary Testing Seam (Smoke Test via `Menu`)**:
  - `Menu` attaches `SmokeTestScript : Orc.Script` (in `TestProject.Scripts`) in its constructor. Each ticket extends it non-interactively to exercise the modules bound so far on every verify run: logging, creating/transforming/drawing/disposing drawables in a small corner region (so `Menu` stays usable), mutating and restoring `Context.Camera`, querying `Input` state (no key presses required), resource lookups, and `GetBus` + `Volume` reads (no audible playback).
  - After its first `OnRender()`, `SmokeTestScript` logs the marker `SmokeTestScript: lifecycle OK` once.
  - `scripts/run-demo.ps1` / `run-demo.sh` analyze the **entire log section appended by the current run** (using the already-recorded initial log size) instead of only the last 20 lines, and accept one or more `-ExpectLogPattern` / `--expect-log-pattern` values. `scripts/verify` requires every marker, so a script or check that silently stops running fails the gate.
  - **Required markers** (added by the ticket that introduces them): `Scripting: managed runtime initialized` (logged from C# through `Orc.Logger` during the handshake), `SmokeTestScript: lifecycle OK`, and `Scripting: shutdown verified` (logged by a successful `verifyShutdown`).
- **Interactive Showcase**: The existing `ScriptTest` layer (reached via `Menu` navigation, not a new `ScriptingTest` layer) attaches `ScriptTestScript`, the manual showcase for keyboard/mouse camera control, audio playback on key press, sprites, text, shapes, and lines.
- **Teardown Coverage**: The smoke run ends through `Window::close()` (§8), so `OnDetach`, `OnDestroy`, and `verifyShutdown` execute on every verification, and the run must exit with code `0`.
- **Modules to Be Tested & Verified**:
  - `Scripting` & `orc::Script`: single-load-context assembly loading, table size validation, `GCHandle` creation/destruction, fail-fast checks, and `verifyShutdown`.
  - `GameLayer`, `GameLayerManager` & `Engine`: automatic `attachScripts`, `updateScripts`, `renderScripts` (with automatic `Scene Scope`), and `detachScripts`.
  - `Orc` C# SDK modules: `Logger`, `Renderer`, `Camera`, `Sprite`, `Rectangle`, `Circle`, `Text`, `Input`, `Audio`, `Bus`, `TextureResources`, and `FontResources`.
- **Prior Art in the Codebase**:
  - Unified verification gate (`.\scripts\verify.ps1` / `./scripts/verify.sh`): convention linter, zero-warning build, and 5-second `TestProject` smoke test (`run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog`).

## Out of Scope

- Creating a separate `OrcEngine.ScriptingGlue` project (glue stays `internal` inside `OrcEngine.Scripting`).
- Moving `GameLayer` or `GameLayerManager` ownership entirely to C# or removing C++ `GameLayer`s.
- Introducing an `Entity` / `Node` / ECS component hierarchy before a concrete game repository requires one.
- C#-controlled scene management: `Renderer.Clear`, `SetViewport`, `BeginScene`, `EndScene`, `Flush`, and standalone owning `Camera` instances (revisit when a game needs custom-camera passes; would require dropping or extending the automatic `Scene Scope`).
- Forwarding `onEvent` or `onGuiRender` to scripts.
- `Vector2i`, `Vector2u`, `Vector4f`, `IntRect`, `UIntRect`, and `Rect.Contains` (no exposed API uses them; `UIntRect` and `contains` do not exist in C++).
- Hot reload / unloading of the consumer script assembly.
- Exposing `Random`, `Clock`, `GameLayerManager`, `Event`, `Window`, `Animator` / `Animation` / `AnimationResources`, low-level GPU primitives (`VertexArray`, `VertexBuffer`, `IndexBuffer`, `BufferLayout`, `Shader`), or Dear ImGui (`Gui`) to C#.
- Replacing XML resource manifests (`fonts.xml`, `textures.xml`, `animations.xml`, `shaders.xml`) with the directory-scanning `ResourceLoader` (see **Further Notes**).

## Resolved Questions

1. **Teardown coverage in the smoke test.** `run-demo` used to kill `TestProject` at the timeout, so `Engine::~Engine` never ran. **Resolved:** add `Window::close()` (a must-have engine feature) and end smoke runs gracefully through it (§8).

## Further Notes

### Future Architecture: Directory-Scanning `ResourceLoader` & `ResourceHolder` Split
Currently, `OrcEngine` loads resources at `Engine` startup from XML manifests (`assets/fonts.xml`, `assets/textures.xml`, `assets/animations.xml`, `assets/shaders.xml`) via `ResourceHolder`. While this spec exposes read-only `TextureResources` and `FontResources` lookups to C# so `Sprite` and `Text` work immediately alongside existing C++ `TestProject` layers, the planned next evolution of resource management is:
1. **Eliminate XML Manifests via Convention-Over-Configuration Directory Scanning**:
   - Games built on `OrcEngine` have a small, bounded asset footprint where loading all assets upfront into memory is fast and desirable, and maintaining XML files for every added texture or font creates unnecessary friction.
   - The on-disk `assets/` directory will follow a predefined folder convention (e.g., `assets/textures/`, `assets/fonts/`, `assets/shaders/`, `assets/audio/`).
   - Logical resource keys will be derived automatically from file paths/stems relative to each category directory (for example, `assets/textures/characters/soldier.png` → `"characters/soldier"` or `"soldier"`).
2. **Split `ResourceHolder` into `ResourceLoader` and `ResourceHolder`**:
   - **`ResourceLoader`**: Responsible for iterating the predefined asset directories on disk, discovering supported file extensions (`.png`/`.jpg` for textures, `.ttf`/`.otf` for fonts, etc.), invoking the native decoders/GPU uploaders, and populating `ResourceHolder`.
   - **`ResourceHolder` (Resource Pool / Registry)**: Responsible purely for storing loaded `Ref<T>` instances in heterogeneous string-keyed maps (`StringViewHash`), serving fast $O(1)$ lookups to C++ and C#, and guaranteeing deterministic teardown before the OpenGL context and FMOD system shut down. It must preserve the address stability of stored `Ref<T>` values, which the C# resource handles rely on (§3).
3. **C++ vs. C# Division of Labor for Resource Loading**:
   - Because OpenGL (`glad`), `stb_image`, FreeType, and FMOD are linked `PRIVATE` to the native `OrcEngine` static library, actual file decoding and GPU/driver resource creation (`Texture::loadFromFile`, `Font::loadFromFile`) must execute in C++.
   - `ResourceLoader` can be triggered from the initial `GameLayer` (or exposed to C# so a script/layer can invoke `ResourceLoader.LoadAll()` at startup), keeping GPU allocation and memory pooling in C++ while allowing C# scripts to drive asset lifecycle and lookup without C++ `GameLayer`s manually wiring individual textures or sounds.

### Documentation & ADR Deliverables
- **Ticket 01**: `CODING_STANDARDS.md` C# Coding Standards section (§7, removing the "will be formally defined" placeholder); ADR 0012 created; ADR 0008 annotated; `AGENTS.md` C# toolchain, `Scripting` loading the script assembly in the initialization order, and the ADR 0012 pointer.
- **Ticket 02**: `GLOSSARY.md` **Script**, **ScriptContext**, and **Native Wrapper**; `AGENTS.md` `LayerContext` contents, corrected frame loop (C++ layers bracket their own `onRender()`; `GameLayer::renderScripts()` brackets script rendering), and graceful shutdown via `Window::close()`; ADR 0012 extended with the lifecycle, Scene Scope, ownership, and shutdown decisions.
- **Tickets 03–05**: update ADR 0012 and `GLOSSARY.md` whenever a slice refines a decision or term; ticket 05 performs the final sync.

Content summary:
- `docs/adr/0012-csharp-module-bindings-and-script-lifecycle.md`: single SDK assembly + consumer assembly in one default load context; size-validated function pointer tables with POD interop structs and `DisableRuntimeMarshalling`; fail-fast exception routing and the main-thread contract; engine-driven `std::vector<Script>` lifecycle with the automatic `Scene Scope` (and why C# scene control was rejected); `IDisposable`-without-finalizer ownership with fail-fast leak detection; `const Ref<T>*` resource handles; ambient context injection before script construction.
- `docs/adr/0008-hostfxr-dotnet-coreclr-scripting.md`: annotate that its entry-point binding (`load_assembly_and_get_function_pointer`) is superseded by ADR 0012.
