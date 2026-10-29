# 04: Expose TextureResources, FontResources, Texture, Font, Sprite & Text Rendering to C#

Status: ready-for-agent
Type: task
Blocked by: 03

**What to build:** Complete the 2D rendering surface in C#:
- Read-only `TextureResources` / `FontResources` lookups that return non-owning `Orc.Texture` / `Orc.Font` handles backed by `const Ref<T>*` pointers into the resource maps.
- The `Texturable.Texture` property.
- `IDisposable` `Orc.Sprite` and `Orc.Text`.
- `Renderer.Draw(Sprite)` and `Renderer.Draw(Text)`.

See spec §3 ("Resource Handles") and §5.

## Acceptance Criteria

- [ ] **Resource handles**
  - `textureResourcesGet` / `fontResourcesGet` return `const Ref<Texture>*` / `const Ref<Font>*` that point at the `Ref<T>` stored in the resource map.
  - Glue functions copy that `Ref<T>` into the native drawable.
  - `Texture` and `Font` are **unchanged**: no `std::enable_shared_from_this`, no extra heap allocation.
- [ ] **Resource lookups**
  - `Context.TextureResources.GetResource(string) -> Texture` and `Context.FontResources.GetResource(string) -> Font` are exposed, and an unknown name fails fast.
  - `Texture` exposes `Width`, `Height`, and `Size` (via `textureGetSize`).
  - `Texture` and `Font` are non-owning and non-disposable.
- [ ] **Texturable texture:** `Orc.Texturable.Texture` is settable via `texturableSetTexture`. The getter returns the cached managed `Texture` without crossing the seam, and this works for `Sprite`, `Rectangle`, and `Circle`.
- [ ] **Sprite:** `Orc.Sprite` (`IDisposable`, no finalizer, inherits `Texturable`, with `Color`) and `Orc.Renderer.Draw(Sprite)` are exposed. Drawing a sprite without a texture fails fast (existing native check).
- [ ] **Text**
  - `Orc.Text` (`IDisposable`, no finalizer, inherits `Transformable`, with `Font`, `Color`, `TextString`, `LocalRect`, `GlobalRect`) and `Orc.Renderer.Draw(Text)` are exposed.
  - `Font` and `TextString` getters return values cached in C#.
- [ ] **Smoke test:** `SmokeTestScript` looks up a texture and a font, then creates, draws, and disposes a `Sprite` and a `Text`. The run exits cleanly via `Window::close()` with zero live wrappers (`Scripting: shutdown verified`).
- [ ] **Showcase:** `ScriptTestScript` renders sprites and text alongside shapes.
- [ ] **Docs:** ADR 0012 and `GLOSSARY.md` are updated if this slice refines any decision or term.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and graceful `TestProject` smoke test).

## Comments
