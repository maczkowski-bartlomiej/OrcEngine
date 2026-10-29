# Deepen Input and Collapse Event to Pure Enum

Status: ready-for-agent

## Problem Statement

Game layers in `OrcEngine` currently face a split-brain paradigm between `Input` and `Event`. Because the `Input` module is shallow—tracking only whether a keyboard key or mouse button is currently held down (`isKeyPressed`, `isMouseButtonPressed`) and the current cursor position (`getMousePosition`)—it cannot answer edge-triggered queries ("was this key just pressed or released this frame?"), mouse button edge queries, mouse cursor movement delta queries, mouse wheel scroll delta queries, or modifier key state (`Keyboard::SpecialKeys`).

As a result, every `GameLayer` that needs single-frame key press detection (such as switching active layers on `Left`/`Right` arrow keys across all `TestProject` layers) or scroll wheel zooming (`CameraTest`) is forced to override `onEvent(const Event&)` and perform runtime `dynamic_cast` downcasts via `getEvent<T>(event)` on polymorphic `Event` subclasses (`KeyboardKeyPressedEvent`, `MouseWheelScrolledEvent`). Worse, `KeyboardKeyReleasedEvent`, `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, and `MouseMovedEvent` exist purely as transient pass-through wrappers between `Window` GLFW callbacks and `Input::onEvent`.

Meanwhile:
1. The remaining non-input events—`WindowResizedEvent` and `WindowClosedEvent`—never have their struct payloads read by any caller (`Engine` and `GameLayer` already query `Window::getFramebufferSize()` and `Window` dimensions directly per ADR 0007).
2. `Window` registers both window-size and framebuffer-size GLFW callbacks that each emit `WindowResized`, causing duplicate resize notifications for a single resize occurrence.
3. `GameLayer::onEvent` handles `WindowResized` by calling `syncCameraWithWindow()`, which imposes a fragile base-call ordering constraint where every derived `GameLayer` overriding `onEvent` must remember to call `GameLayer::onEvent(event)` first or silently break camera aspect-ratio synchronization.
4. Public input headers (`Keyboard`, `Mouse`) leak GLFW-specific adapter translation functions (`orc::glfw::glfwKeyToOrcKey`, `orc::glfw::glfwKeyModsToOrcSpecialKeys`, `orc::glfw::glfwButtonToOrcButton`) to all engine consumers.

## Solution

Deepen the `Input` module into a complete per-frame input snapshot and edge-detection subsystem, eliminate all polymorphic keyboard, mouse, and window event structs, seal GLFW translation helpers behind an internal engine seam (while preserving `Keyboard::SpecialKeys`), centralize camera resize synchronization in `Engine`, and collapse `Event` into a pure `enum class`.

Specifically:
1. **Deepen `Input` (Candidate 1)**:
   - Track current-frame and previous-frame key and mouse button state arrays, current and previous cursor positions (or per-frame cursor delta), per-frame mouse wheel scroll deltas, and `Keyboard::SpecialKeys` state inside `Input`.
   - Expose rich query methods on `Input` through `LayerContext`: held state (`isKeyPressed`, `isMouseButtonPressed`), rising-edge state (`wasKeyJustPressed`, `wasMouseButtonJustPressed`), falling-edge state (`wasKeyJustReleased`, `wasMouseButtonJustReleased`), modifier state (`getSpecialKeys`), cursor position and frame delta (`getMousePosition`, `getMouseDelta`), and scroll wheel delta (`getMouseScrollDelta`).
   - Advance `Input`'s frame snapshot once per frame prior to polling window events so edge transitions and per-frame deltas are deterministic across the entire `GameLayer::onUpdate` tick.
   - Feed keyboard, mouse button, modifier (`SpecialKeys`), cursor position, and scroll wheel updates from `Window` into `Input` without allocating or downcasting polymorphic `Event` subclasses.
   - Migrate all `TestProject` layers (`AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, `SpritesTest`, `ScriptTest`) to query `Input` inside `onUpdate` and delete their `onEvent` overrides.
2. **Collapse `Event` to a Pure Enum (Candidate 1)**:
   - Remove all derived `Event` structs (`KeyboardKeyPressedEvent`, `KeyboardKeyReleasedEvent`, `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, `MouseMovedEvent`, `MouseWheelScrolledEvent`, `WindowResizedEvent`, `WindowClosedEvent`) and their headers, along with `getEvent<T>()`.
   - Replace `class Event` with `enum class Event` containing only discrete window lifecycle occurrences (`Invalid`, `WindowResized`, `WindowClosed`).
3. **Seal GLFW Translation Helpers & Keep `SpecialKeys` (Candidate 2)**:
   - Keep `Keyboard::SpecialKeys` in the public `Keyboard` domain interface and expose it via `Input::getSpecialKeys()`.
   - Move `orc::glfw` translation helpers (`glfwKeyToOrcKey`, `glfwKeyModsToOrcSpecialKeys`, `glfwButtonToOrcButton`) out of the public `Keyboard` and `Mouse` headers into an internal engine header so public consumers only see domain types.
4. **Centralize Camera Resize Sync in `Engine` & Deduplicate Resize Callbacks (Candidate 3)**:
   - Have `Engine` invoke `gameLayer->syncCameraWithWindow()` directly alongside `Renderer::setViewport` when `Event::WindowResized` occurs (matching how `GameLayerManager::setActiveLayer` already calls `syncCameraWithWindow()`), making `GameLayer::onEvent(Event)` an optional no-op virtual hook with no mandatory base-call trap.
   - Deduplicate `Window`'s resize callbacks so a single window resize emits one `Event::WindowResized` notification after updating both window dimensions and `Framebuffer Dimensions`.

## User Stories

1. As a game developer writing a `GameLayer`, I want to query `input.wasKeyJustPressed(key)` inside `onUpdate`, so that I can trigger discrete actions (such as jumping, toggling UI, or switching layers) once per key press without overriding `onEvent`.
2. As a game developer writing a `GameLayer`, I want to query `input.wasKeyJustReleased(key)` inside `onUpdate`, so that I can detect the exact frame a player releases a key (such as releasing a charged attack).
3. As a game developer writing a `GameLayer`, I want to query `input.isKeyPressed(key)` inside `onUpdate`, so that I can continue handling continuous held movement keys seamlessly.
4. As a game developer writing a `GameLayer`, I want to query `input.getSpecialKeys()` on `Input`, so that I can inspect active keyboard modifiers (`alt`, `shift`, `system`, `control`, `numLock`, `capsLock`) during `onUpdate` without inspecting key events.
5. As a game developer writing a `GameLayer`, I want to query `input.isMouseButtonPressed(button)` inside `onUpdate`, so that I can detect continuous mouse button holds (such as dragging or continuous firing).
6. As a game developer writing a `GameLayer`, I want to query `input.wasMouseButtonJustPressed(button)` inside `onUpdate`, so that I can handle single-click actions in the same update loop as keyboard logic.
7. As a game developer writing a `GameLayer`, I want to query `input.wasMouseButtonJustReleased(button)` inside `onUpdate`, so that I can detect the frame a mouse drag or click finishes.
8. As a game developer writing a `GameLayer`, I want to query `input.getMousePosition()` and `input.getMouseDelta()` inside `onUpdate`, so that I can track both absolute cursor coordinates and per-frame cursor movement.
9. As a game developer writing a `GameLayer`, I want to query `input.getMouseScrollDelta()` inside `onUpdate`, so that I can control camera zoom or scrollable views without overriding `onEvent` and downcasting `MouseWheelScrolledEvent`.
10. As a game developer creating a new `GameLayer` subclass, I want camera aspect-ratio synchronization on window resize to be coordinated automatically by `Engine`, so that I never break camera projection by forgetting to call `GameLayer::onEvent(event)` in a derived layer.
11. As a game developer overriding `GameLayer::onEvent(Event)` for custom window lifecycle reactions, I want `Event` to be a simple `enum class` passed by value, so that I can switch on `Event::WindowResized` or `Event::WindowClosed` without `dynamic_cast` or extra event headers.
12. As an engine maintainer, I want `Window` to feed raw key, modifier, mouse button, cursor, and scroll state directly to `Input` (or an `Input` callback/receiver), so that input state updates do not construct polymorphic event objects or perform `dynamic_cast` checks on every mouse move or key press.
13. As an engine maintainer, I want `Input` to transition its frame state (`beginFrame` / `transitionFrame`) immediately before `Window` polls OS events, so that `wasKeyJustPressed`, `wasKeyJustReleased`, `getMouseDelta`, and `getMouseScrollDelta` accurately reflect changes over exactly one frame.
14. As an engine maintainer, I want `orc::glfw` key, modifier, and mouse button mapping functions hidden from the public `Keyboard` and `Mouse` headers, so that the public engine interface remains decoupled from GLFW implementation details.
15. As an engine maintainer, I want `Window` resize callbacks to update both logical window size and physical `Framebuffer Dimensions` without firing duplicate `Event::WindowResized` notifications per resize, so that `Viewport` and camera updates run once per resize.
16. As an engine maintainer, I want `TestProject` layers (`AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, `SpritesTest`, `ScriptTest`) to handle all keyboard navigation and camera zooming through `LayerContext::input` in `onUpdate`, so that `TestProject` serves as a clean reference implementation of the deepened `Input` module.
17. As an engine maintainer, I want `GLOSSARY.md`, `AGENTS.md`, `CODING_STANDARDS.md`, and the architecture decision records in `docs/adr/` synchronized with the pure-enum `Event` and deepened `Input` design, so that future sessions and agents follow the updated domain language and invariants.

## Implementation Decisions

- **`Input` Module Interface & State Model**:
  - `Input` maintains two state buffers for keyboard keys (`m_currentKeys`, `m_previousKeys`) of size `MAX_KEYS` and two state buffers for mouse buttons (`m_currentMouseButtons`, `m_previousMouseButtons`) of size `MAX_MOUSE_BUTTONS`.
  - `Input` maintains `Keyboard::SpecialKeys m_specialKeys = {}` updated whenever keyboard state changes.
  - `Input` maintains `Vector2f m_mousePosition = {0.0f, 0.0f}`, `Vector2f m_previousMousePosition = {0.0f, 0.0f}`, and `Vector2f m_mouseScrollDelta = {0.0f, 0.0f}`.
  - Public query interface on `Input`:
    - `bool isKeyPressed(Keyboard::Key key) const` — true while `key` is down in the current frame.
    - `bool wasKeyJustPressed(Keyboard::Key key) const` — true when `key` is down in the current frame and was up in the previous frame.
    - `bool wasKeyJustReleased(Keyboard::Key key) const` — true when `key` is up in the current frame and was down in the previous frame.
    - `const Keyboard::SpecialKeys& getSpecialKeys() const` — returns the current `Keyboard::SpecialKeys` snapshot.
    - `bool isMouseButtonPressed(Mouse::Button button) const` — true while `button` is down in the current frame.
    - `bool wasMouseButtonJustPressed(Mouse::Button button) const` — true when `button` is down in the current frame and was up in the previous frame.
    - `bool wasMouseButtonJustReleased(Mouse::Button button) const` — true when `button` is up in the current frame and was down in the previous frame.
    - `const Vector2f& getMousePosition() const` — returns current cursor coordinates.
    - `Vector2f getMouseDelta() const` — returns `m_mousePosition - m_previousMousePosition`.
    - `const Vector2f& getMouseScrollDelta() const` — returns the scroll wheel offset (`x`, `y`) accumulated during the current frame.
  - Lifecycle and mutation methods on `Input`:
    - `void beginFrame()` (or `transitionFrame()`): copies `m_currentKeys` into `m_previousKeys`, copies `m_currentMouseButtons` into `m_previousMouseButtons`, sets `m_previousMousePosition = m_mousePosition`, and resets `m_mouseScrollDelta = {0.0f, 0.0f}`.
    - `void reset()`: clears all current/previous key and button arrays, resets `m_specialKeys`, and zeroes scroll deltas.
    - Direct state mutators called by the engine/window seam (`onKeyPressed`, `onKeyReleased`, `onMouseButtonPressed`, `onMouseButtonReleased`, `onMouseMoved`, `onMouseScrolled`, or `setKeyState`, `setSpecialKeys`, `setMouseButtonState`, `setMousePosition`, `addMouseScrollDelta`) replace `Input::onEvent(const Event&)`.
- **Pure Enum `Event` Module**:
  - Supersedes ADR 0005 (`0005-revert-to-polymorphic-event-hierarchy.md`): because `Input` absorbs all keyboard and mouse state and `WindowResized` callers query `Window` directly, `Event` no longer requires polymorphism, virtual dispatch, or `getEvent<T>()` downcasting.
  - `Event` becomes `enum class Event { Invalid = -1, WindowResized, WindowClosed };`.
  - All derived event headers (`KeyboardEvents`, `MouseEvents`, `WindowEvents`) are deleted and removed from the umbrella header (`Orc.hpp`).
  - `Window::EventCallback` and `Engine::onEvent` and `GameLayer::onEvent` take `Event` by value (`void onEvent(Event event)`), following the primitives/enums parameter passing rule in `CODING_STANDARDS.md`.
- **`Window` to `Input` & `Engine` Seam**:
  - `Engine` initializes `m_input` before `m_window` (or wires `m_input` into `m_window`) so `Window`'s GLFW callbacks can never fire against an uninitialized `Input` instance.
  - `Window` holds a reference or pointer to `Input` (passed via constructor injection or explicit setter alongside `setEventCallback`) and directly updates `Input` inside `glfwSetKeyCallback`, `glfwSetMouseButtonCallback`, `glfwSetCursorPosCallback`, and `glfwSetScrollCallback`.
  - `Input::beginFrame()` is invoked each frame immediately before `glfwPollEvents()` so that all GLFW callbacks triggered during event polling record edges and scroll deltas for the upcoming frame update.
  - `Window`'s resize callbacks (`glfwSetWindowSizeCallback` and `glfwSetFramebufferSizeCallback`) are consolidated so `Window` updates both logical dimensions and `Framebuffer Dimensions` and emits `Event::WindowResized` once per resize without duplicate firing.
- **Centralized Camera Resize Sync in `Engine`**:
  - When `Engine::onEvent(Event::WindowResized)` runs, `Engine` updates `m_renderer->setViewport(m_window->getFramebufferWidth(), m_window->getFramebufferHeight())` and, if an active `GameLayer` exists, invokes `gameLayer->syncCameraWithWindow()` before calling `gameLayer->onEvent(event)`.
  - `GameLayer::onEvent(Event event)` becomes an empty virtual hook (`virtual void onEvent(Event event) {}`), eliminating the fragile requirement for derived `GameLayer` classes to call `GameLayer::onEvent(event)`.
- **Sealing GLFW Translation Helpers & Preserving `Keyboard::SpecialKeys`**:
  - `Keyboard::SpecialKeys` remains a public struct inside `Keyboard`.
  - `orc::glfw::glfwKeyToOrcKey`, `orc::glfw::glfwKeyModsToOrcSpecialKeys`, and `orc::glfw::glfwButtonToOrcButton` are removed from the public `Keyboard` and `Mouse` headers and placed in an internal header not exported by `Orc.hpp`.
- **`TestProject` Layer Migration**:
  - All `TestProject` layers remove `onEvent` overrides.
  - `Left` and `Right` arrow key layer switching in `AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, and `SpritesTest` moves into `onUpdate(float deltaTime)` using `m_layerContext.input.wasKeyJustPressed(orc::Keyboard::Key::Left)` and `wasKeyJustPressed(orc::Keyboard::Key::Right)`.
  - `CameraTest` moves scroll-wheel camera zooming into `onUpdate(float deltaTime)` using `m_layerContext.input.getMouseScrollDelta().y`.
  - `InputTest` is expanded to exercise and demonstrate the deepened `Input` capabilities (held keys, just-pressed/just-released edges, `SpecialKeys`, mouse buttons, cursor delta, and scroll delta).
- **Documentation & Standards Synchronization**:
  - Update `GLOSSARY.md` (`Event` and `Input` definitions), `AGENTS.md` (initialization order and ADR pointers), `CODING_STANDARDS.md` (naming table examples referencing deleted event structs), and record the superseding ADR in `docs/adr/`.

## Testing Decisions

- **What Makes a Good Test**:
  - Tests must exercise the external behavior of `Input`, `Event`, and `GameLayer` through their public interfaces (`LayerContext::input` queries during `GameLayer::onUpdate`, and `Engine` / `Window` lifecycle execution), never asserting on private arrays or internal GLFW translation tables.
- **Primary Testing Seam (Single High-Level Seam)**:
  - **The `LayerContext::input` & `GameLayer::onUpdate` seam**: Callers and `TestProject` layers interact with input exclusively through `Input`'s public query interface (`isKeyPressed`, `wasKeyJustPressed`, `wasKeyJustReleased`, `getSpecialKeys`, `isMouseButtonPressed`, `wasMouseButtonJustPressed`, `wasMouseButtonJustReleased`, `getMousePosition`, `getMouseDelta`, `getMouseScrollDelta`) across frame boundaries (`beginFrame()`).
- **Modules to Be Tested & Verified**:
  - `Input` module: state transitions across frames (idle $\rightarrow$ just pressed $\rightarrow$ held $\rightarrow$ just released $\rightarrow$ idle), `SpecialKeys` modifier tracking, cursor delta reset on next frame, and scroll delta accumulation and reset on next frame.
  - `TestProject` integration & smoke verification (`InputTest`, `CameraTest`, and layer navigation across all 9 `TestProject` layers via `scripts/verify.ps1`).
- **Prior Art in the Codebase**:
  - Unified verification gate (`.\scripts\verify.ps1`): convention linter (`.\scripts\lint-conventions.ps1`), zero-warning Debug/Release build (`.\scripts\build.ps1`), and 5-second `TestProject` runtime smoke test (`.\scripts\run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog`).

## Out of Scope

- Gamepad / controller input support.
- Action mapping / keybinding configuration serialization (mapping physical keys to named game actions).
- Text input / Unicode character stream events (`glfwSetCharCallback`), as ImGui already hooks GLFW character callbacks directly for GUI text fields.
- Changing how Dear ImGui (`Gui`) chains its own internal GLFW callbacks.

## Further Notes

- Supersedes ADR 0005 (`Revert Event System to Polymorphic Hierarchy with Safe Downcasting`). A new ADR will document the deepening of `Input` and the collapse of `Event` into a pure `enum class`.
