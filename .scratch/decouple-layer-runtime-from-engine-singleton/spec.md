# Spec: Decouple Layer Runtime from Global Engine Singleton

Status: ready-for-agent

## Problem Statement

Game layers and input polling were tightly coupled to the global `Engine::get()` singleton. When any `GameLayer` was instantiated, its base constructor reached into `Engine::get()` to bind references to seven subsystems (`Audio`, `Window`, `Renderer`, `GameLayerManager`, `FontResources`, `TextureResources`, `AnimationResources`) and fetch window dimensions for camera initialization. Furthermore, static input helpers (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`, `Mouse::getPosition`) bypassed the layer lifecycle entirely to poll GLFW native window handles through the engine singleton.

This ambient singleton coupling caused severe architectural friction:
1. Game layers could not be instantiated, updated, or verified in isolated unit test harnesses without spinning up a live desktop window, an OpenGL 4.6 context, and the FMOD audio engine.
2. Input polling queried volatile OS window state asynchronously mid-frame, causing intra-frame state inconsistencies and making it impossible to simulate, record, or inject input for automated testing.
3. Dependencies were hidden inside constructors rather than explicitly declared at the module boundary.
4. The engine runner reached into active layer internals during window resizing, breaking camera encapsulation.

## Solution

Decouple the game layer runtime and input handling from the engine singleton by introducing explicit context seams:
1. **Constructor Reference Injection**: `GameLayer` requires an explicit `LayerContext&` reference in its constructor, guaranteeing non-null access to persistent engine subsystems without singleton reach-ins.
2. **Per-Frame Update Seam**: `GameLayer::onUpdate` accepts a `const FrameContext&` bundling delta time, an immutable input state snapshot, and a reference to the active `LayerContext`.
3. **Explicit Render Target**: `GameLayer::onRender` accepts `Renderer&` directly, making the drawing capability explicit at the scene submission seam.
4. **Decoupled Input Subsystem**: An engine-managed `Input` service accumulates key and mouse states purely via window events, presenting a stable frame snapshot and allowing input to be tested headlessly.
5. **Encapsulated Viewport Synchronization**: `GameLayer` synchronizes its camera dimensions from `LayerContext` during construction and handles `WindowResizedEvent` directly inside `onEvent`, eliminating external engine mutations of layer cameras.

## User Stories

1. As a game developer, I want to instantiate a `GameLayer` with a mock `LayerContext`, so that I can unit test game logic in a headless CI environment without creating a graphical window or initializing audio hardware.
2. As a game developer, I want `GameLayer` to store a non-null reference to `LayerContext`, so that my layer subclasses can safely access resources, audio, and window services in their constructors without defensive null checks.
3. As a game developer, I want `onUpdate` to receive a `const FrameContext&`, so that delta time, input state, and engine services are delivered uniformly without reaching into ambient globals.
4. As a game developer, I want `FrameContext` to expose `deltaTime`, `input`, and `context`, so that per-frame updates have immediate access to everything required to simulate gameplay.
5. As a game developer, I want `onRender` to accept `Renderer&` directly, so that rendering dependencies are explicit and scoped to scene submission.
6. As a game developer, I want to query key states via `ctx.input.isKeyPressed(key)`, so that my gameplay logic queries a stable snapshot of input rather than polling OS state.
7. As a game developer, I want to query mouse button states via `ctx.input.isMouseButtonPressed(button)`, so that mouse interactions are deterministic and repeatable across frames.
8. As a game developer, I want to retrieve cursor position via `ctx.input.getMousePosition()`, so that aiming, picking, and cursor math remain consistent throughout the frame update.
9. As a tester, I want to feed synthetic events into an `Input` instance, so that I can simulate complex user input sequences headlessly without an operating system window.
10. As a tester, I want to feed custom delta time and input states into `GameLayer::onUpdate`, so that I can deterministically test edge cases like pause, frame drops, or rapid button toggles.
11. As an engine maintainer, I want static input polling functions (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`, `Mouse::getPosition`) eliminated from the public API, so that developers cannot accidentally re-introduce hidden singleton couplings.
12. As a game developer, I want `Keyboard.hpp` and `Mouse.hpp` to retain their key and button enum definitions, so that existing keycode constants (`Key::A`, `Button::Left`) remain stable and easy to use.
13. As an engine maintainer, I want `Input` state to accumulate purely through window events (`KeyboardKeyPressedEvent`, `KeyboardKeyReleasedEvent`, `MouseMovedEvent`, `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`), so that input tracking is completely decoupled from GLFW C-APIs.
14. As an engine maintainer, I want `GameLayer`'s camera to initialize with fallback viewport dimensions if no window is active, so that tests never crash during layer instantiation.
15. As a game developer, I want `GameLayer`'s base `onEvent` to automatically update the layer camera on `WindowResizedEvent`, so that camera aspect ratios stay synchronized without engine reach-in.
16. As a game developer, I want to override `onEvent` in derived layers to customize viewport behavior (e.g., fixed-resolution letterboxing), so that the engine does not dictate camera projection updates.
17. As an engine maintainer, I want `Engine::run()` to construct and pass `FrameContext` to the active layer each frame, creating a single unified per-frame entry point.
18. As a demo author, I want `TestProject` demo layers to adopt the new context seams cleanly, so that all interactive tests serve as validated reference implementations.

## Implementation Decisions

- **`LayerContext` Structure**:
  - Encapsulates references to persistent engine subsystems:
    - `Window& window`
    - `Renderer& renderer`
    - `Audio& audio`
    - `GameLayerManager& gameLayerManager`
    - `FontResources& fontResources`
    - `TextureResources& textureResources`
    - `AnimationResources& animationResources`
    - `Input& input`
  - Owned by `Engine` and constructed once during engine initialization.
  - Exposes accessor `Engine::getLayerContext()`.

- **`GameLayer` Constructor Reference Injection**:
  - `GameLayer` constructor signature: `explicit GameLayer(LayerContext& context)`.
  - Member `LayerContext& m_context` stored as a protected reference.
  - Accessor `LayerContext& getContext()` provided for convenience.
  - Subclasses pass `context` via member initializer list: `MyLayer(LayerContext& context) : GameLayer(context)`.

- **`FrameContext` Structure**:
  - Lightweight value struct passed by `const&` to `onUpdate`:
    - `float deltaTime`
    - `const Input& input`
    - `LayerContext& context`

- **Layer Lifecycle Seam Modifications**:
  - `virtual void onUpdate(const FrameContext& ctx)` replaces `onUpdate(float dt)`.
  - `virtual void onRender(Renderer& renderer)` replaces `onRender()`.
  - `virtual void onAttach()` and `virtual void onDetach()` retained for activation hooks.
  - `virtual void onEvent(const Event& event)` retained for event dispatch.
  - `virtual void onGuiRender()` retained for ImGui integration.

- **`Input` Subsystem Architecture**:
  - Explicit engine service owned as `UniquePtr<Input>` inside `Engine`.
  - Registered as an event listener in `Engine::onEvent` or passed event notifications.
  - Internal state tracks:
    - Fixed boolean array/bitset for key states indexed by `Keyboard::Key`.
    - Fixed boolean array/bitset for mouse button states indexed by `Mouse::Button`.
    - `Vector2f` for current cursor position.
  - Query API:
    - `bool isKeyPressed(Keyboard::Key key) const`
    - `bool isMouseButtonPressed(Mouse::Button button) const`
    - `Vector2f getMousePosition() const`
    - `void onEvent(const Event& event)` (accumulates state from events)
    - `void reset()` (clears pressed states)
  - Static polling methods (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`, `Mouse::getPosition`) are removed from the public engine API.

- **Camera Lifecycle & Viewport Resizing**:
  - `GameLayer` constructor initializes `camera` using `m_context.window.getWidth()` and `getHeight()`, falling back to 1280x720 if dimensions are zero/uninitialized.
  - Base `GameLayer::onEvent(const Event& event)` intercepts `WindowResizedEvent` and invokes `camera.setViewportSize(...)`.
  - `Engine::onEvent` window resize handling stops mutating active layer camera directly.

- **`TestProject` Migration**:
  - Migrate all demo layers (`AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, `ScriptTest`, `SpritesTest`) to:
    - Accept `LayerContext& context` in constructors.
    - Query input via `ctx.input.isKeyPressed(...)` and `ctx.input.getMousePosition()` in `onUpdate(const FrameContext& ctx)`.
    - Accept `Renderer& renderer` in `onRender(Renderer& renderer)`.
    - Register peer layers in `Menu` passing `m_context`.

## Testing Decisions

- **Single Primary Seam**:
  - The in-process **`LayerContext` / `FrameContext` Seam**.
  - All unit testing of game layers occurs by instantiating layers with a mock or headless `LayerContext` and invoking `onUpdate(const FrameContext&)` or `onRender(Renderer&)`.
  - Good tests verify external observable behaviors: position updates, state transitions, layer switching requests, and event handling. They do not inspect private fields or mock internal counters.

- **`Input` Subsystem Seam**:
  - Verified by feeding synthetic `KeyboardKeyPressedEvent`, `KeyboardKeyReleasedEvent`, `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, and `MouseMovedEvent` directly into `Input::onEvent` and asserting on query results.
  - Zero platform or GLFW dependencies required for full input test coverage.

- **Prior Art & Integration Coverage**:
  - `TestProject` demo layers serve as visual and interactive smoke tests.
  - Verification via `./scripts/build.sh --run --timeout 5` confirms runtime stability, layer navigation, and logging without regressions.

## Out of Scope

- Multi-window or multi-display input routing.
- Gamepad, controller, or joystick abstraction (future input milestone).
- Asset manager unification or XML schema changes (belongs to Issue 03).
- Camera projection lazy matrix evaluation (belongs to Issue 04).

## Further Notes

- Architectural decision recorded in `docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`.
- Canonical domain vocabulary defined in `GLOSSARY.md` (`LayerContext`, `FrameContext`, `Input`).
- Tracked under `.scratch/architecture-review/issues/02-decouple-layer-runtime-from-engine-singleton.md`.
