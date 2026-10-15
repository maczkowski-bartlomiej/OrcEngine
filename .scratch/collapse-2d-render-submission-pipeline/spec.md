# Spec: Collapse the 2D Render Submission Pipeline

Status: resolved

## Problem Statement

The engine's 2D rendering pipeline currently fragments batching across four separate shape structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) inside the general renderer, each duplicating vertex and texture slot tracking. Because batch flushes occur in a fixed type order (`Sprites` -> `Lines` -> `Circles` -> `Rectangles` -> `Glyphs`) rather than temporal submission order, overlapping primitives of different types render out of order. To work around this, the engine relies on an inverted depth hack (`1.0f / zIndex`) with depth testing, which breaks alpha blending and causes visual artifacts when transparent shapes overlap. Furthermore, the frame rendering lifecycle leaks across module boundaries: the engine runner calls `begin` and `end` around each layer, yet demo layers redundantly invoke `clear`, `begin`, and `end` within their render callbacks.

## Solution

Consolidate 2D primitive rendering behind a unified `Renderer2D` deep module that:
1. Replaces separate shape batchers with a single dynamic Quad Batcher that packs sprites, solid/bordered rectangles, circles, and font glyphs into one vertex buffer and index buffer.
2. Employs a composite quad shader to render all quad-based primitives in a single pass without premature pipeline flushes.
3. Preserves natural submission order, allowing transparent primitives to blend properly without depth-buffer z-index hacks.
4. Decouples the scene submission lifecycle from engine loop orchestration: the engine handles target buffer clearing, while layers explicitly control their Scene Scope using `beginScene(Camera)` and `endScene()`.
5. Retains unbatched mesh rendering on the general renderer, automatically flushing the 2D batcher when custom geometry is submitted to maintain temporal correctness.

## User Stories

1. As a game developer, I want all 2D quad-based shapes (sprites, rectangles, circles, and font glyphs) to batch into a single pipeline, so that draw calls are minimized across mixed UI and game scenes.
2. As a game developer, I want 2D primitives to render strictly in the order they were submitted, so that visual stacking directly matches caller execution.
3. As a game developer, I want transparent shapes of different types (such as text over a colored rectangle or a sprite over a circle) to blend naturally without background pixels being discarded.
4. As a game developer, I want the inverted z-index depth hack removed, so that 2D rendering no longer suffers from depth precision degradation or z-fighting.
5. As an engine maintainer, I want a dedicated `Renderer2D` deep module, so that 2D batching logic is cleanly separated from system-level window, context, and clear operations.
6. As a game developer, I want to explicitly define a Scene Scope with `beginScene(Camera)` and `endScene()`, so that I can render multiple camera views or dedicated orthographic UI overlays within the same layer.
7. As an engine maintainer, I want `Engine` to clear the framebuffer once per frame without prematurely invoking 2D scene boundaries, eliminating double `begin` and `end` invocations.
8. As a game developer, I want existing high-level shape objects (`Sprite`, `Circle`, `Rectangle`, `Text`) to remain supported via convenience overloads on `Renderer2D`, so that existing game entity code does not require redesign.
9. As a game developer, I want object transformation (translation, rotation, scale, origin) to be handled uniformly through `Transformable` matrix evaluations, avoiding redundant rotated shape methods.
10. As a game developer, I want to submit custom vertex arrays and shaders for special effects, with queued 2D batches automatically flushed before the unbatched draw to preserve layering.
11. As a game developer, I want lines to batch in an internal secondary buffer that flushes only when transitioning to quads or at scene end, so that debug gizmos do not trigger excessive draw calls.
12. As an engine maintainer, I want dynamic batch buffers to automatically flush and reset when vertex limits or texture slot capacities are reached, preventing buffer overflow on large scenes.
13. As an engine maintainer, I want texture unit bindings to be dynamically assigned across all quad primitives up to GPU limits, maximizing batching efficiency across multiple textures.

## Implementation Decisions

- **Dedicated `Renderer2D` Deep Module**:
  - Extract all 2D quad and line batching out of the general renderer into a dedicated `Renderer2D` class.
  - The module exposes a compact interface: initialization/shutdown, scene scoping (`beginScene`, `endScene`), flushing, high-level shape submissions (`draw(Sprite)`, `draw(Rectangle)`, `draw(Circle)`, `draw(Text)`), and line submissions (`drawLine`).
  - Low-level frame clearing (`clear`, `setClearColor`) and unbatched mesh drawing (`draw(VertexArray, Shader, Matrix)`) remain on the base `Renderer`.

- **Unified Quad Vertex & Composite Shader**:
  - Unify `SpriteVertex`, `RectangleVertex`, `CircleVertex`, and `GlyphVertex` into a single `QuadVertex` layout.
  - `QuadVertex` attributes:
    - `position` (`Vector3f`)
    - `color` (`Vector4f`)
    - `textureCoord` (`Vector2f`)
    - `textureIndex` (`float`)
    - `shapeType` (`float` encoding Sprite, Rectangle, Circle, or Glyph)
    - `shapeParams` (`Vector4f` carrying border thickness, inner radius, local quad coordinates, or corner radii)
  - A single composite 2D shader program handles texturing, border/radii evaluation, and SDF circle smoothing based on `shapeType`.

- **Natural Submission Order & Blend State**:
  - Disable OpenGL depth writes and depth testing for 2D batched scenes. Rely entirely on standard alpha blending (`GL_SRC_ALPHA`, `GL_ONE_MINUS_SRC_ALPHA`) and sequential painter's algorithm.
  - Remove all inverted z-index tracking (`zIndex`, `1.0f / zIndex`).

- **Scene Scope & Frame Lifecycle Decoupling**:
  - `Engine::run()` clears the color buffer at the start of each frame.
  - Remove `m_renderer->begin(gameLayer->getCamera())` and `m_renderer->end()` from `Engine::run()`.
  - Layers explicitly call `renderer2D.beginScene(camera)` to bind the camera view-projection matrix and `renderer2D.endScene()` to flush queued primitives.
  - Layers can open multiple scene scopes per frame (e.g., game world camera followed by UI camera).

- **Unbatched Mesh Boundary Handling**:
  - When an unbatched `draw(VertexArray, Shader, Matrix)` is called on the base renderer, any active batch in `Renderer2D` is flushed immediately before the unbatched draw call executes.

- **Line Batching**:
  - Maintain a separate lightweight `LineVertex` buffer inside `Renderer2D`.
  - When a line is submitted after quads, or quads are submitted after lines, the active batch is flushed to guarantee correct draw order between lines and shapes.

## Testing Decisions

- **Highest Seam**:
  - The public interface of `Renderer2D` (`beginScene`, `draw`, `endScene`, `flush`).
  - Good tests verify external observable behavior: batch flush counts, vertex submission counts, texture unit binding counts, and preservation of submission order.
  - Avoid testing internal private arrays or intermediate loop counters.

- **Test Double & Headless Execution**:
  - `Renderer2D` should support verification in a headless/mock OpenGL context or test double environment by decoupling vertex data generation from raw driver calls.

- **Prior Art & Integration Harness**:
  - `TestProject` demo layers (`SpritesTest`, `CirclesTest`, `RectanglesTest`, `AnimationTest`, `Menu`) serve as the primary visual integration verification.
  - Updating `TestProject` layers to use `Renderer2D::beginScene` / `endScene` provides immediate verification that sprite animations, circles, rectangles, text rendering, and input gizmos render with correct alpha blending and submission layering.

## Out of Scope

- 3D model/mesh loading or deferred 3D rendering pipelines.
- Custom post-processing framebuffer passes or render graph architecture.
- Font glyph SDF generation at runtime (FreeType glyph bitmap loading remains as-is).
- Dynamic growing vertex buffers on the GPU (preallocated fixed batch capacity with auto-flush on overflow is preserved).

## Further Notes

- Documented in ADR `docs/adr/0001-collapse-2d-render-submission-pipeline.md`.
- Canonical domain vocabulary defined in `GLOSSARY.md` (`Renderer2D`, `Scene Scope`, `Quad Batcher`, `Submission Order`).
