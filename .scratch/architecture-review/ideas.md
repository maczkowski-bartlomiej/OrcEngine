# Architecture Deepening Candidates & Grilling Guide

This document captures the architectural deepening opportunities identified for **OrcEngine** during the codebase architecture review. Each candidate proposes turning shallow, leaky modules into deep modules with small interfaces, high leverage for callers, and high locality for maintainers.

The opportunities are prepared for future **grilling sessions** (`/grill-me` or `/grilling`), including design trees and frontier decision rounds.

---

## Summary of Candidates

| # | Candidate | Category | Strength | Core Focus |
|---|---|---|---|---|
| 1 | **Collapse the 2D Render Submission Pipeline** | `local-substitutable` | **Strong** | Consolidate 4 fragmented shape batchers and double-begin lifecycle leakage into a single deep 2D batch renderer. |
| 2 | **Decouple Layer Runtime from Global Engine Singleton** | `in-process` | **Strong** | Replace ambient `Engine::get()` reach-in with an explicit frame context at the layer lifecycle seam. |
| 3 | **Consolidate the Fragmented Asset Pipeline** | `local-substitutable` | **Worth exploring** | Merge 4 specialized resource holders into a single deep `AssetManager` with unified XML parsing and safe fallbacks. |
| 4 | **Encapsulate Camera Projection & Viewport Synchronization** | `in-process` | **Speculative** | Replace 14 eager mutators with lazy projection calculation and automatic viewport tracking. |

---

## Candidate 1: Collapse the 2D Render Submission Pipeline

### 1. Classification & Scope
- **Status:** `needs-triage`
- **Strength:** `Strong`
- **Dependency Category:** `local-substitutable` (OpenGL context / test double)
- **Primary Files:**
  - `OrcEngine/include/Orc/Graphics/Renderer.hpp` & `OrcEngine/source/Orc/Graphics/Renderer.cpp`
  - `OrcEngine/include/Orc/Engine/Engine.hpp` & `OrcEngine/source/Orc/Engine/Engine.cpp`
  - `OrcEngine/include/Orc/Engine/GameLayer.hpp`
  - `TestProject/source/*.cpp` (All demo layers)

### 2. Architectural Friction & Seam Analysis
- **Shallow Interface & Seam Leakage:**
  - `Engine::run()` calls `m_renderer->begin(gameLayer->getCamera())` and `m_renderer->end()`, yet every demo layer (`CameraTest`, `SpritesTest`, etc.) calls `renderer.clear()`, `renderer.setClearColor()`, `renderer.begin(camera)`, and `renderer.end()` inside `onRender()`. The frame render lifecycle leaks across the seam.
  - `Renderer` exposes 4 separate shape-specific draw functions: `draw(Sprite)`, `draw(Circle)`, `draw(Rectangle)`, `draw(Text)`.
  - Internally, `Renderer` manages 4 copy-pasted batch structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) that flush independently in `batchFlush()`.
  - Because batch flushes are partitioned by shape type rather than submission order, draw ordering across different shape types fails unless worked around using an inverted z-value trick (`1.0f / zIndex`) and depth buffer testing.
- **The Deepening:**
  - Consolidate 2D primitive rendering behind a unified `Renderer2D` module.
  - The external interface presents simple geometric submissions (`drawQuad`, `drawRotatedQuad`, `drawCircleQuad`, `drawString`).
  - The implementation absorbs vertex layout formatting, 32-slot texture unit allocation, dynamic index buffering, and automatic flushes on buffer overflow or texture saturation.
  - Draw submission order is naturally preserved without depth-buffer z-index inversions.
- **Benefits:**
  - *Locality:* Batch management and OpenGL drawing logic concentrate in one module.
  - *Leverage:* One interface renders all 2D shapes across all game layers.
  - *Deletions:* 4 duplicated shape batch structures and redundant `begin`/`end` invocations vanish.
  - *Testability:* Pipeline can be verified with a null/headless shader or mock GL state.

### 3. Grilling Frontier (Design Tree for Future Interview)

❓ **Q1.1 - Unified Quad Vertex vs Multiple Buffers:** Should all 2D shapes (sprites, rectangles, circles, glyphs) be unified into a single quad vertex format with a composite shader, or should `Renderer2D` maintain separate internal pipelines that flush whenever the primitive type switches?
➡️ **Recommended answer:** Use a unified quad vertex format that carries shape attributes (color, texture coords, texture index, primitive type, corner radii/parameters). This keeps batching continuous across mixed primitives without premature draw call breaks.

---

❓ **Q1.2 - Frame Lifecycle Ownership:** Should `Engine` own `beginFrame()`/`endFrame()` and camera binding, or should the active `GameLayer` explicitly declare when rendering begins and with which camera?
➡️ **Recommended answer:** `GameLayer::onRender()` should receive an active `Renderer2D&` already scoped to a target camera or pass the camera to `renderer.beginScene(camera)`. The outer `Engine` manages clearing and target buffer presentation.

---

❓ **Q1.3 - Backward Compatibility with Existing Shape Objects:** Should `Sprite`, `Circle`, `Rectangle`, and `Text` retain their `Renderer::draw(shape)` convenience overloads, or should they become purely data/transform definitions?
➡️ **Recommended answer:** Retain lightweight overloads on `Renderer2D` that extract vertices/quad data and delegate to the internal quad batcher, keeping call sites in `TestProject` concise while unifying the backend.

---

## Candidate 2: Decouple Layer Runtime from Global Engine Singleton

### 1. Classification & Scope
- **Status:** `triaged`
- **Strength:** `Strong`
- **Dependency Category:** `in-process`
- **Primary Files:**
  - `OrcEngine/include/Orc/Engine/Engine.hpp` & `OrcEngine/source/Orc/Engine/Engine.cpp`
  - `OrcEngine/include/Orc/Engine/GameLayer.hpp` & `OrcEngine/source/Orc/Engine/GameLayer.cpp`
  - `OrcEngine/source/Orc/Input/Keyboard.cpp` & `OrcEngine/source/Orc/Input/Mouse.cpp`

### 2. Architectural Friction & Seam Analysis
- **Shallow Interface & Ambient Leakage:**
  - `Engine::get()` is a global singleton service locator.
  - `GameLayer` constructor secretly binds 7 references (`audio`, `window`, `renderer`, `gameLayerManager`, `fontResources`, `textureResources`, `animationResources`) by reaching into `Engine::get()`.
  - Static input queries (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`) query `Engine::get().getWindow().getNativeWindow()`.
  - As a result, no layer, input routine, or game logic can be constructed or tested without initializing the full OpenGL window, FreeType library, and FMOD audio system.
- **The Deepening:**
  - Define an explicit in-process `FrameContext` (containing `deltaTime`, `InputState&`, `AssetRegistry&`, `Audio&`) passed into `GameLayer::onUpdate(FrameContext&)` and `GameLayer::onRender(RenderContext&)`.
  - Input state is captured during event polling and queried through the context rather than reaching into native GLFW window handles.
- **Benefits:**
  - *Locality:* Layer dependencies become visible at the interface rather than hidden inside the constructor.
  - *Leverage:* Headless unit tests can construct and update game layers with a mock context in microseconds.
  - *Isolation:* Breaking ambient singleton dependencies allows running multiple test layers or simulations in parallel.

### 3. Grilling Frontier (Design Tree for Future Interview)

❓ **Q2.1 - Context Passing vs Constructor Injection:** Should dependencies be passed per-frame via a `FrameContext` struct to `onUpdate`/`onRender`, or injected once into `GameLayer::onAttach(LayerContext&)`?
➡️ **Recommended answer:** Pass `LayerContext` on `onAttach` for persistent resources (assets, audio), and pass `float dt, const Input&` to `onUpdate`. This avoids storing raw references to the entire engine inside the base layer.

---

❓ **Q2.2 - Static Input Queries:** Should `Keyboard::isKeyPressed` remain static (backed by an engine input state buffer) or become an instance method on an `Input` service passed to layers?
➡️ **Recommended answer:** Make `Input` an explicit module owned by `Engine` and queried via context, with an optional static facade if legacy ease-of-use in tests is deemed essential.

---

## Candidate 3: Consolidate the Fragmented Asset Pipeline

### 1. Classification & Scope
- **Status:** `needs-triage`
- **Strength:** `Worth exploring`
- **Dependency Category:** `local-substitutable` (Filesystem XML manifests / in-memory manifests)
- **Primary Files:**
  - `OrcEngine/include/Orc/Engine/ResourceHolder.hpp` & `OrcEngine/source/Orc/Engine/ResourceHolder.cpp`
  - `OrcEngine/include/Orc/Engine/Engine.hpp` & `OrcEngine/source/Orc/Engine/Engine.cpp`

### 2. Architectural Friction & Seam Analysis
- **Shallow Interface & Duplication:**
  - Resource management is split into 4 separate template specializations: `Resources<Font>`, `Resources<Texture>`, `Resources<Shader>`, `Resources<Animation>`.
  - Each specialization re-implements `loadResources(const FilePath&)` with identical `tinyxml2` document parsing loops.
  - Errors in XML parsing trigger hard aborts via `ORC_FATAL_CHECK`, terminating the process instead of returning fallback assets (e.g. pink checkerboard texture or default font).
  - `Engine` exposes 3 different getters for resources, and `GameLayer` stores 3 separate references.
- **The Deepening:**
  - Collapse `Resources<T>` into a unified, deep `AssetManager` module.
  - Interface: `get<T>(string_view name)` and `loadManifest(FilePath path)`.
  - Implementation encapsulates XML traversal, type-safe asset registries, cache eviction, and missing-asset fallback placeholders.
- **Benefits:**
  - *Locality:* XML parsing and asset caching concentrate entirely inside one module.
  - *Leverage:* One interface retrieves all engine assets with compile-time type safety.
  - *Safety:* Graceful fallback textures/fonts replace hard application crashes.

### 3. Grilling Frontier (Design Tree for Future Interview)

❓ **Q3.1 - Manifest Format:** Should `AssetManager` stick with the current XML format or move to a more modern/compact format (JSON / YAML / binary), and should multi-type assets be loaded in a single manifest?
➡️ **Recommended answer:** Maintain backward compatibility with the existing XML schema first, but support a single `<ASSETS>` manifest that contains fonts, textures, shaders, and animations together.

---

❓ **Q3.2 - Fallback Handling on Missing Asset:** When an asset fails to load or is not found in `get<T>(name)`, should `AssetManager` return `nullptr`, a fallback default asset (e.g., magenta 1x1 texture), or throw/log?
➡️ **Recommended answer:** Log an error and return a built-in default asset (1x1 white texture, default font) so game layers don't segfault on missing files during development.

---

## Candidate 4: Encapsulate Camera Projection & Viewport Synchronization

### 1. Classification & Scope
- **Status:** `needs-triage`
- **Strength:** `Speculative`
- **Dependency Category:** `in-process`
- **Primary Files:**
  - `OrcEngine/include/Orc/Graphics/Camera.hpp` & `OrcEngine/source/Orc/Graphics/Camera.cpp`
  - `OrcEngine/source/Orc/Engine/Engine.cpp`

### 2. Architectural Friction & Seam Analysis
- **Shallow Interface & Eager Computation:**
  - `Camera` provides 14 getters, setters, and relative offset mutators (`setZoom`, `zoom`, `setPosition`, `move`, `setRotation`, `rotate`, `setViewportSize`).
  - Every single setter eagerly triggers `recalculateViewMatrix()`, computing matrix multiplications even if the camera is mutated multiple times before the next draw call.
  - `Engine::onEvent` manually intercepts `WindowResizedEvent` and directly mutates the active layer's camera viewport size.
- **The Deepening:**
  - Deepen `Camera` into a `Camera2D` module with lazy matrix evaluation (dirty flag pattern) and clean coordinate conversion utilities (`screenToWorld`, `worldToScreen`).
  - Projection automatically adapts to viewport dimensions without requiring callers to push raw float rectangles.
- **Benefits:**
  - *Locality:* Viewport logic and coordinate transformations reside inside the camera module.
  - *Performance:* Matrix calculation occurs only once per frame when the matrix is requested by the renderer.
  - *Clean Interface:* Eliminates redundant setter variations and external resize tampering.

### 3. Grilling Frontier (Design Tree for Future Interview)

❓ **Q4.1 - Lazy Evaluation vs Direct Transform:** Should the camera use dirty-flag caching for `getViewProjectionMatrix()`, or should transforms simply produce a `glm::mat4` on request?
➡️ **Recommended answer:** Use dirty-flag caching. Most frames have stationary or single-move cameras; recomputing only when dirty is standard and cheap.

---

❓ **Q4.2 - Coordinate Conversions:** Should `screenToWorld(Vector2f)` and `worldToScreen(Vector2f)` be part of the Camera interface?
➡️ **Recommended answer:** Yes. This is one of the most common requirements in 2D engines (for mouse picking and UI placement) and deeply justifies the camera module's seam.

---

## Recommended Next Steps

1. Select one candidate to grill (recommended starting point: **Candidate 1: Collapse the 2D Render Submission Pipeline**).
2. Run `/grilling` or `/grill-me` pointing to this document to work through the candidate's design tree.
3. Once decisions are settled, run `/to-spec` and `/to-tickets` to generate tickets under `.scratch/architecture-review/issues/` for implementation.
