# OrcEngine

OrcEngine is a 2D/3D game engine and runtime built in C++20 with OpenGL and FMOD.

## Language

**Renderer**:
The unified rendering module responsible for scene lifecycle, dynamic quad batching, line rendering as quads, and unbatched mesh drawing.
_Avoid_: Renderer2D, BatchManager, RenderContext, SpriteRenderer

**Scene Scope**:
The active submission window bounded by `beginScene()` and `endScene()` that binds a camera's view-projection matrix to queued draw calls.
_Avoid_: Frame lifecycle, render pass, frame scope

**Quad Batcher**:
The unified dynamic vertex accumulator inside `Renderer` that aggregates sprites, rectangles, circles, font glyphs, and lines into a single vertex and index buffer.
_Avoid_: Shape batcher, sprite batcher, primitive pipeline

**Submission Order**:
The temporal order in which drawing commands are submitted by layers, naturally preserved by appending to the dynamic batch buffer without depth-buffer z-index inversions.
_Avoid_: Z-index ordering, depth sorting

**Texturable**:
A 2D transformable quad element holding texture coordinates, bounds, and four `Vertex` elements directly compatible with dynamic quad batching.
_Avoid_: TexturedQuad, ShapeVertexArray, QuadDrawable

**LayerContext**:
The persistent environment context supplied to a layer via constructor injection, granting scoped access to engine subsystems and resources.
_Avoid_: EngineContext, SystemRegistry, ServiceLocator

**Input**:
The engine subsystem tracking keyboard, mouse, and cursor state snapshots decoupled from window handles and platform APIs.
_Avoid_: InputManager, InputState, RawInput

**Event**:
The lightweight polymorphic base class representing discrete window, keyboard, and mouse occurrences dispatched through `onEvent` using `getType()` and `getEvent<T>()`.
_Avoid_: EventVariant, EventDispatcher, ActionEvent, InputMessage
