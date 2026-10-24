# Centralize Viewport Ownership and Synchronize Layer Cameras

Previously, OpenGL viewport configuration (`glViewport`) was handled ad-hoc inside a native GLFW callback within `Window.cpp` without being explicitly initialized upon engine startup. Furthermore, window pixel dimensions were conflated with screen coordinates on high-DPI displays. When the window was resized, only the active game layer received the resize event, leaving inactive layers with stale camera viewport dimensions. When switching to an inactive layer later, rendering suffered from clipped or distorted projections.

We decided to centralize viewport ownership in `Renderer` and ensure automatic camera synchronization across layers:
- `Renderer::setViewport`: Exposes authoritative viewport configuration wrapping `glViewport` with explicit type conversions, decoupling window callbacks from raw OpenGL calls.
- `Window` Framebuffer Queries: `Window` tracks physical framebuffer dimensions via `glfwGetFramebufferSize` (`getFramebufferWidth()`, `getFramebufferHeight()`, `getFramebufferSize()`) alongside logical dimensions, ensuring correct 1:1 pixel scaling on high-DPI and Retina displays.
- `Engine` Bootstrap and Resize Dispatch: `Engine` explicitly configures `Renderer::setViewport` on startup using the initial framebuffer dimensions and updates it upon `WindowResizedEvent` prior to forwarding the event to the active layer.
- `GameLayer::syncCameraWithWindow`: Provides an explicit synchronization method updating `m_camera`'s orthographic projection bounds to match the current window dimensions.
- Pre-Attach Layer Camera Synchronization: `GameLayerManager::setActiveLayer` automatically invokes `syncCameraWithWindow()` on incoming layers immediately before calling `onAttach()`, ensuring inactive layers never awaken with stale projections.
