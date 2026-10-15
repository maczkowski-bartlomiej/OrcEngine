# 03: Add Shape Overloads and Unbatched Boundary Flush

**What to build:**
Convenience overloads on `Renderer2D` for `Sprite`, `Circle`, `Rectangle`, and `Text` that extract transformed vertices and submit them to the quad batcher, plus link `Renderer::draw(VertexArray, Shader, Matrix)` to flush any active `Renderer2D` scene before unbatched draws.

**Blocked by:** 02: Implement Renderer2D Batching Engine

**Status:** ready-for-agent

- [ ] Implement `Renderer2D::draw(const Sprite&)` decomposing sprite vertices into `QuadVertex` stream.
- [ ] Implement `Renderer2D::draw(const Rectangle&)` passing border thickness and colors.
- [ ] Implement `Renderer2D::draw(const Circle&)` passing radius, inner radius, and border properties.
- [ ] Implement `Renderer2D::draw(const Text&)` batching glyphs with font atlas textures.
- [ ] Ensure calling `Renderer::draw(VertexArray, Shader, Matrix)` triggers `Renderer2D::flush()` when a 2D scene is active.
