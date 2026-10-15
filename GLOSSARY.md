# OrcEngine

OrcEngine is a 2D/3D game engine and runtime built in C++20 with OpenGL and FMOD.

## Language

**Renderer2D**:
The dedicated batch submission module responsible for buffering, sorting, and flushing 2D geometry within a camera pass.
_Avoid_: BatchManager, RenderContext, SpriteRenderer

**Scene Scope**:
The active submission window bounded by `beginScene()` and `endScene()` that binds a camera's view-projection matrix to queued draw calls.
_Avoid_: Frame lifecycle, render pass, frame scope

**Quad Batcher**:
The unified dynamic vertex accumulator inside `Renderer2D` that aggregates sprites, rectangles, circles, and font glyphs into a single vertex and index buffer.
_Avoid_: Shape batcher, sprite batcher, primitive pipeline

**Submission Order**:
The temporal order in which drawing commands are submitted by layers, naturally preserved by appending to the dynamic batch buffer without depth-buffer z-index inversions.
_Avoid_: Z-index ordering, depth sorting
