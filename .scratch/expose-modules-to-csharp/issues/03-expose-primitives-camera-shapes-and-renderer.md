# 03: Expose Core Value Primitives, Layer Camera, Transformable, Texturable, Shapes (Rectangle, Circle) & Renderer Shape Drawing to C#

Status: ready-for-agent
Type: task
Blocked by: 02

**What to build:** Cut the 2D geometric rendering and camera slice from C# `Orc.Script.OnRender()` (already inside the automatic `Scene Scope` from ticket 02) through `Orc.Renderer` to OpenGL. Expose:
- Value structs: `Vector2f`, `Vector3f`, `Color`, `FloatRect`.
- The non-owning layer camera on `Context.Camera`.
- Base classes: `Orc.Transformable`, `Orc.Texturable`.
- `IDisposable` shapes: `Orc.Rectangle`, `Orc.Circle`.
- `Orc.Renderer`: `SetClearColor`, `Draw(Rectangle)`, `Draw(Circle)`, `DrawLine`.

See spec §3 and §5.

## Acceptance Criteria

- [ ] **Value structs**
  - `Vector2f`, `Vector3f`, `Color`, and `FloatRect` are `[StructLayout(LayoutKind.Sequential)]` structs in `namespace Orc`.
  - They provide arithmetic/equality operators and the geometric helpers that exist in C++ (`Position`, `Size`, `Intersects`).
  - They cross the seam only through the POD interop mirror structs.
  - `Vector2i`, `Vector2u`, `Vector4f`, `IntRect`, `UIntRect`, and `Contains` are **not** added.
- [ ] **Layer camera**
  - `Context.Camera` is a non-owning, non-disposable handle to the owning `GameLayer::m_camera`.
  - It exposes `Position`, `Rotation`, `Zoom`, `Move`, `Rotate`, and `ApplyZoom`.
  - It exposes no constructor, `Dispose`, or `SetViewportSize`, so ADR 0007 window synchronization stays authoritative.
- [ ] **Transformable & Texturable**
  - `Orc.Transformable` exposes `Position`, `Rotation`, `Scale`, `Origin`, `Move`, `Rotate`, and `ApplyScale`.
  - `Orc.Texturable` exposes `TextureRect`, `LocalRect`, and `GlobalRect` (`Texture` arrives in ticket 04).
  - Both are backed by `Transformable*` / `Texturable*` handles from `InteropDrawableHandles`, filled by real C++ upcasts; C++ never casts `void*` to a base class.
- [ ] **Shapes**
  - `Orc.Rectangle` (`Size`, `FillColor`, `BorderColor`, `BorderThickness`) and `Orc.Circle` (`Radius`, `InnerRadius`, `FillColor`, `BorderColor`, `BorderThickness`) are `IDisposable` with no finalizer.
  - Each is registered in the live-wrapper registry and released through `transformableDestroy`.
  - Use after `Dispose()` fails fast.
- [ ] **Renderer:** `Context.Renderer` exposes `SetClearColor(Color)`, `Draw(Rectangle)`, `Draw(Circle)`, `DrawLine(Vector2f, Vector2f, Color, float)`, and `DrawLine(Vector3f, Vector3f, Color, float)`. `Clear`, `SetViewport`, `BeginScene`, `EndScene`, and `Flush` are **not** exposed.
- [ ] **Smoke test**
  - `SmokeTestScript` creates, transforms, draws (in a small corner region), and disposes a `Rectangle` and a `Circle`, and draws a line.
  - It mutates and restores `Context.Camera`.
  - The run exits cleanly via `Window::close()` with zero live wrappers (`Scripting: shutdown verified`).
- [ ] **Showcase:** `ScriptTestScript` renders shapes and lines.
- [ ] **Docs:** ADR 0012 and `GLOSSARY.md` are updated if this slice refines any decision or term.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and graceful `TestProject` smoke test).

## Comments
