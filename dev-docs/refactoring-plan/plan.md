# OrcEngine Architecture Audit & Refactoring Master Plan

This document establishes the architectural baseline, identified safety and performance issues, modern C++ evolution paths, and key design questions for refactoring OrcEngine across all five primary engine modules.

---

## 1. Executive Summary & Codebase Audit Findings

OrcEngine is a 2D game engine built around OpenGL 4.6 (Core Profile), GLFW, GLAD, FreeType, FMOD Studio, and GLM. While functional, the engine currently exhibits several structural anti-patterns:
- **Pervasive Singleton Coupling**: `Engine::get()` is called directly by low-level leaf components (`Keyboard`, `Mouse`, `GameLayer`), creating circular dependencies and prohibiting multi-window, headless testing, or modular testing.
- **Resource Ownership Inconsistencies**: Destructors calling `std::unique_ptr::release()` instead of letting RAII clean up resources (resulting in severe memory/GPU resource leaks). Move operations are deleted across all GPU handles (`Shader`, `Texture`, `VertexBuffer`, `IndexBuffer`, `VertexArray`), forcing heap allocation via `std::shared_ptr`.
- **Renderer Batching Redundancy & Texture Slot Bugs**: Five separate batching pipelines with ~80% duplicated code. Texture IDs are directly passed as sampler slot indices (`0..31`), risking driver crashes and undefined behavior for IDs ≥ 32. Texture maps in shapes never clear across batch flushes.
- **Silent Logic Bugs**: Inverted boolean checks (e.g. `GameLayerManager::addLayer`), array index out-of-bounds risks, map iterator invalidation in `Animator`, and misleading coordinate nomenclature in `Rect<T>` (`right` vs `width`).
- **Compiler Warnings**: Member reordering in constructors (`-Wreorder`), NULL converted to `GLuint` (`-Wconversion-null`), and signed/unsigned comparison mismatches.

---

## 2. Module-by-Module Audit & Proposals

### Module A: Engine Module
#### Current Architecture
- `Engine` acts as an all-in-one God object owning `Window`, `Renderer`, `Audio`, `Gui`, `FTLibrary`, `GameLayerManager`, and all `Resources<T>`.
- `GameLayer` constructor reaches into `Engine::get()` and stores 7 raw reference members (`Audio&`, `Window&`, `Renderer&`, `GameLayerManager&`, `FontResources&`, `TextureResources&`, `AnimationResources&`), inflating every layer instance and locking it to a single static `Engine` lifetime.
- `GameLayerManager` stores layers in an `std::unordered_map<std::string, Ref<GameLayer>>` with inverted condition checks.
- `ResourceHolder` declares a generic `Resources<T>` template in a header, but explicitly specializes `loadResources` in a `.cpp`. Cache misses construct and heap-allocate an empty dummy object on every failed lookup.
- `Random` stores mutable static state without thread safety and `getSeed()` actually generates and returns the *next* number from the Mersenne Twister engine instead of returning the seed.
- `Clock` has duplicate clock queries between `elapsed()` and `reset()`.

#### Identified Safety / Performance Issues
1. **Engine Destructor Resource Leak**: `m_audio.release()`, `m_renderer.release()`, `m_gui.release()`, `m_window.release()`, and `m_ftLibary.release()` deliberately leak heap allocations and bypass destructor teardown.
2. **Inverted Check in Layer Manager**: `if (m_layers.try_emplace(name, gameLayer).second)` triggers `ORC_LOG_WARNING("Game layer '{}' is already added.")` when insertion *succeeds*, while failing to warn when insertion is rejected.
3. **Resource Leak on Cache Miss**: `getResource()` returns `createRef<ResourceType>()` on not found, causing continuous heap allocations on invalid lookups.

#### Modern C++ (C++20 / C++23) Proposals
- **Context Injection over Singleton**: Replace 7 stored references in `GameLayer` with an `EngineContext` (or pass an explicit `FrameContext` / `UpdateContext` / `RenderContext` to `onUpdate`, `onRender`, etc.).
- **Atomic Clock Restart**: Replace `elapsed()` + `reset()` with a unified `float restart() noexcept` using `std::chrono::steady_clock` to eliminate clock drift.
- **Thread-safe RNG**: Use `thread_local std::mt19937` or modernize with standard distributions, storing seed explicitly.
- **Proper Resource Cache with `std::optional` / Nullable Ref**: Return `Ref<ResourceType>` as `nullptr` or `std::optional<Ref<ResourceType>>` instead of instantiating empty resources.

---

### Module B: Graphics Module
#### Current Architecture
- Custom immediate/batch 2D renderer using dynamic VBOs with quads and lines.
- Five separate batch containers (`Lines`, `Shape<CircleVertex>`, `Shape<SpriteVertex>`, `Shape<RectangleVertex>`, `Shape<GlyphVertex>`).
- OpenGL resource wrappers (`Shader`, `Texture`, `VertexBuffer`, `IndexBuffer`, `VertexArray`) prohibit copy AND move operations, forcing `orc::Ref` (`std::shared_ptr`) for everything.
- `Transformable` maintains cached matrices and dirty flags via mutable members.
- `Rect<T>` has misleading member semantics: `right` stores `width`, and `bottom` stores `height`.
- `Camera` performs 4x4 matrix inversion (`glm::inverse`) on every single position or zoom change.

#### Identified Safety / Performance Issues
1. **Critical Texture Slot Bug in `Renderer`**:
   - `texture.second->bind(texture.first)` passes the OpenGL texture ID (`getRendererID()`, e.g. 42) as the active texture slot (`0..31`).
   - Vertex `textureIndex` is written as `(float)texture->getRendererID()`. In GLSL: `u_textures[int(v_textureIndex)]` causes an out-of-bounds access on `u_textures[32]` for any texture ID ≥ 32.
   - `m_sprites->textures` is never cleared on flush, meaning once 32 textures are loaded, the batch flushes on *every single sprite*.
2. **`Animator` Map Pointer Invalidation**:
   - `m_currentAnimation = &animation->second;` stores a raw pointer into an `std::unordered_map`. If another animation is added, rehashing invalidates this pointer, leading to use-after-free.
3. **Division by Zero in `Animator`**:
   - `m_currentAnimation->durationMs / m_currentAnimation->frames.size()` has no check for `frames.empty()`.
4. **Memory Leak in `Texture::loadFromFile`**:
   - If image channels are not 3 or 4, `pixels` from `stbi_load` is leaked without calling `stbi_image_free`.
5. **Raw Pointer Unsafety in Buffer APIs**:
   - `VertexBuffer(void* vertices, uint32_t size)` takes non-const `void*`.
6. **Compiler Warnings**:
   - `Shader.cpp`: `glUseProgram(NULL)` causes `-Wconversion-null`.
   - `Sprite.cpp`, `Transformable.cpp`, `BufferLayout.cpp`: member initialization order causes `-Wreorder`.

#### Modern C++ (C++20 / C++23) Proposals
- **Move-Enabled RAII Wrappers**: Implement the Rule of 5 (move constructor + move assignment, delete copy) for `Shader`, `Texture`, `VertexBuffer`, `IndexBuffer`, `VertexArray`.
- **`std::span` for Buffer Interfaces**: Pass `std::span<const std::byte>` or `std::span<const T>` instead of `void*` + `size`.
- **Unified 2D Batch Pipeline**: Merge Sprite, Rectangle, and Glyph batches into a unified Quad Batcher. Consolidate index generation and batch flushing logic.
- **Dynamic Texture Slot Allocator**: Map OpenGL texture handles to active texture units `[0..31]` during batching, clearing the map on flush.
- **Optimized 2D Camera**: Compute view matrix directly with inverse translation/rotation/scale rather than full 4x4 `glm::inverse()`.
- **Clarify `Rect<T>`**: Replace confusing `left, top, right, bottom` with unambiguous `x, y, width, height` (or `min, max`).

---

### Module C: Audio Module
#### Current Architecture
- Built on FMOD Studio.
- `Audio` holds pointers to FMOD Studio System and Bank objects, with three pre-configured `Bus` instances (`Master`, `Music`, `SFX`).
- Public headers include `<fmod_studio.hpp>` directly, forcing FMOD SDK include requirements onto all consumer code.
- Banks are hardcoded in `Audio.cpp`:
  ```cpp
  banks.push_back("assets/audio/Master.bank");
  banks.push_back("assets/audio/Master.strings.bank");
  banks.push_back("assets/audio/Music.bank");
  banks.push_back("assets/audio/SFX.bank");
  ```
  completely ignoring any bank paths specified in configuration.

#### Identified Safety / Performance Issues
1. **Incomplete Bank Unloading**: `Audio::~Audio()` calls `bank->unloadSampleData()` but never calls `bank->unload()`, leaking bank handles in FMOD.
2. **Limited Playback Control**: `Audio::play(const std::string& eventPath)` immediately calls `eventInstance->release()`. There is no handle or interface to pause, stop, change pitch, or alter parameters of a running sound.
3. **Public Header Leak**: Consumers of `Orc.hpp` or `Audio.hpp` must have FMOD Studio headers in their include path.

#### Modern C++ (C++20 / C++23) Proposals
- **Config-Driven Asset Loading**: Load bank paths dynamically from `AudioSettings` or configuration manifests.
- **Pimpl / Forward-Declaration Idiom**: Keep FMOD structures out of public engine headers so consumers don't require FMOD include directories.
- **Sound / Channel Handle Abstraction**: Introduce an `AudioInstance` or `SoundHandle` for controllable sound playback (pause, stop, volume, parameters).

---

### Module D: Input Module
#### Current Architecture
- `Keyboard` and `Mouse` expose static query methods (`Keyboard::isKeyPressed`, `Mouse::getPosition`, `Mouse::isButtonPressed`).
- Internally, both classes invoke `Engine::get().getWindow().getNativeWindow()` and query GLFW directly.
- Huge manual translation tables between GLFW keys/buttons and Orc keys/buttons.

#### Identified Safety / Performance Issues
1. **Broken Keyboard Mapping**:
   - `case GLFW_KEY_2: return Keyboard::Key::Four;` — Key `2` incorrectly maps to `Key::Four`.
   - Keys `3` and `4` are completely missing from the key mapping switch.
2. **Wrong Default in Mouse Mapping**:
   - `orcButtonToGlfwButton` returns `GLFW_KEY_UNKNOWN` on invalid/default button rather than an invalid mouse button.
3. **C-Style Casts**:
   - `(Vector2f)mousePosition` in `Mouse::getPosition()` rather than `static_cast<Vector2f>`.
4. **Hard Dependency on Engine**:
   - Input cannot be queried or mocked without a running `Engine` and GLFW window.

#### Modern C++ (C++20 / C++23) Proposals
- **Clean Lookup Tables / `std::array`**: Replace massive 300-line switch statements with `constexpr` lookup tables or flat arrays for $O(1)$ zero-branch translation.
- **Window-Scoped or Context-Aware Input**: Allow querying input from a specific `Window` instance or via an `Input` subsystem instead of reaching into the global singleton.

---

### Module E: Events Module
#### Current Architecture
- Abstract base class `Event` with virtual destructor and `virtual Type getEventType() const = 0`.
- Template CRTP-like helper `EventT<Event::Type>`.
- Downcasting helper `getEvent<DerivedEvent>(event)` utilizing `ORC_ASSERT` and `static_cast`.
- `mutable bool m_handled` to allow const event references to be marked handled.

#### Identified Safety / Performance Issues
1. **Runtime Type Assertions instead of Compile-Time Safety**: Downcasting with `static_cast` relies on runtime asserts. In release builds (`ORC_RELEASE`), if an incorrect event type is passed, the cast succeeds blindly, leading to undefined memory access.
2. **Const-Correctness Circumvention**: Const reference `onEvent(const Event& event)` combined with `mutable bool m_handled` is an anti-pattern. If an event is mutable state being consumed and handled, it should be passed as non-const or return a consumption status.
3. **No Direct Event Dispatcher**: Layers must use `switch(event.getType())` followed by manual `getEvent<T>(event)` boilerplate.

#### Modern C++ (C++20 / C++23) Proposals
- **Option 1: Modern `std::variant` & `std::visit`**:
  Model all events as a variant:
  `using Event = std::variant<WindowResizedEvent, WindowClosedEvent, KeyboardKeyPressedEvent, KeyboardKeyReleasedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseMovedEvent, MouseWheelScrolledEvent>;`
  - Eliminates virtual functions, vtables, and heap overhead completely.
  - Allows pattern-matching via `std::visit(overloaded { [](const WindowResizedEvent& e) { ... }, ... }, event)`.
  - Compile-time exhaustive checking.
- **Option 2: Event Dispatcher Class**:
  Keep class-based events if extensibility is required, but implement a type-safe `EventDispatcher`:
  ```cpp
  EventDispatcher dispatcher(event);
  dispatcher.dispatch<WindowResizedEvent>([](WindowResizedEvent& e) { ... return true; });
  ```

---

## 3. Cross-Cutting Engineering Guidelines

1. **Rule of 5 / RAII Compliance**:
   All classes owning resources (OpenGL handles, FMOD handles, GLFW windows, FreeType faces) must implement explicit move operations and disable copy, ensuring zero accidental duplication and zero resource leaks.
2. **Buffer Safety with `std::span`**:
   Deprecate raw `(void* data, uint32_t size)` idioms across vertex buffers, index buffers, and texture loaders.
3. **Explicit Error Handling**:
   Replace crashing asserts (`ORC_FATAL_CHECK`) for recoverable runtime failures (e.g. missing texture file, missing font, XML parsing) with `std::optional` or `std::expected` (C++23).
4. **Compile-Time Constraints with C++20 Concepts**:
   Constrain templates like `Rect<T>` with `std::integral` or `std::floating_point`.
5. **No Warnings Policy**:
   Fix all `-Wreorder`, `-Wsign-compare`, and `-Wconversion-null` warnings, adhering to strict clean compilation.

---

## 4. Key Questions & Design Decisions for You ("Grill Me")

To ensure the refactoring aligns with your vision, please review these key architectural decisions:

1. **Event System Architecture**:
   - Do you prefer **`std::variant` with `std::visit`** (zero virtual overhead, value semantics, fixed event set, pattern matching) or a **classic Polymorphic Event Dispatcher** (`EventDispatcher.dispatch<T>()`)?
2. **Singleton vs Context Injection**:
   - Would you like to eliminate the `Engine::get()` singleton pattern in favor of injecting an `EngineContext&` into `GameLayer` and subsystems, or keep `Engine::get()` as a convenience accessor for client code while decoupling internal engine modules?
3. **Renderer Batching**:
   - Should we unify Sprite, Textured Rectangle, and Glyph rendering into a single Quad Batcher (which reduces draw calls and state switches when interspersing text and sprites), or keep distinct batchers with shared base logic?
4. **Input Querying**:
   - Should `Keyboard::isKeyPressed` and `Mouse::isButtonPressed` accept an optional/default `Window*` or subsystem reference, or should we introduce an `Input` class owned by the window/context?
5. **Asset Pipeline & Config**:
   - Should audio banks and font/texture XML paths be loaded dynamically through a configuration manifest file rather than hardcoded string paths in C++ sources?

---

## 5. Grill-Me Verification & Decision Log

The interactive design verification session established the following architectural choices:

| Module | Verification Outcome | Selected Architecture |
| :--- | :--- | :--- |
| **Events** | `std::variant` & `std::visit` | Full value-semantic events without virtual dispatch or RTTI. Provides `std::visit` with `Overload` pattern matching, `is<T>()` and `getIf<T>()` query helpers, and backwards-compatible `getType()` / `getEvent<T>()` shims. |
| **Graphics** | 32-Slot Dynamic Batching | Retain dynamic slot mapping (`0..31`) with per-flush cleanup. Analytical 2D camera inverse (`scale * rotate * translate`) confirmed. |
| **Engine** | RAII Memory Cleanups | Retain existing `Engine` coordinator with static accessors; apply RAII memory leak fixes (`.reset()` over `.release()`), without introducing ECS or Service Locator complexity. |
| **Audio** | FMOD Internal Lookups | Rely directly on FMOD's internal lookup tables without extra engine-side pointer caching; enforce full RAII cleanup (`bank->unload()`). |
| **Input** | Direct State Polling | Retain zero-allocation direct querying (`Input::isKeyPressed`, `Mouse::isButtonPressed`). |
| **Modern C++ Views** | `std::string_view` & `std::span` | Replace `const std::string&` with `std::string_view` for string parameters and `void*` with `std::span` for GPU buffers across engine modules. |

