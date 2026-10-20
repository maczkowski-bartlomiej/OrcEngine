# 01: Add Renderer setViewport and Window Framebuffer Support

Status: completed
Type: task
Blocked by: none

**What to build:** An authoritative viewport setter in the rendering module and physical framebuffer dimension tracking in the window subsystem, ensuring correct viewport initialization on startup and accurate pixel scaling on high-DPI displays.

**Blocked by:** None (can start immediately)

- [x] `Renderer` provides public `setViewport(uint32_t width, uint32_t height)` and `setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)` encapsulating `glViewport`
- [x] `Window` tracks physical framebuffer dimensions via `glfwGetFramebufferSize` alongside logical dimensions, exposing accessors for pixel dimensions
- [x] `Window` eliminates direct `glViewport` calls inside its low-level framebuffer size callback
- [x] `Engine` explicitly initializes `Renderer::setViewport` during engine bootstrap with initial framebuffer dimensions
- [x] `Engine::onEvent` updates `Renderer::setViewport` upon `WindowResizedEvent` prior to propagating the event to game layers
- [x] The engine static library and demo compile and run with zero errors
