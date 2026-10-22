# AGENTS.md

> [!IMPORTANT]
> ### Mandatory Agent Invariants
> Every agent session and subagent working in this repository **must** strictly abide by the following invariants:
> 1. **Read Core Guidance First**: On session startup, read `AGENTS.md` and `GLOSSARY.md` before executing any task or modifying code.
> 2. **Script-First Toolchain Primacy**: Always use `.\scripts\build.ps1` (Windows) or `./scripts/build.sh` (Linux) as the primary build command. The scripts automatically discover and activate MSVC/Ninja toolchains and validate environment configurations. Raw `cmake` commands are only a documented fallback.
> 3. **Unified Verification Gate**: Before completing any turn, resolving a ticket, or committing work, run the unified verification gate `.\scripts\verify.ps1` (Windows) or `./scripts/verify.sh` (Linux). The verification gate runs conventions linting, compiles the target build preset, and executes the 5-second smoke test (`TestProject`). All stages must pass with 100% success (zero errors).
> 4. **Synchronize Documentation as Ticket Condition**: Any changes touching engine architecture, lifecycle, subsystems, or terminology **must** update `GLOSSARY.md`, record an ADR under `docs/adr/` if design trade-offs were made, and update the corresponding subsystem sections in `AGENTS.md`. No ticket is considered resolved until documentation is in sync.

---

## 1. Project Overview & Mission

OrcEngine is a modular, high-performance 2D game engine built in C++20 using OpenGL 4.6 (Core Profile), GLFW, GLAD, FreeType, FMOD Studio, spdlog, and GLM. It is compiled as a static library (`OrcEngine`), with `TestProject` serving as the demo application, feature validation suite, and integration harness.

### Repository Layout
- **Public Engine Headers**: `OrcEngine/include/Orc/` (umbrella entry point: `OrcEngine/include/Orc/Orc.hpp`)
- **Engine Source**: `OrcEngine/source/Orc/`
- **Demo / Harness Headers & Sources**: `TestProject/include/` and `TestProject/source/`
- **Demo Runtime Assets & Shaders**: `TestProject/assets/` and `TestProject/shaders/`
- **Domain Documentation**: `GLOSSARY.md` (ubiquitous language), `docs/adr/` (architectural decision records), `docs/agents/` (agent operational specs and protocols)
- **Local Issue Tracker & Specs**: `.scratch/`
- **Automation Scripts**: `scripts/` (environment check, build runner, conventions linter, demo runner, ticket CLI)
- **Build System**: `CMakeLists.txt`, `CMakePresets.json`, `cmake/`, `vcpkg.json`

Keep reusable functionality in `OrcEngine`; use `TestProject` for examples, experiments, and integration coverage.

---

## 2. Project Direction

The high-level trajectory of the project is:

```
Refactor architecture a bit
        │
        ▼
Refactor codebase (especially rendering)
        │
        ▼
Add C# scripting
        │
        ▼
Start building with game engine to release a game (in a separate game repository)
and let the engine grow naturally driven by real game requirements
```

> [!NOTE]
> Games developed using OrcEngine reside in their own separate repositories. Reusable engine features belong in `OrcEngine`, and validation/prototypes belong in `TestProject`.

---

## 3. Agent Behavior & Consultation Protocol

In addition to the [Mandatory Agent Invariants](#mandatory-agent-invariants) at the top of this document, all autonomous agents must strictly adhere to the following consultation principles to prevent misaligned changes, wasted effort, and premature assumptions:

1. **Zero Unilateral Assumptions**:
   - Never assume architectural designs, implementation choices, library selections, or feature details without explicit user direction or confirmation.
   - Do not invent elaborate sub-phases or predetermined technical choices (e.g. picking specific C# runtimes or redesigning whole pipelines) without consulting the user first.
2. **Consult & Propose First ("Ask / Propose Options")**:
   - When an agent sees an opportunity, identifies a bug, or considers multiple implementation approaches, it must present clear options with trade-offs and **ask the user** before writing code.
3. **Incremental Execution**:
   - Work slowly and iteratively: implement one focused change or script, test and verify it thoroughly, obtain user confirmation, and only then proceed to the next step.
   - Avoid dumping large batches of files, scripts, or refactors all at once.
4. **Preserve Scope**:
   - Keep changes tightly scoped to the immediate task requested by the user. Avoid drive-by refactoring or unrelated cleanup.
5. **Documentation Synchronization**:
   - In accordance with the [Mandatory Agent Invariants](#mandatory-agent-invariants), synchronize `GLOSSARY.md`, architectural records (`docs/adr/`), and `AGENTS.md` subsystem documentation alongside every code change as an integral condition of ticket resolution.

---

## 4. Subsystem Architecture & Lifecycle

OrcEngine is organized into six core subsystems (**Engine**, **Graphics & Renderer**, **Audio**, **Input & Events**, **GUI**, and **Resources**) built upon the foundational **Core** primitives module.

```
                              ┌────────────────────────────────────────┐
                              │            Engine Lifecycle            │
                              │ (Window, Audio, Renderer, Gui, Input,  │
                              │  GameLayerManager, Resources, Context) │
                              └───────────────────┬────────────────────┘
                                                  │
                                                  ▼
                                      Main Loop: Engine::run()
                                                  │
         ┌────────────────────────────────────────┴────────────────────────────────────────┐
         │ 1. Clock::restart() -> elapsed delta time                                       │
         │ 2. Renderer::clear()                                                            │
         │ 3. GameLayerManager::getActiveLayer()                                           │
         │ 4. activeLayer->onUpdate(elapsed)                                               │
         │ 5. activeLayer->onRender() [inside layer: beginScene(m_camera) -> endScene()]   │
         │ 6. Gui::begin() -> activeLayer->onGuiRender() -> Gui::end()                     │
         │ 7. Audio::update()                                                              │
         │ 8. Window::display() -> glfwPollEvents() & glfwSwapBuffers()                    │
         └─────────────────────────────────────────────────────────────────────────────────┘
```

### 4.1 Engine Coordination & Initialization Order
`Engine` is the top-level coordinator. It constructs and initializes subsystems in strict sequential order:
1. `Logger::init(config.logPath)`: Sets up file and console logging via spdlog.
2. `GameLayerManager`: Manages layer registration and active layer pointer.
3. `Window`: Initializes GLFW, creates the OpenGL 4.6 context, loads GLAD function pointers, and configures event callbacks.
4. `Renderer`: Unified dynamic quad batcher, vertex/index buffers, and composite shader. Initialized with explicit `setViewport` from window physical framebuffer bounds.
5. `Gui`: ImGui GLFW and OpenGL 3 backend context initialized with a `Window&` reference.
6. `Audio`: FMOD Studio system, master/music/sfx buses, and pre-loads configured banks.
7. `Input`: Event-driven keyboard, mouse button, and cursor position state tracker.
8. `Resources` caches: Pre-loads XML asset manifests (`fontsPath`, `texturesPath`, `animationsPath`). `Font::loadFromFile` manages its own local `FT_Library` and `FT_Face` lifecycle while building the glyph bitmap atlas.
9. `LayerContext`: Bundles references to persistent subsystems (`window`, `renderer`, `audio`, `gameLayerManager`, `fontResources`, `textureResources`, `animationResources`, `input`) for constructor injection into game layers and application bootstrap via `Engine::getLayerContext()`.

### 4.2 Game Layers & Layer Management
The layer runtime and context injection model is detailed in [ADR 0004](docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md) and [ADR 0007](docs/adr/0007-centralize-viewport-and-synchronize-camera.md).
- `GameLayerManager` stores registered layers by name in a hash map and maintains a pointer to the currently active layer. Switching layers (`setActiveLayer(name)`) invokes `onDetach()` on the outgoing layer, automatically synchronizes the incoming layer's camera viewport to current window bounds via `syncCameraWithWindow()`, and then invokes `onAttach()`.
- `GameLayer` requires an explicit `LayerContext&` via constructor reference injection (`explicit GameLayer(LayerContext& layerContext)`), storing `m_layerContext` and initializing its `m_camera` from `m_layerContext.window` dimensions.
- `GameLayer` provides virtual lifecycle hooks:
  - `onAttach()`: Invoked when the layer becomes active (e.g., setting window title, setting up scene state).
  - `onDetach()`: Invoked when transitioning away from the layer.
  - `onUpdate(float deltaTime)`: Per-frame game logic and frame delta time, accessing input and subsystems through `m_layerContext`.
  - `onRender()`: Submits 2D primitives to `m_layerContext.renderer` within an explicit **Scene Scope** (`m_layerContext.renderer.beginScene(m_camera)` ... `m_layerContext.renderer.endScene()`).
  - `onGuiRender()`: Submits ImGui controls enclosed between `Gui::begin()` and `Gui::end()`.
  - `onEvent(const Event& event)`: Handles dispatched input and window events. Base `GameLayer::onEvent` automatically updates `m_camera` viewport dimensions on `WindowResizedEvent`.
  - `syncCameraWithWindow()`: Authoritatively aligns `m_camera` viewport dimensions with `m_layerContext.window.getWidth()` and `getHeight()`.

### 4.3 Window, Input, and Events Flow
The polymorphic event hierarchy and downcasting architecture is detailed in [ADR 0005](docs/adr/0005-revert-to-polymorphic-event-hierarchy.md).
- **Event Generation**: GLFW window callbacks (keys, mouse buttons, cursor move, scroll, window resize, window close) trigger lambda dispatchers configured in `Window::setCallbacks()`. `Window` tracks physical framebuffer dimensions via `glfwGetFramebufferSize` (`getFramebufferWidth()`, `getFramebufferHeight()`, `getFramebufferSize()`) alongside logical dimensions, eliminating direct raw `glViewport` calls from window callbacks.
- **Event Representation**: `Event` is an open polymorphic base class storing an `Event::Type` enum (`m_type`), initialized via an `explicit Event(Type type)` constructor and cleaned up via a virtual destructor. Concrete event structs inherit publicly from `Event`:
  - `WindowResizedEvent`, `WindowClosedEvent`
  - `KeyboardKeyPressedEvent`, `KeyboardKeyReleasedEvent`
  - `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, `MouseMovedEvent`, `MouseWheelScrolledEvent`
- **Inspection & Downcasting**:
  - Type query: `event.getType()` returning `Event::Type`.
  - Type downcasting: `getEvent<T>(event)` returns `static_cast<const T&>(event)`, with an `ORC_ASSERT` verifying the runtime downcast (`dynamic_cast`) in debug builds.
  - Pattern matching: Standard C++ `switch (event.getType())` or `if (event.getType() == Event::Type::...)` statements.
- **Event Propagation**: `Window` invokes its `EventCallback` which routes to `Engine::onEvent()`. First, `Input::onEvent(event)` updates the internal input state snapshot. Upon `WindowResizedEvent`, `Engine` updates `Renderer::setViewport` with the window's physical framebuffer dimensions. Next, the active `GameLayer` processes the event via `gameLayer->onEvent(event)` (updating `m_camera` viewport size on `WindowResizedEvent`). Afterwards, `Engine` processes engine-level hooks (setting `m_running = false` on `WindowClosedEvent`).
- **Input Subsystem**: `Input` tracks pressed keys (`isKeyPressed(Keyboard::Key)`), mouse buttons (`isMouseButtonPressed(Mouse::Button)`), and cursor coordinates (`getMousePosition()`) purely from window events without polling native GLFW window handles. Layers query input via `m_layerContext.input`.

### 4.4 Graphics & Rendering Subsystem

The rendering architecture is detailed in [ADR 0001](docs/adr/0001-collapse-2d-render-submission-pipeline.md) and [ADR 0002](docs/adr/0002-unify-vertex-and-untemplate-texturable.md).

- **Renderer Model**: 2D dynamic batch renderer using OpenGL 4.6 (Core Profile).
- **Authoritative Viewport Management**: Viewport rasterization bounds are centrally encapsulated in `Renderer::setViewport(width, height)` and `setViewport(x, y, width, height)`, issuing typed `glViewport` commands and eliminating scattered OpenGL calls in platform windowing loops.
- **Scene Scope**: Rendering occurs within an explicit submission scope bounded by `renderer.beginScene(camera)` and `renderer.endScene()`. `beginScene` binds the camera's view-projection matrix to `m_quadShader` and initializes batch counters; `endScene` flushes any remaining queued geometry.
- **Quad Batcher**: All 2D geometry (sprites, rectangles, procedural circles, font glyphs, and lines) is aggregated into a single dynamic vertex and index buffer inside `Renderer`:
  - Static index buffer: Pre-allocated for `MAX_QUADS = 2000` quads (12,000 indices) following the repeating `[0, 1, 2, 2, 3, 0]` pattern.
  - Dynamic vertex buffer: Holds up to `MAX_QUAD_VERTICES = 8000` canonical `Vertex` instances, updated via `glBufferSubData`.
- **Unified Vertex Layout**: A single canonical struct `orc::Vertex` is used for all batched primitives:
  - `position` (`Vector3f`): World-space vertex coordinate.
  - `color` (`Vector4f`): Primary RGBA color / tint.
  - `secondaryColor` (`Vector4f`): Secondary color (e.g. circle inner color, border color).
  - `textureCoord` (`Vector2f`): UV texture coordinates.
  - `localPosition` (`Vector2f`): Coordinate normalized relative to the quad for procedural shader effects (circles, rounded borders).
  - `textureIndex` (`float`): Texture slot unit (`0.0f..31.0f`), or `32.0f` (`EMPTY_TEXTURE_SLOT`) for untextured geometry.
  - Procedural parameters: `shapeType` (`0.0f` = standard quad, `1.0f` = procedural circle), `borderThickness`, `innerRadius`, and `radius`.
- **Texturable & Geometry Submission**:
  - `Texturable` is a non-templated `Transformable` base class holding four `Vertex` instances directly in `m_vertices`.
  - High-level primitives (`Sprite`, `Rectangle`, `Circle`) inherit from `Texturable`, submitting geometry directly to the quad batcher via `submitQuad(vertices, texture)` using a cache-friendly single-pass copy and texture-slot assignment loop. Raw `drawQuad` methods have been removed.
  - `Text` stores a dynamic `std::vector<Vertex>` directly, which is streamed into the batch buffer via single-pass vertex assignment.
  - `submitQuad`, `draw`, and `flush` assert active **Scene Scope** (`m_isSceneActive`) in debug builds.
- **Texture Slot Allocation**:
  - `MAX_TEXTURE_SLOTS = 32`. Textures submitted during draw calls are deduplicated into `m_textures`.
  - Texture unit indices `0..31` are mapped dynamically to the shader sampler array `u_textures`.
- **Automatic Batch Flush Conditions**:
  A batch flush (`flush()`) streams queued vertices to the GPU and issues `glDrawElements` whenever:
  1. Queued vertex count reaches capacity (`m_quadVerticesCount >= MAX_QUAD_VERTICES`).
  2. A draw call references a new texture when all 32 texture units are occupied.
  3. `endScene()` is called at the end of the Scene Scope.
  4. An unbatched custom mesh is drawn.
- **Natural Submission Order**: Geometry is drawn in the exact temporal order submitted by game layers. Z-indexing depth buffer inversions (`1.0f / zIndex`) have been completely eliminated, ensuring correct alpha blending between mixed primitive types.
- **Lines as Oriented Quads**: `drawLine(start, end, color, thickness)` computes the perpendicular normal offset and routes its oriented quad directly through `submitQuad(vertices, nullptr)`, requiring zero pipeline or shader switches.
- **Unbatched Custom Geometry**: `Renderer::draw(Ref<VertexArray>, Ref<Shader>, const Matrix&)` automatically flushes any active 2D dynamic batch before binding and drawing custom geometry.
- **Transform Hierarchy**: `Transformable` maintains `m_position`, `m_rotation`, `m_scale`, and `m_origin`. It lazily updates and caches `m_transformMatrix` (GLM 3x3 affine) and `m_inverseTransformMatrix` via `m_isTransformMatrixUpdateNeeded`.

### 4.5 Audio Subsystem (FMOD Studio)
- **Engine Core**: Built on `FMOD::Studio::System`.
- **Bank Loading**: Pre-loads configured banks (`Master.bank`, `Master.strings.bank`, `Music.bank`, `SFX.bank`) and loads sample data into memory during initialization.
- **Bus Control**: Exposes three dedicated audio buses for independent hierarchy volume and pause control:
  - `m_masterBus` (`bus:/`)
  - `m_musicBus` (`bus:/Music`)
  - `m_sfxBus` (`bus:/SFX`)
- **Playback & Lifetime**: `Audio::play("event:/path")` queries `FMOD::Studio::EventDescription`, creates an `FMOD::Studio::EventInstance`, starts it, and immediately calls `release()` so FMOD cleans up the instance automatically when playback ends.
- **Per-Frame Tick**: `Audio::update()` is called inside `Engine::run()` every frame, advancing 3D positions, streaming buffers, and processing FMOD event callbacks.

### 4.6 Resource Management Subsystem
- **Template Architecture**: `Resources<ResourceType>` template instantiated for:
  - `FontResources = Resources<Font>`
  - `ShaderResources = Resources<Shader>`
  - `TextureResources = Resources<Texture>`
  - `AnimationResources = Resources<Animation>`
- **XML Manifests**: Loads resource definitions declaratively from XML files (`assets/fonts.xml`, `assets/textures.xml`, `assets/animations.xml`).
- **Allocation-Free Lookups**: Storage uses `std::unordered_map<std::string, Ref<ResourceType>, string_view_hash, std::equal_to<>>` enabling zero-allocation lookups via `std::string_view`:
  ```cpp
  Ref<Texture> tex = m_layerContext.textureResources.getResource("player_idle"); // No std::string allocation
  ```

### 4.7 Core Foundation Module (`Orc/Core/`)
The foundational domain modularization is detailed in [ADR 0006](docs/adr/0006-modularize-core-primitives.md).
- **Domain Role**: Layer 0 foundational primitives independent of engine runtime coordination or OS windowing loops. All engine subsystems and client applications depend on `Orc/Core/`.
- **Granular Organization**:
  - `Platform.hpp`: Platform identification macros (`ORC_PLATFORM_WINDOWS`, `ORC_PLATFORM_LINUX`), debug assertions toggle (`ORC_DEBUG_ASSERTS`), platform-specific debugbreak trigger (`ORC_DEBUGBREAK()`), and compiler intrinsic headers.
  - `Memory.hpp`: Universal smart pointer type aliases (`Ref<T>`, `UniquePtr<T>`) and standard forwarding factory functions (`createRef<T>`, `createUniquePtr<T>`).
  - `Types.hpp`: GLM initialization settings (`GLM_FORCE_CTOR_INIT`, `GLM_ENABLE_EXPERIMENTAL`), math aliases (`Vector2/3/4` families, `Matrix`, `Matrix4`), file path alias (`FilePath`), and driver handle types (`RendererID`).
  - `Version.hpp`: Semantic engine version constants (`MAJOR_VERSION`, `MINOR_VERSION`, `PATCH_VERSION`) defined under `orc::version`.
  - `Core.hpp`: Umbrella aggregator header including `Platform.hpp`, `Memory.hpp`, `Types.hpp`, and `Version.hpp`.

---

## 5. Code Patterns, Argument Conventions, Naming & Style

To maintain architectural integrity across human and agent edits, follow these strict coding standards:

### 5.1 Naming Conventions
Follow the style of the surrounding file; the repository has no enforced formatter.

| Category | Convention | Examples |
| :--- | :--- | :--- |
| **Namespaces** | lowercase snake_case | `orc`, `orc::version`, `orc::glfw` |
| **Types / Classes / Structs** | PascalCase | `GameLayer`, `WindowResizedEvent`, `Vertex`, `AudioSettings` |
| **Functions / Methods** | camelCase | `loadFromFile()`, `isKeyPressed()`, `onAttach()`, `setViewportSize()` |
| **Local Variables** | camelCase | `elapsed`, `windowResizedEvent`, `gameLayer`, `dt` |
| **Member Variables** | `m_camelCase` | `m_running`, `m_videoSettings`, `m_glfwWindow`, `m_texture`, `m_camera` |
| **Constants & Macros** | UPPER_SNAKE_CASE | `MAX_TEXTURE_SLOTS`, `ORC_LOG_INFO`, `ORC_ASSERT`, `ORC_DEBUG` |
| **Enum Classes & Values** | PascalCase for both | `enum class Key { Space, Escape, LeftShift };` |
| **File Names** | PascalCase | `GameLayer.hpp`, `Renderer.cpp`, `Transformable.hpp` |

> [!NOTE]
> *Resolution of Naming Discrepancy*: While older guidelines colloquially stated "camelCase for functions, local variables, and data members", real codebase inspection confirms that member variables uniformly adhere to `m_camelCase` with the `m_` prefix across all engine and demo classes (`m_running`, `m_texture`, `m_camera`). The convention is formally specified as `m_camelCase` for member variables and `camelCase` for local variables and function parameters.

### 5.2 Parameter & Argument Passing Conventions
- **Strings (Read-Only)**: Use `std::string_view` for lookups, non-owning string inspection, and logging. Use `const std::string&` only where null-terminated C-strings (`.c_str()`) or third-party C interfaces (GLFW, GLAD, FMOD) strictly require it.
- **Paths**: Use `const FilePath&` (`const std::filesystem::path&`).
- **Buffers & Spans**: Pass contiguous memory via `std::span<const T>` or `std::span<const std::byte>` rather than raw pointers with separate size parameters.
- **Math & Structures**:
  - Pass compound types (`Vector2f`, `Vector3f`, `Vector4f`, `Matrix`, `Matrix4`, `Color`, `FloatRect`) by `const&`.
  - Pass primitive types (`float`, `uint32_t`, `int`, `bool`, `size_t`) and small enums/enum classes by value.
  - Pass small proxy objects (`std::initializer_list<T>`) by value.
- **Smart Pointer Passing (Ref<T>)**:
  - **Sinks**: When a constructor or setter takes ownership or stores a `Ref<T>` into a member variable or collection, take `Ref<T>` by value and sink it using `std::move(...)` to avoid extra atomic reference-count overhead.
  - **Observers**: When a method merely inspects, queries, binds, or temporary-draws a resource without retaining ownership, pass `const Ref<T>&` (or `const T&`) to avoid atomic reference count increments and decrements.

### 5.3 Constructor, Constness, Linkage & Type Safety Invariants
- **Explicit Constructors**: All single-argument constructors (and multi-argument constructors where trailing parameters have default values) must be marked `explicit` across all engine classes and event structs. The only permitted exception is intentional type-erasure wrappers.
- **Logical Constness**: Logical constness takes strict precedence over handle/pointer constness. Methods that mutate subsystem state, poll OS events, swap buffers, tick timers, advance audio mixers, or upload GPU buffer data must remain non-const, even if internal handles are pointers (`GLFWwindow*`, `FMOD::Studio::System*`) or driver IDs (`RendererID`). Pure inspector methods (e.g. `Texture::bind()`, `Resources::getResource()`, accessors) must be marked `const`.
- **Linkage Invariant**:
  - Local helper classes and formatters defined exclusively inside a `.cpp` file and not exposed in any header must be enclosed in an anonymous namespace to prevent symbol leakage across translation units.
  - Cross-module utility functions declared in public engine headers (such as `fmodCall`, `ftCall`, `getErrnoMessage`) must retain external linkage under `namespace orc`.
- **Casting & Type Safety**:
  - Never use C-style casts. Use `static_cast` for value/arithmetic conversions and `reinterpret_cast` for opaque pointer reinterpretation.
  - Narrowing conversions (such as from `uint32_t` stride/sizes to OpenGL signed `GLsizei`) must be explicitly cast with `static_cast<GLsizei>`.
- **Modern Syntax & Control Flow**:
  - Use C++17 concatenated nested namespaces (`namespace orc::glfw { ... }`).
  - Do not leave empty namespace stubs or stray semicolons after namespace closing braces.
  - Switch statements over bitmasks or integral states must explicitly provide a `default: break;` case.
  - Derived class destructors overriding a virtual base destructor must specify `override`.

### 5.4 Memory Ownership & RAII Rules
- **Rule of 5**: Resource-owning classes (wrapping OpenGL IDs, GLFW handles, FreeType faces, FMOD objects) must explicitly define move constructor + move assignment (zeroing moved-from IDs) and delete copy operations.
- **Smart Pointer Aliases**:
  - `orc::Ref<T>` (`std::shared_ptr<T>`) via `orc::createRef<T>(...)` for shared resources (`Texture`, `Font`, `Shader`, `GameLayer`).
  - `orc::UniquePtr<T>` (`std::unique_ptr<T>`) via `orc::createUniquePtr<T>(...)` for single-owner subsystems (`Renderer`, `Window`, `Audio`, `GameLayerManager`).
- **Destructor Teardown**: **Never** call `m_ptr.release()` in destructors to leak memory. Use `m_ptr.reset()` or let RAII destroy the resource automatically.

### 5.5 Error Handling & Logging
- **Logging Macros**: Use the built-in spdlog macros (standardized on ISO C++20 `__VA_OPT__(,) __VA_ARGS__` expansion, permitting calls without trailing format arguments):
  - `ORC_LOG_INFO(message, ...)`: Normal diagnostic messages.
  - `ORC_LOG_WARNING(message, ...)`: Recoverable unexpected states or fallbacks.
  - `ORC_LOG_ERROR(message, ...)`: Operation failures (e.g. failed asset load). Never use legacy `ORC_ERROR` directly in engine source files.
  - `ORC_LOG_FATAL(message, ...)`: Critical unrecoverable errors.
- **Assertion Macros**: Use `ORC_ASSERT(condition, message, ...)` for debug-only invariant checks (active under `ORC_DEBUG_ASSERTS`), also conforming to ISO C++20 `__VA_OPT__(,)`. Use `!container.empty()` rather than checking `.size()` as a boolean.
- **Graceful Degradation**: Handle recoverable runtime failures gracefully (e.g. return `false`, `nullptr`, or `std::nullopt`) rather than crashing the engine.

### 5.6 Formatting & File Layout
- **Tabs for Indentation**: Match the repository convention (tab character for indentation, verified by `lint-conventions`).
- **Braces**: Opening brace on a new line (Allman-style) for classes, methods, and control blocks.
- **Header Guards**: `#pragma once` at the top of every `.hpp` and `.inl` file.
- **Umbrella Header**: When adding a new public engine component, expose its header in `OrcEngine/include/Orc/Orc.hpp`.
- **File Extensions**: Use `.hpp` for headers, `.cpp` for implementations, and `.inl` for template or inline implementation files.
- **Codebase Discipline**: Avoid broad cleanup or formatting changes in files touched for a focused fix.

---

## 6. Build, Run, and Validation Discipline (Script-First)

### 6.1 Script-First Toolchain
Always prefer the automated scripts located in `scripts/` over invoking raw CMake commands. The scripts automate compiler detection (such as activating the Visual Studio Developer Command Prompt on Windows), environment checks, output directory management, and error log parsing.

| Task | Windows (PowerShell) | Linux / macOS (Bash) | Purpose |
| :--- | :--- | :--- | :--- |
| **Build (Debug)** | `.\scripts\build.ps1` | `./scripts/build.sh` | Configures and compiles `windows-debug` or `linux-debug` preset |
| **Build (Release)** | `.\scripts\build.ps1 -Preset windows-release` | `./scripts/build.sh --preset linux-release` | Optimized release build |
| **Build & Run Demo** | `.\scripts\build.ps1 -Run -TimeoutSeconds 5` | `./scripts/build.sh --run --timeout 5` | Builds and launches `TestProject` smoke test |
| **Lint Conventions** | `.\scripts\lint-conventions.ps1 -ChangedOnly` | `./scripts/lint-conventions.sh --changed-only` | Validates tabs, `#pragma once`, and namespace rules |
| **Full Codebase Lint**| `.\scripts\lint-conventions.ps1` | `./scripts/lint-conventions.sh` | Lints all header and implementation files |
| **Run Demo Directly** | `.\scripts\run-demo.ps1` | `./scripts/run-demo.sh` | Runs demo from proper working directory |
| **Demo Smoke Test** | `.\scripts\run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog` | `./scripts/run-demo.sh --timeout 5 --fail-on-error-log` | Verifies execution and checks `logs/TestProject.log` |
| **Environment Check** | `.\scripts\check-env.ps1` | `./scripts/check-env.sh` | Validates CMake, Ninja, compiler, and env variables |

### 6.2 Raw CMake Fallback Commands
If the automated scripts cannot be used, run the following raw CMake presets from the repository root:

**Windows:**
```powershell
# Debug build
cmake --preset windows-debug
cmake --build --preset windows-debug

# Release build
cmake --preset windows-release
cmake --build --preset windows-release

# Run demo (must run from build directory so relative assets resolve)
cd build/windows-debug
.\TestProject.exe
```

**Linux:**
```bash
# Debug build
cmake --preset linux-debug
cmake --build --preset linux-debug

# Release build
cmake --preset linux-release
cmake --build --preset linux-release

# Run demo (must run from build directory so relative assets resolve)
cd build/linux-debug
./TestProject
```

To compile only the engine without the demo, configure with `-DORC_BUILD_TEST_PROJECT=OFF` in a separate build directory or local user preset. Keep local preset files untracked.

### 6.3 Build Prerequisites & Environment Variables
The presets require CMake 3.25 or newer, Ninja, and a C++20 compiler (MSVC on Windows, GCC 11+ or Clang 13+ on Linux), plus the Linux system packages listed in `README.md` (when on Linux). In addition, two paths configure third-party dependencies:
- `VCPKG_ROOT`: Environment variable pointing to a vcpkg checkout. If unset on Linux, build and diagnostic scripts automatically fall back to `$HOME/vcpkg` if present. Open-source dependencies (`glfw3`, `glad`, `glm`, `fmt`, `spdlog`, `tinyxml2`, `freetype`, `imgui`, `stb`) are managed via `vcpkg.json`.
- `FMOD_ROOT`: Environment variable or CMake cache variable (`-DFMOD_ROOT=...`) pointing to a manually installed FMOD SDK (Core and Studio) with headers in `api/core/inc` and `api/studio/inc`, and libraries in `api/core/lib` and `api/studio/lib`. If unset, `cmake/FindFMOD.cmake` and automation scripts inspect standard multi-tier discovery paths on Linux (`/opt/fmod`, `/usr/local/fmod`, `/usr/local/opt/fmod`, `$HOME/fmod`, `$HOME/dev/libs/cpp/fmod`) and Windows (`Program Files`, user profile directories).

Do not commit machine-specific paths, generated build trees (`build/`), `vcpkg_installed/`, or `CMakeUserPresets.json`.

### 6.4 Compiler Warnings Configuration
Compiler warning and standards flags are configured centrally in `cmake/OrcWarnings.cmake` and `CMakeLists.txt`:
- **GCC / Clang**: `-Wall -Wextra -Wno-unused-parameter`
- **MSVC**: `/W4 /wd4100` (warnings in `cmake/OrcWarnings.cmake`), with `/Zc:preprocessor` configured on `OrcEngine` (PUBLIC in `CMakeLists.txt`) for C++20 `__VA_OPT__` conforming preprocessor semantics

All engine and demo code must compile with **zero warnings** under these flags across both debug and release configurations.

### 6.5 Dependencies and Build Files
- Add open-source packages to `vcpkg.json` and declare their CMake targets in `CMakeLists.txt`.
- Keep FMOD discovery in `cmake/FindFMOD.cmake` because FMOD is not managed through vcpkg.
- Source lists use `GLOB_RECURSE` with `CONFIGURE_DEPENDS`, so new files under existing source trees are picked up automatically. Update `CMakeLists.txt` explicitly when introducing a new source root, generated source, dependency, compile definition, or resource copy step.
- Dependencies exposed in public engine headers belong in `OrcEngine`'s `PUBLIC` link interface. Implementation-only dependencies must remain `PRIVATE`.

### 6.6 Assets and Runtime Files
- `TestProject/assets/` and `TestProject/shaders/` are automatically copied next to the demo executable after each build, together with the FMOD runtime libraries.
- Keep asset manifest paths relative and verify added or renamed assets in a clean build directory.
- The renderer loads shaders from `shaders/` relative to the current working directory; always execute the demo from `build/<preset>` so relative paths resolve.
- Do not edit or commit generated build outputs. Checked-in DLLs and binary SDK artifacts are intentional legacy files; replace them only when explicitly required and document their source/version.

### 6.7 Unified Verification Gate & Pre-Commit Protection
Before concluding any implementation task or resolving a ticket, execute the unified verification gate:
- **Windows**: `.\scripts\verify.ps1`
- **Linux**: `./scripts/verify.sh`

The verification gate executes three sequential stages:
1. **Conventions Linting**: `.\scripts\lint-conventions.ps1 -ChangedOnly` (or `./scripts/lint-conventions.sh --changed-only`) verifying tab indentation, `#pragma once`, and namespace rules with zero errors. (Use `-Fix` or `--fix` to automatically convert 4-space indentation to tabs and strip trailing whitespace).
2. **Compilation**: Compiles the target build preset (`build.ps1` or `build.sh`) with zero compiler errors.
3. **Runtime Smoke Run**: Runs `TestProject` for 5 seconds with `--fail-on-error-log` (`run-demo.ps1` or `run-demo.sh`) ensuring no crashes, assertion failures, or error logs occur on startup.
4. **Git Pre-Commit Protection**: Install automated pre-commit conventions checking via `.\scripts\setup-git-hooks.ps1` (Windows) or `./scripts/setup-git-hooks.sh` (Linux) so that convention regressions are blocked automatically before git commits.
5. **Documentation Synchronization**: Verify that `GLOSSARY.md`, relevant ADRs in `docs/adr/`, and subsystem sections in `AGENTS.md` are up to date.

If FMOD, display, or audio access prevents runtime validation, report that limitation and still perform the strongest available compile check (including both debug and release configurations).

### 6.8 Change Discipline & "No Legacy Cruft"
- Keep engine API changes backward-compatible where practical and update `TestProject` usages in the same change.
- Check event, renderer, window, and audio lifetime assumptions before retaining references across frames or layer transitions.
- Add a focused demo screen or path for new visible engine behavior when no automated test can cover it.
- Update `README.md` when prerequisites, build commands, runtime steps, or advertised capabilities change.
- **No legacy cruft**: OrcEngine is an internal/private project. Aggressively prune dead, obsolete, or unused methods, typedefs, and dummy structs without hesitation or asking for permission. Breaking changes to engine APIs are completely acceptable as long as `TestProject` is updated in the same change.

---

## 7. Issue Tracking, Domain Docs, and Documentation Sync Rule

### 7.1 Local Markdown Issue Tracker
Issue tracking and task execution are handled locally through markdown files in `.scratch/`. For detailed specifications, refer to [docs/agents/issue-tracker.md](docs/agents/issue-tracker.md).

#### Directory Structure
```
.scratch/
└── <feature-slug>/
    ├── spec.md              # Feature specification and acceptance criteria
    ├── map.md               # Notes, Decisions so far, and Fog
    └── issues/
        ├── 01-<task-slug>.md
        ├── 02-<task-slug>.md
        └── ...
```

#### Ticket Header Format
Every ticket file in `issues/` must be formatted with metadata lines parsed by `scripts/ticket.ps1` and `scripts/ticket.sh`:
```markdown
# 01: Task Title

Status: ready-for-agent
Type: task
Blocked by: none
```
- `Status`: Current state (`needs-triage`, `needs-info`, `ready-for-agent`, `ready-for-human`, `claimed`, `resolved`, `wontfix`).
- `Type`: Work classification (`task`, `research`, `prototype`, `grilling`).
- `Blocked by`: Comma-separated list of preceding ticket numbers (e.g. `01`, `01, 02`, or `none`). A ticket is unblocked only when all referenced tickets have `Status: resolved`.

#### Wayfinding Workflow & CLI Commands
Use the `ticket.ps1` (Windows) or `ticket.sh` (Linux) script to navigate and update the ticket graph:

1. **List Tickets in Feature**:
   - Windows: `.\scripts\ticket.ps1 list <feature-slug>`
   - Linux: `./scripts/ticket.sh list <feature-slug>`
2. **Find Next Unblocked Ticket (Wayfinding Frontier)**:
   Scans `issues/` for open, unblocked tickets in numerical order:
   - Windows: `.\scripts\ticket.ps1 frontier <feature-slug>`
   - Linux: `./scripts/ticket.sh frontier <feature-slug>`
3. **Claim Ticket Before Starting**:
   Sets `Status: claimed`:
   - Windows: `.\scripts\ticket.ps1 claim .scratch/<feature-slug>/issues/<NN>-<slug>.md`
   - Linux: `./scripts/ticket.sh claim .scratch/<feature-slug>/issues/<NN>-<slug>.md`
4. **Resolve Ticket Upon Completion**:
   Sets `Status: resolved`, appends resolution details to `## Answer`, and updates `map.md` Decisions-so-far (only after `./scripts/verify.sh` or `.\scripts\verify.ps1` passes with 100% success):
   - Windows: `.\scripts\ticket.ps1 resolve .scratch/<feature-slug>/issues/<NN>-<slug>.md -Answer "Brief summary of work"`
   - Linux: `./scripts/ticket.sh resolve .scratch/<feature-slug>/issues/<NN>-<slug>.md "Brief summary of work"`
5. **Triage Overview Across Repository**:
   - Windows: `.\scripts\ticket.ps1 triage`
   - Linux: `./scripts/ticket.sh triage`

### 7.2 Canonical Triage Labels & Ticket Statuses
Issue tracking distinguishes between initial evaluation triage roles and active execution statuses:

- **Canonical 5-Role Triage Labels**: Defined in [docs/agents/triage-labels.md](docs/agents/triage-labels.md) and mapped from universal skill roles. Used to classify who should evaluate or implement an issue:
  - `needs-triage`: Maintainer needs to evaluate this issue.
  - `needs-info`: Waiting on reporter for more information.
  - `ready-for-agent`: Fully specified, ready for an autonomous agent.
  - `ready-for-human`: Requires human implementation.
  - `wontfix`: Evaluated and rejected.
- **Wayfinding Execution Statuses**: Managed via `ticket.ps1` / `ticket.sh` as documented in [docs/agents/issue-tracker.md](docs/agents/issue-tracker.md):
  - `claimed`: An agent has actively claimed the ticket and is executing work.
  - `resolved`: Ticket work is complete, verified, and recorded in `map.md`.

### 7.3 Domain Documentation & Architectural Records
OrcEngine uses a single-context domain documentation layout, documented in [docs/agents/domain.md](docs/agents/domain.md):
- **`GLOSSARY.md`**: Ubiquitous language dictionary for the repository. Always use terms as defined (e.g. `Renderer`, `Scene Scope`, `Quad Batcher`, `Submission Order`, `Texturable`, `Viewport`, `Framebuffer Dimensions`) and strictly avoid their `_Avoid_` synonyms.
- **`docs/adr/`**: Architectural Decision Records documenting non-trivial design decisions and trade-offs (e.g. [ADR 0001](docs/adr/0001-collapse-2d-render-submission-pipeline.md), [ADR 0002](docs/adr/0002-unify-vertex-and-untemplate-texturable.md), [ADR 0007](docs/adr/0007-centralize-viewport-and-synchronize-camera.md)). Always inspect existing ADRs before proposing architectural changes. If an approach contradicts an ADR, explicitly highlight the contradiction.

### 7.4 Strict Documentation Synchronization Rule
To prevent architectural documentation drift, the following condition is mandatory for every agent task:
> [!IMPORTANT]
> Whenever code changes affect engine terminology, subsystem architecture, lifecycle ordering, or design trade-offs, the agent **must** update:
> 1. `GLOSSARY.md`: Add or update domain definitions and list avoided synonyms.
> 2. `docs/adr/`: Record a new ADR whenever an architectural trade-off or pipeline modification is made.
> 3. `AGENTS.md`: Update the relevant subsystem architecture (Section 4), code patterns (Section 5), or build instructions (Section 6).
>
> A ticket is **not complete** until all three documentation surfaces are synchronized with the code.
