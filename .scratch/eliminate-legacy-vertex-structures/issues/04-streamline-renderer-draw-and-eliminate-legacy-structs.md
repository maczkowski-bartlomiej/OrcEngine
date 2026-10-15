# 04: Streamline Renderer Draw Fast-Copy and Eliminate Legacy Structs

Status: resolved
Type: task

**What to build:**
Streamline `Renderer::draw` for `Sprite`, `Rectangle`, `Circle`, and `Text` into fast bulk-copy submissions (`std::copy_n` or `std::memcpy`) that assign the batch texture slot with zero field transcoding. Delete legacy vertex structs (`SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, `LineVertex`) completely from `OrcEngine/include/Orc/Graphics/Vertex.hpp`.

**Blocked by:** 02, 03

## Acceptance criteria

- [ ] `Renderer::draw(const Sprite&)`, `draw(const Rectangle&)`, and `draw(const Circle&)` copy 4 `Vertex` elements into `m_quadVertices` and stamp `textureIndex`.
- [ ] `Renderer::draw(const Text&)` copies glyph `Vertex` quads into `m_quadVertices` and stamps `textureIndex`.
- [ ] All per-vertex field transcoding loops in `Renderer::draw` are eliminated.
- [ ] `SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, and `LineVertex` are removed completely from `OrcEngine/include/Orc/Graphics/Vertex.hpp`.
- [ ] `OrcEngine/include/Orc/Graphics/Vertex.hpp` contains only `Vertex` (`orc::Vertex`).
- [ ] Engine and demo compile cleanly across `windows-debug` and `windows-release`.
- [ ] `run-demo.ps1` runs with 0 errors.
- [ ] `lint-conventions.ps1 -ChangedOnly` passes with 0 violations.

## Comments

## Answer

Eliminated SpriteVertex, RectangleVertex, CircleVertex, GlyphVertex, LineVertex, and QuadVertex alias so Vertex.hpp contains only canonical orc::Vertex. Verified compilation and demo execution across debug and release configurations.
