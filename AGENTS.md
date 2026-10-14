# AGENTS.md

## 1. Project Overview & Mission

OrcEngine is a modular, high-performance 2D game engine built in C++20 using OpenGL 4.6 (Core Profile), GLFW, GLAD, FreeType, FMOD Studio, spdlog, and GLM. It is compiled as a static library (`libOrcEngine.a`), with `TestProject` serving as the demo application, feature validation suite, and integration harness.

### Repository Layout
- **Public Engine Headers**: `OrcEngine/include/Orc/` (umbrella entry point: `OrcEngine/include/Orc/Orc.hpp`)
- **Engine Source**: `OrcEngine/source/Orc/`
- **Demo / Harness Headers & Sources**: `TestProject/include/` and `TestProject/source/`
- **Demo Runtime Assets & Shaders**: `TestProject/assets/` and `TestProject/shaders/`
- **Design & Architecture Docs**: `dev-docs/refactoring-plan/`
- **Build System**: `CMakeLists.txt`, `CMakePresets.json`, `cmake/`, `vcpkg.json`

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

Note: Games developed using OrcEngine reside in their own separate repositories. Reusable engine features belong in `OrcEngine`, and validation/prototypes belong in `TestProject`.

---

## 3. Agent Behavior & Consultation Protocol

To avoid misaligned changes, wasted effort, and premature assumptions, all autonomous agents must strictly adhere to the following consultation principles:

1. **Zero Unilateral Assumptions**:
   - Never assume architectural designs, implementation choices, library selections, or feature details without explicit user direction or confirmation.
   - Do not invent elaborate sub-phases or predetermined technical choices (e.g. picking specific C# runtimes or redesigning whole pipelines) without consulting the user first.
2. **Consult & Propose First ("Ask / Propose Options")**:
   - When an agent sees an opportunity, identifies a bug, or considers multiple implementation approaches, it must present clear options with trade-offs and **ask the user** before writing code.
3. **Incremental Execution**:
   - Work slowly and iteratively: implement one focused change or script, test and verify it thoroughly, obtain user confirmation, and only then proceed to the next step.
   - Avoid dumping large batches of files, scripts, or refactors all at once.
4. **Preserve Scope**:
   - Keep changes tightly scoped to the immediate task requested by the user.

---

## 4. Subsystem Architecture & Inter-Module Communication

OrcEngine consists of five core modules: **Engine**, **Graphics**, **Audio**, **Input**, and **Events**.

```
                              ┌────────────────────────────────────────┐
                              │            Engine Lifecycle            │
                              │    (Window, Audio, Renderer, Gui,      │
                              │     GameLayerManager, Resources)       │
                              └───────────────────┬────────────────────┘
                                                  │
                                                  ▼
                                      Main Loop: Engine::run()
                                                  │
         ┌────────────────────────────────────────┴────────────────────────────────────────┐
         │ 1. Clock::restart() -> elapsed delta time                                       │
         │ 2. GameLayerManager::getActiveLayer()                                           │
         │ 3. activeLayer->onUpdate(dt)                                                    │
         │ 4. Renderer::begin(camera) -> activeLayer->onRender() -> Renderer::end()        │
         │ 5. Gui::begin() -> activeLayer->onGuiRender() -> Gui::end()                     │
         │ 6. Audio::update()                                                              │
         │ 7. Window::display() -> glfwPollEvents() & glfwSwapBuffers()                    │
         └─────────────────────────────────────────────────────────────────────────────────┘
```

### 4.1 Engine & Layer Coordination
- `Engine` is the top-level coordinator. It initializes subsystems in sequence: `Logger` -> `GameLayerManager` -> `FTLibrary` -> `Window` (creates OpenGL context via GLAD) -> `Renderer` -> `Gui` -> `Audio` -> `ResourceHolder` caches.
- `GameLayerManager` stores registered layers in a hash map and maintains a pointer/reference to the active layer.
- `GameLayer` provides virtual lifecycle hooks:
  - `onAttach()`: Called when the layer becomes active (e.g. setting window title, initializing state).
  - `onDetach()`: Called when transitioning away from the layer.
  - `onUpdate(float dt)`: Frame logic and state updates.
  - `onRender()`: Submits 2D draw calls to `Renderer`.
  - `onGuiRender()`: Submits ImGui controls to `Gui`.
  - `onEvent(const Event& event)`: Handles dispatched input/window events.

### 4.2 Window, Input, and Events Flow
- **Event Generation**: GLFW window callbacks (key, mouse button, mouse move, scroll, window resize, window close) trigger lambdas set in `Window::setCallbacks()`.
- **Event Representation**: `Event` uses value semantics wrapping `std::variant<std::monostate, WindowResizedEvent, WindowClosedEvent, KeyboardKeyPressedEvent, KeyboardKeyReleasedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseMovedEvent, MouseWheelScrolledEvent>`.
  - Pattern matching via `event.visit(...)` using the `Overload` struct helper.
  - Type-safe inspection via `event.is<T>()`, `event.getIf<T>()`, and `event.as<T>()`.
  - Classic dispatching via `EventDispatcher(event).dispatch<T>(callback)`.
- **Event Propagation**: `Window` invokes its `EventCallback`, which calls `Engine::onEvent()`. The active `GameLayer` receives first chance to handle the event; then `Engine` processes engine-level hooks (closing on `WindowClosedEvent`, resizing viewport on `WindowResizedEvent`).
- **Input Polling**: `Keyboard::isKeyPressed(key)` and `Mouse::isButtonPressed(button)` query GLFW directly via `Engine::get().getWindow().getNativeWindow()`.

### 4.3 Graphics & Rendering Subsystem
- **Renderer Model**: 2D immediate/batch renderer using OpenGL 4.6.
- **Batching Pipelines**: Maintains specialized vertex arrays/buffers for:
  - `m_sprites`: Quads with 4 `SpriteVertex` elements (`position`, `color`, `texCoords`, `textureIndex`).
  - `m_rectangles`: Quads with 4 `RectangleVertex` elements.
  - `m_circles`: Quads with 4 `CircleVertex` elements (rendered via procedural circle fragment shader).
  - `m_glyphs`: Text characters with 4 `GlyphVertex` elements.
  - `m_lines`: Line primitives with 2 `LineVertex` elements.
- **Texture Slot Allocation**:
  - `MAX_TEXTURE_SLOTS = 32`. Textures collected during `draw()` calls are deduplicated into batch texture vectors.
  - Dynamic slot assignment: active texture unit indices (`0..31`) are passed to the shader array `u_textures`.
  - If a batch exceeds vertex count or hits 32 unique textures, `batchFlush()` is called automatically, submitting geometry via `glBufferSubData` and resetting batch structures.
- **Z-Ordering & Depth**: `Renderer` maintains an incremental `zIndex`. Each submitted quad receives `position.z = 1.0f / (float)zIndex` to ensure deterministic 2D sprite layering with OpenGL depth testing enabled (`GL_LEQUAL`).
- **Transform Hierarchy**: `Transformable` maintains `m_position`, `m_rotation`, `m_scale`, and `m_origin`. It caches `m_transformMatrix` (GLM 3x3 affine) and updates lazily via `m_isTransformMatrixUpdateNeeded`.

### 4.4 Audio Subsystem (FMOD Studio)
- **Engine**: Wrapper around `FMOD::Studio::System`.
- **Bank Loading**: Pre-loads configured banks (`Master.bank`, `Master.strings.bank`, `Music.bank`, `SFX.bank`) and loads sample data into memory during initialization.
- **Bus Control**: Exposes `m_masterBus` (`bus:/`), `m_musicBus` (`bus:/Music`), and `m_sfxBus` (`bus:/SFX`) for independent group volume and pause control.
- **Playback & Lifetime**: `Audio::play("event:/path")` creates an `FMOD::Studio::EventInstance`, starts it, and flags it with `release()` so FMOD cleans up the instance once playback finishes.
- **Per-Frame Tick**: `Audio::update()` is called inside `Engine::run()` every frame to update 3D positions, stream buffers, and process asynchronous FMOD events.

### 4.5 Resource Management Subsystem
- `Resources<ResourceType>` template (instantiated for `Font`, `Shader`, `Texture`, `Animation`).
- Loads definitions from XML manifests (`assets/fonts.xml`, `assets/textures.xml`, `assets/animations.xml`).
- Uses transparent string hashing (`string_view_hash`, `std::equal_to<>`) in `std::unordered_map` to eliminate allocations during string-keyed lookups:
  ```cpp
  Ref<Texture> tex = textureResources.getResource("player_idle"); // std::string_view lookup
  ```

---

## 5. Code Patterns, Argument Conventions, Naming & Style

To maintain architectural integrity across human and agent edits, follow these strict coding standards:

### 5.1 Naming Conventions
| Category | Convention | Examples |
| :--- | :--- | :--- |
| **Namespaces** | lowercase snake_case | `orc`, `orc::version`, `orc::glfw` |
| **Types / Classes / Structs** | PascalCase | `GameLayer`, `EventDispatcher`, `SpriteVertex`, `AudioSettings` |
| **Functions / Methods** | camelCase | `loadFromFile()`, `isKeyPressed()`, `onAttach()`, `setViewportSize()` |
| **Member Variables** | `m_camelCase` | `m_running`, `m_videoSettings`, `m_glfwWindow`, `m_texture` |
| **Constants & Macros** | UPPER_SNAKE_CASE | `MAX_TEXTURE_SLOTS`, `ORC_LOG_INFO`, `ORC_ASSERT`, `ORC_DEBUG` |
| **Enum Classes & Values** | PascalCase for both | `enum class Key { Space, Escape, LeftShift };` |
| **File Names** | PascalCase | `GameLayer.hpp`, `Renderer.cpp`, `Texturable.inl` |

### 5.2 Parameter & Argument Passing Conventions
- **Strings (Read-Only)**: Use `std::string_view` for lookups, non-owning string inspection, and logging. Use `const std::string&` only where null-terminated C-strings (`.c_str()`) or FMOD/OpenGL string interfaces strictly require it.
- **Paths**: Use `const FilePath&` (`const std::filesystem::path&`).
- **Buffers & Vertex Spans**: Pass contiguous memory via `std::span<const T>` or `std::span<const std::byte>` rather than raw `(void* data, uint32_t size)`.
- **Math & Structures**:
  - Pass compound types (`Vector2f`, `Vector3f`, `Matrix`, `Color`, `FloatRect`) by `const&`.
  - Pass primitives (`float`, `uint32_t`, `int`, `bool`, `size_t`) by value.
- **Move Semantics & Sinks**: Use pass-by-value + `std::move` or rvalue references (`T&&`) when transferring ownership into member fields.

### 5.3 Memory Ownership & RAII Rules
- **Rule of 5**: Resource-owning classes (wrapping OpenGL IDs, GLFW handles, FreeType faces, FMOD objects) must explicitly define move constructor + move assignment and delete copy operations.
- **Smart Pointer Aliases**:
  - `orc::Ref<T>` (`std::shared_ptr<T>`) via `orc::createRef<T>(...)` for shared resources (`Texture`, `Font`, `Shader`, `GameLayer`).
  - `orc::UniquePtr<T>` (`std::unique_ptr<T>`) via `orc::createUniquePtr<T>(...)` for single-owner subsystems (`Renderer`, `Window`, `Audio`).
- **Destructor Teardown**: **Never** call `m_ptr.release()` in destructors to leak memory. Use `m_ptr.reset()` or let RAII destroy the resource automatically.

### 5.4 Error Handling & Logging
- Use the built-in spdlog macros:
  - `ORC_LOG_INFO(message, ...)`: Normal diagnostic messages.
  - `ORC_LOG_WARNING(message, ...)`: Recoverable unexpected states or fallbacks.
  - `ORC_LOG_ERROR(message, ...)`: Operation failures (e.g. failed asset load).
  - `ORC_LOG_FATAL(message, ...)`: Critical unrecoverable errors.
- Use `ORC_ASSERT(condition, message, ...)` for debug-only invariant checks (active under `ORC_DEBUG_ASSERTS`).
- Handle recoverable runtime failures gracefully (e.g. return `false`, `nullptr`, or `std::optional<T>`) rather than crashing the engine.

### 5.5 Formatting & File Layout
- **Tabs for Indentation**: Match the existing repository convention (tab character for indents).
- **Braces**: Opening brace on a new line (Allman-style) for classes, methods, and control blocks.
- **Header Guards**: `#pragma once` at the top of every `.hpp` header.
- **Umbrella Header**: When adding a new public engine component, expose its header in `OrcEngine/include/Orc/Orc.hpp`.

---

## 6. Build, Run, and Validation Discipline

### Build Presets
From the repository root:
```bash
# Debug build (with symbols and assertions)
cmake --preset linux-debug
cmake --build --preset linux-debug

# Release build (optimized)
cmake --preset linux-release
cmake --build --preset linux-release
```

### Running TestProject
Always execute from the build directory so relative assets (`assets/`, `shaders/`) resolve:
```bash
cd build/linux-debug
./TestProject
```

### Validation Checklist for Changes
1. **Compilation Cleanliness**: Build both `linux-debug` and `linux-release`. Ensure **zero** compiler warnings (`-Wall`, `-Wextra`, `-Werror` clean).
2. **Runtime Verification**: If display/audio are accessible, run `TestProject` and verify the affected layer (e.g. `Menu`, `SpritesTest`, `InputTest`, `AudioTest`).
3. **Log Check**: Inspect `build/linux-debug/logs/TestProject.log` or console output for unexpected warnings or errors.
4. **Asset Integrity**: If shaders or assets are added/modified, ensure they are placed in `TestProject/shaders/` or `TestProject/assets/` and picked up by CMake copy steps.
