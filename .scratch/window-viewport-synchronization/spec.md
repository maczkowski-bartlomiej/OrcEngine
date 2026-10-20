# Spec: Window Viewport Synchronization

## Problem Statement

When running an application in OrcEngine, resizing the window or running on high-DPI displays causes visual distortion, stretched projections, or incomplete rendering bounds. Specifically, the low-level graphics pipeline (`glViewport`) is not explicitly initialized upon engine startup, and viewport updates are handled solely through native GLFW callbacks without a centralized engine owner. Furthermore, when the window is resized, only the currently active game layer receives the resize event; inactive layers that are registered with the layer manager retain stale camera viewport bounds. When switching to an inactive layer later, its scene is rendered with outdated camera dimensions, causing incorrect clipping or improper orthographic scaling.

## Solution

Centralize viewport management inside the `Renderer` subsystem and ensure end-to-end synchronization across window initialization, window resizing, and layer switching. `Renderer` will own an authoritative `setViewport` method wrapping `glViewport`, and `Window` will provide accurate framebuffer pixel dimensions to support high-DPI displays properly. `Engine` will configure the initial viewport on startup and update it upon window resize events. To ensure inactive layers never suffer from stale dimensions, `GameLayerManager` will automatically synchronize the layer's camera with current window bounds immediately before invoking `onAttach()`, maintaining a consistent 1:1 pixel coordinate world expansion.

## User Stories

1. As a game developer, I want the OpenGL viewport (`glViewport`) to be explicitly set upon engine startup, so that rendering output fills the initial window dimensions cleanly from frame one.
2. As a player using a High-DPI or Retina display, I want the renderer to use physical framebuffer dimensions for `glViewport`, so that the game does not render in a low-resolution or quarter-screen quadrant.
3. As an engine developer, I want `Renderer` to provide an authoritative `setViewport` method, so that graphics state is encapsulated in the rendering subsystem rather than hidden in low-level window callbacks.
4. As an engine developer, I want `Window` to expose physical framebuffer dimensions alongside logical window dimensions, so that subsystems requiring pixel dimensions can query them reliably.
5. As an engine coordinator, I want `Engine` to update `Renderer::setViewport` upon startup and when handling `WindowResizedEvent`, so that GPU viewport state remains synchronized with window changes.
6. As a game developer switching between scenes (e.g. from Menu to Gameplay or between demo layers), I want `GameLayerManager` to automatically synchronize the target layer's camera viewport with current window bounds before `onAttach()` runs, so that inactive layers never awaken with stale projection matrices.
7. As a game developer implementing a custom layer, I want `GameLayer::onAttach()` to receive a camera that is already synchronized to current window dimensions, so that I can inspect the true viewport or apply custom camera positioning/zoom without fighting outdated defaults.
8. As a game layer receiving a `WindowResizedEvent`, I want the camera viewport to update smoothly while active, maintaining 1:1 pixel coordinate world expansion.
9. As an agent and engine maintainer, I want all demo layers in `TestProject` (including `CameraTest`, `SpritesTest`, `Menu`) to maintain proper aspect ratios and rendering bounds during and after window resizes.
10. As an agent following project invariants, I want `AGENTS.md` and `GLOSSARY.md` updated to document `Renderer::setViewport`, framebuffer dimension queries, and automatic layer camera synchronization on switch.

## Implementation Decisions

- **Authoritative Renderer Viewport**:
  - The `Renderer` subsystem will expose a public `setViewport(uint32_t width, uint32_t height)` method (and an overload specifying `x, y, width, height`), which executes `glViewport`.
  - Direct `glViewport` calls in window callback lambdas will be replaced by coordination through the engine lifecycle and event system.
- **Window Framebuffer Dimensions**:
  - `Window` will track and expose physical framebuffer dimensions (querying `glfwGetFramebufferSize`) in addition to logical window sizes, ensuring correct pixel ratios on high-DPI displays.
- **Engine Coordination**:
  - During engine bootstrap, after `Window` and `Renderer` are initialized, `Engine` will invoke `Renderer::setViewport` with the initial framebuffer size.
  - In `Engine::onEvent`, when a window resize event is processed, `Engine` will update `Renderer::setViewport` before propagating the event to the active game layer.
- **Layer Camera Synchronization**:
  - `GameLayer` will expose a mechanism (`syncCameraWithWindow()`) that updates `m_camera`'s orthographic projection to match the current window dimensions.
  - Base `GameLayer::onEvent` will continue to handle `WindowResizedEvent` to adjust the active layer's camera viewport.
  - `GameLayerManager::setActiveLayer` will invoke camera synchronization on the incoming layer immediately before calling `onAttach()`. This guarantees inactive layers that were dormant during a resize event immediately adopt the correct viewport dimensions.
- **Coordinate System Policy**:
  - The camera projection will continue using 1:1 orthographic pixel coordinate mapping (`[0, 0]` at top-left to `[width, height]` at bottom-right), expanding visible world bounds rather than stretching or letterboxing.

## Testing Decisions

- **Definition of Good Test**: The verification must validate observable behavior: launching the engine with different initial dimensions configures the correct viewport, dispatching window resize events updates both the renderer viewport and active camera projection, and switching between layers in `TestProject` ensures the new layer's camera immediately matches current window bounds without distortion.
- **Modules Tested**: `Renderer`, `Window`, `GameLayer`, `GameLayerManager`, `Engine`, and `TestProject` layers.
- **Prior Art**: `TestProject` interactive layers (`CameraTest`, `Menu`, `SpritesTest`) and the unified verification gate (`.\scripts\verify.ps1`), which compiles the targets and executes the smoke test.

## Out of Scope

- Implementing letterbox/pillarbox fixed aspect ratio scaling modes (left for future camera/viewport mode extensions).
- Offscreen framebuffer / render target attachments (out of scope for window viewport synchronization).
- Multi-viewport or split-screen rendering.

## Further Notes

- This refactor directly fulfills issue [04] from `.scratch/ideas/issues/04-resizing-window-does-not-resize-viewport.md`.
- All changes must strictly pass `.\scripts\verify.ps1` before completing the feature.
