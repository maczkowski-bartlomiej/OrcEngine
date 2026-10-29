# 03: Relocate Rect<T> to Core, Repair width/height Field Semantics & Sync Architecture Docs

Status: resolved
Type: task
Blocked by: 01, 02

**What to build:** Relocate `Rect<T>` (`IntRect`, `FloatRect`) into Layer 0 (`Core`) alongside `Vector2<T>`, rename its third and fourth struct fields from `right` and `bottom` to `width` and `height` so field names match constructor and method semantics across all callers, and synchronize repository documentation and ADRs with the deepened Layer 0 architecture.

## Acceptance Criteria

- [x] `Rect<T>` (`IntRect`, `FloatRect`) is relocated to `Core` and its fields are named `left`, `top`, `width`, and `height`, with `getSize()` returning `Vector2<T>(width, height)` and `intersects()` using `width` and `height`.
- [x] All callers across `Texturable`, `Sprite`, `Text`, `Camera`, `Resources<Animation>`, and `TestProject` are updated to use `Rect<T>::width` and `Rect<T>::height` cleanly (including local UV bounds in `Texturable` and orthographic `Viewport` projection in `Camera`).
- [x] `GLOSSARY.md`, `AGENTS.md`, `CODING_STANDARDS.md`, and a new ADR in `docs/adr/` are updated to reflect the deepened `Core` Layer 0 module, `spdlog` private encapsulation seam, removal of `Utility` and `FontUtility`, and `Rect<T>` field semantics.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments

## Answer

Relocated Rect<T> to Core, renamed right/bottom fields to width/height across all callers, updated GLOSSARY.md, AGENTS.md, CODING_STANDARDS.md, and documented ADR 0011.
