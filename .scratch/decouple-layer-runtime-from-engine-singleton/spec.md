# Spec: Decouple Layer Runtime from Global Engine Singleton

Status: ready-for-agent

## Problem Statement

Game layers, input polling, GUI initialization, and font loading were coupled to the global `Engine::get()` singleton. Furthermore, the initial iteration of layer decoupling introduced redundant wrappers (`FrameContext`, `onRender(Renderer&)`), unnecessary getter methods on `Engine` and `GameLayer`, inconsistent parameter naming (`context` instead of `layerContext`), speculative fallback viewport constants, and a public FreeType header leak (`FTLibrary`).

Specifically:
1. `FrameContext` duplicated `Input&` and `LayerContext&`, both of which are already available on the `GameLayer` instance via constructor-injected `m_layerContext`.
2. Passing `Renderer&` to `onRender` was redundant because `m_layerContext.renderer` is already bound at construction time.
3. `Engine` still exposed `Engine::get()` and individual subsystem getters (`getAudio`, `getWindow`, `getRenderer`, `getFTLibary`, `getGameLayerManager`, `getInput`, `getFontResources`, `getTextureResources`, `getAnimationResources`) only because `Gui` fetched `Window` via `Engine::get().getWindow()` and `Font` fetched `FT_Library` via `Engine::get().getFTLibary()`.
4. `FTLibrary` existed as a standalone engine subsystem and leaked `<freetype/freetype.h>` into public headers even though FreeType is linked privately and only needed transiently while loading a font's glyph bitmap atlas.
5. `GameLayer` exposed `getCamera()` and `getContext()` getters even though layers access `m_layerContext` and `m_camera` directly, and named its camera member `camera` instead of `m_camera`.
6. `GameLayer` included speculative `DEFAULT_VIEWPORT_WIDTH` / `DEFAULT_VIEWPORT_HEIGHT` fallback logic instead of directly using `m_layerContext.window` dimensions.

## Solution

Complete the decoupling of the layer runtime and engine subsystems and eliminate the `Engine::get()` singleton entirely:
1. **Single Injection Seam (`LayerContext`)**: Inject `LayerContext& layerContext` into `GameLayer`'s constructor (`m_layerContext`) as the sole channel for persistent engine services (`window`, `renderer`, `audio`, `gameLayerManager`, `fontResources`, `textureResources`, `animationResources`, `input`).
2. **Eliminate `FrameContext` & Redundant Render Parameter**: Revert `GameLayer::onUpdate` to `virtual void onUpdate(float deltaTime)` and `GameLayer::onRender` to `virtual void onRender()`, with layers accessing `m_layerContext.input` and `m_layerContext.renderer` directly.
3. **Encapsulate FreeType Inside `Font`**: Remove the `FTLibrary` class and `FT_Face` member from `Font`. Initialize a local `FT_Library` and `FT_Face` inside `Font::loadFromFile`, build the glyph atlas texture, and immediately destroy the face and library handles.
4. **Inject `Window&` into `Gui`**: Pass `Window& window` directly to the `Gui` constructor instead of reaching into `Engine::get().getWindow()`.
5. **Eliminate `Engine::get()` & Individual Getters**: Remove the static singleton instance, `Engine::get()`, and all individual subsystem getters from `Engine`, leaving only `Engine::getLayerContext()` alongside lifecycle methods.
6. **Clean Up `GameLayer` Members & Naming**: Remove `getCamera()` and `getContext()` from `GameLayer`, standardize protected members as `m_layerContext` and `m_camera`, initialize `m_camera` directly from `m_layerContext.window.getWidth()` and `getHeight()`, and standardize parameter names to `LayerContext& layerContext` across engine and demo layers.

## User Stories

1. As a game developer, I want `GameLayer` to receive `LayerContext& layerContext` in its constructor and store it in `m_layerContext`, so that my layer has direct access to all engine subsystems without a global singleton or redundant per-frame context wrappers.
2. As a game developer, I want `onUpdate(float deltaTime)` to take only the frame delta time, so that the update signature stays minimal while input and subsystems are accessed through `m_layerContext`.
3. As a game developer, I want `onRender()` to take no parameters, so that layers render using `m_layerContext.renderer` consistently with how they access all other subsystems.
4. As a game developer, I want `GameLayer` to expose `m_layerContext` and `m_camera` as protected members without redundant `getContext()` or `getCamera()` accessors, so that subclass code has a single, consistent way to reference layer state.
5. As a game developer, I want `m_camera` initialized directly from `m_layerContext.window.getWidth()` and `m_layerContext.window.getHeight()`, so that there is no dead fallback viewport logic inside `GameLayer`.
6. As a game developer, I want `GameLayer::onEvent` to update `m_camera` when `WindowResizedEvent` arrives, so that camera viewport dimensions stay synchronized with the window automatically.
7. As a game developer, I want to query keyboard and mouse state via `m_layerContext.input`, so that input queries read a deterministic event-driven state snapshot rather than polling GLFW globals.
8. As an engine maintainer, I want `Font::loadFromFile` to manage its own local `FT_Library` and `FT_Face` lifecycle and release both immediately after constructing the glyph atlas texture, so that FreeType headers and handles never leak into public engine interfaces or `Engine` state.
9. As an engine maintainer, I want `Gui` to accept `Window& window` in its constructor, so that ImGui initialization and per-frame updates do not depend on `Engine::get()`.
10. As an engine maintainer, I want `Engine::get()`, `Engine::m_instance`, and all individual subsystem getters removed from `Engine`, so that `Engine::getLayerContext()` is the single explicit entry point for accessing engine subsystems.
11. As an application author, I want the `onEngineStart(Engine& engine)` startup callback to register initial layers via `engine.getLayerContext().gameLayerManager`, so that application bootstrap works cleanly without individual getters on `Engine`.
12. As a contributor, I want all `LayerContext` constructor parameters consistently named `layerContext` across `GameLayer` and all demo layers, so that the codebase adheres uniformly to the project naming conventions.

## Implementation Decisions

- **Single Context Injection (`LayerContext`)**:
  - `LayerContext` aggregates references to `Window`, `Renderer`, `Audio`, `GameLayerManager`, `FontResources`, `TextureResources`, `AnimationResources`, and `Input`.
  - `FrameContext` is removed completely; its header and umbrella header inclusion are deleted.
  - `GameLayer` constructor accepts `explicit GameLayer(LayerContext& layerContext)` and stores `LayerContext& m_layerContext`.

- **`GameLayer` Interface & Camera Initialization**:
  - Lifecycle hooks:
    - `virtual void onAttach() {}`
    - `virtual void onDetach() {}`
    - `virtual void onUpdate(float deltaTime) {}`
    - `virtual void onRender() {}`
    - `virtual void onGuiRender() {}`
    - `virtual void onEvent(const Event& event)`
  - `getCamera()` and `getContext()` methods are removed from `GameLayer`.
  - Protected camera member is renamed from `camera` to `m_camera` (conforming to `m_camelCase`).
  - `GameLayer` initializes `m_camera` directly from `static_cast<float>(m_layerContext.window.getWidth())` and `static_cast<float>(m_layerContext.window.getHeight())` without fallback dimension helpers.
  - `GameLayer::onEvent` updates `m_camera.setViewportSize(...)` on `WindowResizedEvent` (left as-is until the dedicated event system refactor).

- **FreeType Encapsulation in `Font`**:
  - The standalone `FTLibrary` class is deleted and removed from the build configuration, umbrella header, and `Engine`.
  - `Font` removes `<freetype/freetype.h>` from its public header and removes the `FT_Face m_face` member variable.
  - `Font::loadFromFile` initializes a local `FT_Library` via `FT_Init_FreeType` and a local `FT_Face` via `FT_New_Face`, passes `FT_Face` to `calculateBitmap`, and calls `FT_Done_Face` and `FT_Done_FreeType` before returning.

- **`Gui` Window Injection**:
  - `Gui` constructor signature is updated to `explicit Gui(Window& window)`, storing `Window& m_window`.
  - `Gui` uses `m_window` during initialization and frame beginning instead of calling `Engine::get().getWindow()`.

- **`Engine` Singleton & Getter Removal**:
  - `Engine::get()` and `static Engine* m_instance` are removed.
  - Individual subsystem getters (`getAudio`, `getWindow`, `getRenderer`, `getFTLibary`, `getGameLayerManager`, `getInput`, `getFontResources`, `getTextureResources`, `getAnimationResources`) are removed from `Engine`.
  - `Engine` retains `LayerContext& getLayerContext()` as the sole accessor for its subsystems.
  - `Engine::run()` invokes `gameLayer->onUpdate(elapsed)` and `gameLayer->onRender()`.

- **Consistent Naming Across Demo Layers**:
  - All demo layers in `TestProject` standardize their constructor parameter name to `LayerContext& layerContext`, access `m_layerContext` and `m_camera`, implement `onUpdate(float deltaTime)` and `onRender()`, and restore helper signatures (such as `drawLines()`) to use `m_layerContext.renderer`.
  - Demo bootstrap in `onEngineStart` accesses `engine.getLayerContext().gameLayerManager`.

## Testing Decisions

- **Primary Seam (`LayerContext` / `Input` Seam)**:
  - The highest in-process seam is `GameLayer` constructed with a `LayerContext&` (and `Input::onEvent(const Event&)` for deterministic input state transitions).
  - Tests and verification exercise observable behavior through `onAttach()`, `onUpdate(float deltaTime)`, `onRender()`, and `onEvent(const Event&)` without relying on any global `Engine::get()` singleton.
- **Verification & Smoke Testing**:
  - Build and run the `TestProject` integration suite via the repository's build and smoke-test scripts (`./scripts/build.sh --run --timeout 5`) to verify font loading, ImGui rendering, layer switching, camera updates, and input handling with zero error logs.
  - Run `./scripts/lint-conventions.sh --changed-only` to verify formatting, header guards, and naming conventions.

## Out of Scope

- Refactoring the event dispatch architecture (`onEvent` and `WindowResizedEvent` handling remain as-is for a dedicated event system refactor).
- Multi-window or gamepad input support.
- Resource manager consolidation (covered by a separate architectural issue).

## Further Notes

- Architectural decision recorded in `docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`.
- Domain terminology synchronized in `GLOSSARY.md` and `AGENTS.md`.
