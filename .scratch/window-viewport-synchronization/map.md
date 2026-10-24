# Window Viewport Synchronization Map

## Notes
Centralized viewport and framebuffer dimensions management, synchronizing renderer and layer camera projection bounds across window resize and layer transitions.

## Decisions so far
- **Authoritative Renderer Viewport**: `Renderer::setViewport(width, height)` and `setViewport(x, y, width, height)` encapsulate low-level `glViewport` calls.
- **Physical Framebuffer Tracking**: `Window` tracks framebuffer dimensions via `glfwGetFramebufferSize` (`getFramebufferWidth()`, `getFramebufferHeight()`, `getFramebufferSize()`), eliminating direct `glViewport` calls in window callbacks.
- **Engine Viewport Bootstrap & Dispatch**: `Engine` sets the initial viewport during startup and updates `Renderer::setViewport` on `WindowResizedEvent` prior to forwarding events to active layers.
- **Pre-Attach Layer Camera Sync**: `GameLayer` exposes `syncCameraWithWindow()`, and `GameLayerManager::setActiveLayer` calls it on the target layer immediately before `onAttach()`.
- **Domain Docs and ADR**: Created ADR 0007 (`0007-centralize-viewport-and-synchronize-camera.md`), updated `GLOSSARY.md` with Viewport and Framebuffer Dimensions, and updated `AGENTS.md`.

## Fog
None. All components verified clean.
