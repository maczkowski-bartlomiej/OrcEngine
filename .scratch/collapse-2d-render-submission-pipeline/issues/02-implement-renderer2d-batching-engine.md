# 02: Implement Renderer2D Batching Engine

**What to build:**
A dedicated `Renderer2D` deep module that manages a preallocated dynamic vertex/index buffer for quads, manages dynamic texture slots, batches lines, and auto-flushes on buffer saturation or scene boundary.

**Blocked by:** 01: Create Unified Quad Vertex and Composite Shader

**Status:** ready-for-agent

- [ ] Create `OrcEngine/include/Orc/Graphics/Renderer2D.hpp` and `OrcEngine/source/Orc/Graphics/Renderer2D.cpp`.
- [ ] Implement `beginScene(const Camera&)` and `endScene()`.
- [ ] Implement dynamic texture slot tracking across up to 32 texture units with auto-flush on slot saturation.
- [ ] Implement dynamic quad vertex buffering with auto-flush on vertex/index capacity limit.
- [ ] Implement lightweight line buffering in `Renderer2D::drawLine`.
