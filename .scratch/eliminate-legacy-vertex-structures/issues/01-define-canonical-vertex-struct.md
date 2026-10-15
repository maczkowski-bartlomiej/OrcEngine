# 01: Define Canonical Vertex Struct and Update Renderer Batch Buffer

Status: resolved
Type: task

**What to build:**
Define the single canonical `orc::Vertex` structure in `OrcEngine/include/Orc/Graphics/Vertex.hpp` matching dynamic batch quad shader expectations, and update `Renderer`'s dynamic batch buffer to use `Vertex`.

**Blocked by:** None (can start immediately)

## Acceptance criteria

- [ ] `orc::Vertex` is defined in `OrcEngine/include/Orc/Graphics/Vertex.hpp` with position, color, secondaryColor, textureCoord, localPosition, textureIndex, shapeType, borderThickness, innerRadius, and radius.
- [ ] `Renderer` dynamic quad buffer `m_quadVertices` uses `Vertex` (with temporary alias or coexistence with `QuadVertex` for non-breaking incremental migration).
- [ ] Engine and demo compile cleanly.

## Comments

## Answer

Defined canonical orc::Vertex in Vertex.hpp and updated Renderer dynamic batch buffer to use Vertex.
