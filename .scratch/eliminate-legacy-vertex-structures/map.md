# Eliminate Legacy Vertex Structures and Untemplate Texturable Map

## Notes
Consolidating all 2D quad geometry around a single canonical `orc::Vertex` struct and untemplating `Texturable`.

## Decisions so far
- [04-streamline-renderer-draw-and-eliminate-legacy-structs](issues/04-streamline-renderer-draw-and-eliminate-legacy-structs.md): Eliminated SpriteVertex, RectangleVertex, CircleVertex, GlyphVertex, LineVertex, and QuadVertex alias so Vertex.hpp contains only canonical orc::Vertex. Verified compilation and demo execution across debug and release configurations.
- [03-decouple-text-to-use-vertex-vector](issues/03-decouple-text-to-use-vertex-vector.md): Decoupled Text from GlyphVertex to use dynamic std::vector<Vertex>, populating Vertex quads directly and fast-copying them in Renderer::draw(const Text&).
- [02-untemplate-texturable-and-refactor-shapes](issues/02-untemplate-texturable-and-refactor-shapes.md): Untemplated Texturable to hold 4 Vertex instances directly, implemented methods in Texturable.cpp, deleted Texturable.inl, and updated Sprite, Rectangle, and Circle to inherit from Texturable and configure Vertex fields directly.
- [01-define-canonical-vertex-struct](issues/01-define-canonical-vertex-struct.md): Defined canonical orc::Vertex in Vertex.hpp and updated Renderer dynamic batch buffer to use Vertex.
- ADR 0002: Unify Vertex and Untemplate Texturable (`docs/adr/0002-unify-vertex-and-untemplate-texturable.md`).
- Single canonical `Vertex` struct in `orc` namespace.
- Untemplated `Texturable` storing `std::array<Vertex, 4>`.
- `Text` stores `std::vector<Vertex>` directly.
- Direct fast-copy submission in `Renderer` with zero per-field transcoding loops.

## Fog
None. Architecture decisions locked during grilling.
