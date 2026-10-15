# Eliminate Legacy Vertex Structures and Untemplate Texturable Map

## Notes
Consolidating all 2D quad geometry around a single canonical `orc::Vertex` struct and untemplating `Texturable`.

## Decisions so far
- [01-define-canonical-vertex-struct](issues/01-define-canonical-vertex-struct.md): Defined canonical orc::Vertex in Vertex.hpp and updated Renderer dynamic batch buffer to use Vertex.
- ADR 0002: Unify Vertex and Untemplate Texturable (`docs/adr/0002-unify-vertex-and-untemplate-texturable.md`).
- Single canonical `Vertex` struct in `orc` namespace.
- Untemplated `Texturable` storing `std::array<Vertex, 4>`.
- `Text` stores `std::vector<Vertex>` directly.
- Direct fast-copy submission in `Renderer` with zero per-field transcoding loops.

## Fog
None. Architecture decisions locked during grilling.
