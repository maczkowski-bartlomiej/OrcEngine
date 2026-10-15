# 04: Rendering Section for the Unified Quad Batcher

Status: resolved
Type: task
Blocked by: 03

**What to build:** Write the rendering subsystem subsection of Section 4 in `AGENTS.md` so it accurately describes the current **Renderer**: a single **Quad Batcher** accumulating sprites, rectangles, circles, glyphs, and lines into one vertex/index buffer of unified `Vertex` elements, drawn in natural **Submission Order** within a **Scene Scope**. It must align with ADR 0001 and ADR 0002 and use `GLOSSARY.md` vocabulary.

## Acceptance criteria

- [ ] Describes the Renderer, Scene Scope, Quad Batcher, Submission Order, and Texturable using the glossary terms (and none of their `_Avoid_` aliases).
- [ ] Describes the unified `Vertex` layout, texture slot allocation and automatic flush conditions, and how lines and unbatched meshes are handled — all verified against current source.
- [ ] Contains no mention of per-shape vertex arrays/batches, per-shape vertex structs, or depth-buffer z-index inversion as current behaviour.
- [ ] Describes the `Transformable` transform caching as it currently exists.
- [ ] Links to ADR 0001 and ADR 0002 for rationale.
- [ ] Replaces the placeholder left by ticket 03 and fits the lifecycle diagram's naming.
- [ ] If writing the section reveals a missing or outdated glossary term, `GLOSSARY.md` is updated in the same change.

## Answer

Authored Section 4.4 documenting the dynamic quad batcher, Scene Scope, canonical Vertex layout, natural submission order, oriented line quads, unbatched geometry flush, and links to ADR 0001 and ADR 0002.
