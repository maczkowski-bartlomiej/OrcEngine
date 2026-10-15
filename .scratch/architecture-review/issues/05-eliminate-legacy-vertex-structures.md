# Eliminate Legacy Vertex Structures and Untemplate Texturable

Status: resolved
Type: task

## Description

Following the collapse of the 2D rendering pipeline into `Renderer` with dynamic batching and `QuadVertex`, the legacy per-shape vertex structures in `OrcEngine/include/Orc/Graphics/Vertex.hpp` (`SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, `LineVertex`) are obsolete. None of these legacy structures are uploaded to the GPU anymore.

However, `Texturable` is currently implemented as a template:
```cpp
template<typename VertexType, uint64_t VerticesCount>
class Texturable : public Transformable
```
Because of this template design:
1. `Sprite` inherits from `Texturable<SpriteVertex, 4>`, `Rectangle` from `Texturable<RectangleVertex, 4>`, and `Circle` from `Texturable<CircleVertex, 4>`.
2. Every call to `Renderer::draw(const Sprite&)`, `Renderer::draw(const Rectangle&)`, and `Renderer::draw(const Circle&)` must manually iterate through 4 legacy vertices and transcode each field into `QuadVertex` before appending to `m_quadVertices`.
3. `Texturable` cannot be used as a non-templated polymorphic interface or uniform reference for textured 2D elements.
4. Redundant boilerplate and template bloat exists in `Texturable.hpp` and `Texturable.inl` to accommodate arbitrary `VertexType` merely to update vertex UV coordinates and global rect bounds.

## Objective

1. **Untemplate `Texturable`**:
   - Refactor `Texturable` into a clean, non-templated class (`class Texturable : public Transformable`) or base drawable component.
   - Standardize internal quad geometry representation around `QuadVertex` (or remove redundant vertex caching from shapes in favor of direct quad generation / properties).
2. **Eliminate Legacy Vertex Structures**:
   - Remove `SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, and `LineVertex` from `OrcEngine/include/Orc/Graphics/Vertex.hpp`.
   - Standardize all 2D quad operations exclusively on `QuadVertex`.
3. **Streamline `Renderer::draw` Overloads**:
   - Eliminate per-vertex field transcoding loops in `Renderer::draw()`.
   - Allow fast bulk copy (`std::memcpy` or direct writes) of 4 `QuadVertex` elements into the batch buffer.
4. **Update Shape Classes**:
   - Refactor `Sprite`, `Rectangle`, `Circle`, and `Text` to use the untemplated `Texturable` and unified vertex layout.

## Acceptance Criteria

- [ ] `Vertex.hpp` contains only `Vertex` (`orc::Vertex`).
- [ ] `Texturable` is non-templated (`class Texturable : public Transformable`) holding `std::array<Vertex, 4>`.
- [ ] `Sprite`, `Rectangle`, and `Circle` inherit from non-templated `Texturable`.
- [ ] `Text` uses `std::vector<Vertex>` directly and remains decoupled from `Texturable`.
- [ ] `Renderer::draw(...)` fast-copies `Vertex` directly and stamps `textureIndex` without field-by-field transcoding.
- [ ] Legacy vertex structures (`SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, `LineVertex`) are deleted without dummy structs or typedefs.
- [ ] All `TestProject` demo scenes and engine builds (`windows-debug`, `windows-release`, and Linux presets) build and run without warnings or regressions.
- [ ] `lint-conventions.ps1` passes with 0 violations.

## Comments

- Grilling session completed and architectural decisions locked:
  - Spec produced at `.scratch/eliminate-legacy-vertex-structures/spec.md`.
  - Architecture decision recorded at `docs/adr/0002-unify-vertex-and-untemplate-texturable.md`.
  - Domain glossary updated at `GLOSSARY.md`.


## Answer

Implemented spec eliminate-legacy-vertex-structures on integration branch feature/eliminate-legacy-vertex-structures across 4 tickets. Unified 2D geometry around canonical orc::Vertex, untemplated Texturable, decoupled Text to std::vector<Vertex>, enabled fast-copy batch submission in Renderer, and completely deleted all legacy vertex structures.
