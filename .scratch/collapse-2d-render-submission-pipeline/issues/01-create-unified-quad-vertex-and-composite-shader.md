# 01: Create Unified Quad Vertex and Composite Shader

**What to build:**
A unified `QuadVertex` layout and composite shader that can render textured sprites, solid/bordered rectangles, procedural circles/rings, and font glyphs in a single pass without changing shaders.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

- [ ] Define `QuadVertex` with position, color, texture coordinates, texture slot index, shape type, and shape parameters.
- [ ] Create `composite2d.vert.glsl` and `composite2d.frag.glsl` supporting sprites, rectangles with borders, SDF circles with inner/outer radius, and font glyphs.
- [ ] Verify shader compiles cleanly and handles multi-texture slot sampling.
