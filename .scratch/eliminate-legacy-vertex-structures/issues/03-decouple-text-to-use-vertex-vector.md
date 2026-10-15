# 03: Decouple Text to Use Dynamic Vertex Vector

Status: ready-for-agent
Type: task

**What to build:**
Refactor `Text` in `OrcEngine/include/Orc/Graphics/Text.hpp` and `OrcEngine/source/Orc/Graphics/Text.cpp` to store and populate `std::vector<Vertex>` directly instead of legacy `GlyphVertex`.

**Blocked by:** 01

## Acceptance criteria

- [ ] `Text.hpp` declares `const std::vector<Vertex>& getVertices() const` and `mutable std::vector<Vertex> m_vertices`.
- [ ] `Text::updateVertices()` generates `Vertex` quads with `shapeType = 3.0f`, primary `color`, transformed positions, and glyph atlas UV coordinates.
- [ ] No references to `GlyphVertex` remain in `Text.hpp` or `Text.cpp`.

## Comments
