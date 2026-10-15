# Eliminate Legacy Vertex Structures and Untemplate Texturable

Status: ready-for-agent
Role: spec

## Problem Statement

OrcEngine's 2D rendering was previously fragmented across multiple specialized batchers and legacy per-shape vertex structs (`SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, `LineVertex`). While 2D rendering has now been consolidated into a single dynamic quad batcher, the legacy per-shape vertex structs remain in the engine, and `Texturable` remains a template (`Texturable<VertexType, VerticesCount>`).

Because of this template design:
1. Every draw call in `Renderer` performs costly per-vertex member-by-member field transcoding to unpack legacy vertex fields into dynamic batch vertices.
2. `Texturable` cannot serve as a clean, uniform base class for textured 2D elements.
3. Template bloat and inline boilerplate persist in engine headers to accommodate arbitrary vertex types when only 4-vertex quads are ever used.
4. Dead, unused vertex accessors clutter the public shape interface.

## Solution

Standardize all 2D quad geometry and batch submission around a single canonical `Vertex` struct (`orc::Vertex`). Untemplate `Texturable` into a clean, non-templated `Transformable` base class that directly holds a fixed array of 4 `Vertex` elements. Have 2D shapes (`Sprite`, `Rectangle`, `Circle`) inherit directly from `Texturable` and configure their shape parameters directly within their `Vertex` array. Decouple `Text` from `Texturable` while replacing `GlyphVertex` with `std::vector<Vertex>`. Streamline `Renderer` draw calls into direct fast-copy operations that transfer pre-configured `Vertex` quads into the dynamic batch buffer and stamp the batch-assigned texture index, eliminating per-field transcoding loops entirely. Prune all dead vertex accessors and completely eliminate all legacy vertex structs with zero compatibility cruft.

## User Stories

1. As an engine developer, I want a single canonical `Vertex` struct in the engine, so that I do not have to maintain separate, obsolete vertex layouts for each 2D shape.
2. As an engine developer, I want `Texturable` to be a non-templated class, so that 2D shapes share a concrete, uniform base class without template bloat or header instantiation overhead.
3. As an engine developer, I want `Texturable` to cache 4 `Vertex` elements directly, so that vertex transformations and UV coordinates are computed once upon transform changes rather than recomputed per frame.
4. As an engine developer, I want `Sprite`, `Rectangle`, and `Circle` to configure their vertex attributes directly into `Texturable`'s vertex array, so that shapes maintain fully prepared vertices for rendering.
5. As an engine developer, I want `Renderer` to fast-copy 4 `Vertex` elements directly into the batch buffer, so that draw calls avoid per-vertex field transcoding loops.
6. As a rendering system, I want `Renderer` to only stamp the assigned batch texture slot onto pre-built vertices during submission, so that batching overhead is minimal.
7. As an engine developer, I want `Text` to manage a dynamic `std::vector<Vertex>` rather than inheriting from `Texturable`, so that multi-glyph text layout does not compromise the fixed 4-vertex quad structure of `Texturable`.
8. As an engine developer, I want `Renderer::draw(const Text&)` to copy glyph vertices directly into the dynamic batch buffer in 4-vertex quad chunks, so that font glyphs are submitted with zero transcoding.
9. As an engine maintainer, I want `SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, and `LineVertex` deleted completely with no dummy structs or deprecated aliases, so that no legacy cruft remains in the codebase.
10. As an engine maintainer, I want unused methods `setVertices` and `getVertex` removed from `Texturable`, so that the API surface remains lean and focused only on active capabilities.
11. As an engine developer, I want `Texturable` implementation logic placed in a standard translation unit rather than an inline file, so that compile times are improved and engine source organization is consistent.
12. As a graphics programmer, I want `Sprite` to retain its texture requirement guard in `Renderer::draw`, so that sprites without a texture are safely skipped without corrupting batch state.
13. As a graphics programmer, I want `Rectangle` and `Circle` to render cleanly whether a texture is assigned or not, so that solid and textured geometric shapes both work seamlessly in the dynamic batcher.
14. As an engine user, I want existing `Renderer::draw` overloads for `Sprite`, `Rectangle`, `Circle`, and `Text` preserved, so that demo code and scene logic remain intuitive without artificial abstraction layers.
15. As a QA / build engineer, I want the unified vertex layout to compile cleanly across debug and release configurations without warnings, so that engine stability is guaranteed.
16. As a demo user, I want `TestProject` demo scenes (primitives, sprites, text) to produce identical visual output, so that the refactoring causes zero behavioral regressions.

## Implementation Decisions

- **Single Canonical Vertex Definition**: The engine defines a single universal 2D batch vertex structure named `Vertex` within the `orc` namespace. It encapsulates all per-vertex data required by the composite batch shader: world position, primary color (tint/fill), secondary color (border), texture coordinates, local quad position (for circle procedural rendering), texture sampler slot index, shape type identifier, and shape parameters (border thickness, inner radius, radius).
- **Untemplated Texturable Base Class**: Refactor `Texturable` into a non-templated abstract base class deriving from `Transformable`. It holds a fixed array of 4 `Vertex` instances, an optional texture reference, texture rectangle coordinates, and cached global rectangular bounds. The inline implementation file is removed in favor of a standard translation unit in the graphics source directory.
- **Shape Subclass Architecture**: `Sprite`, `Rectangle`, and `Circle` inherit from the non-templated `Texturable`. Constructors and property setters (`setFillColor`, `setBorderColor`, `setRadius`, `setBorderThickness`, etc.) directly initialize and update their corresponding fields inside the 4 cached `Vertex` instances.
- **Decoupled Multi-Glyph Text Component**: `Text` inherits directly from `Transformable` (not `Texturable`) and stores an array of `Vertex` instances representing glyph quads. In `Text::updateVertices`, each glyph quad is written directly as 4 `Vertex` instances with glyph position, normalized font atlas texture coordinates, and text shape identifier.
- **Renderer Fast-Copy Batch Submission**: In `Renderer::draw(const Sprite&)`, `Renderer::draw(const Rectangle&)`, and `Renderer::draw(const Circle&)`, the 4 cached `Vertex` elements are copied directly into the dynamic batch buffer followed by setting `textureIndex` to the assigned texture slot. In `Renderer::draw(const Text&)`, glyph quads are copied into the batch buffer in 4-vertex chunks with the font bitmap slot stamped.
- **No Extra Renderer API Abstractions**: Do not introduce a polymorphic `Renderer::draw(const Texturable&)` overload. Retain only the concrete, existing overloads (`Sprite`, `Rectangle`, `Circle`, `Text`) to prevent unnecessary engine API growth.
- **Aggressive Pruning of Dead APIs and Legacy Structs**: `SpriteVertex`, `RectangleVertex`, `CircleVertex`, `GlyphVertex`, and `LineVertex` are removed completely from the codebase without retaining typedefs or dummy structs. Unused vertex mutation methods (`setVertices`) and single-index accessors (`getVertex`) on `Texturable` are pruned.

## Testing Decisions

- **Behavioral and Integration Testing Focus**: Verify functionality through external rendering output, build validation, and demo scene execution rather than testing internal vertex struct layouts.
- **Compilation Across Presets**: Verify compilation across both debug and release configurations (`windows-debug` and `windows-release`) to guarantee template elimination and clean linking.
- **Demo Scene Execution**: Run `TestProject` to exercise `PrimitivesDemo` (rectangles, circles, lines), `SpriteDemo` (animated and static sprites), and `TextDemo` (string glyph rendering) to ensure identical visual behavior and 0 console/runtime errors.
- **Code Style and Conventions**: Run `scripts/lint-conventions.ps1` to enforce 0 violations with tab indentation, naming conventions, and header formatting.

## Out of Scope

- Introducing 3D mesh vertex structures or unbatched 3D geometry pipelines.
- Adding a polymorphic `Renderer::draw(const Texturable&)` interface.
- Modifying shader code or alter dynamic batch shader stage uniform bindings.

## Further Notes

- Architecture decision recorded in `docs/adr/0002-unify-vertex-and-untemplate-texturable.md`.
- Canonical domain term `Texturable` updated in `GLOSSARY.md`.
