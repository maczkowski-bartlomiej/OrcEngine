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
The engine subsystem tracking per-frame keyboard, mouse button, modifier, cursor position, cursor delta, and scroll delta state snapshots decoupled from window handles and platform APIs.
_Avoid_: InputManager, InputState, RawInput

**Event**:
The pure enum class representing discrete window lifecycle occurrences (`WindowResized`, `WindowClosed`) dispatched by value through `onEvent`.
_Avoid_: EventVariant, EventDispatcher, ActionEvent, InputMessage, PolymorphicEvent

**Core**:
The foundational Layer 0 module providing universal primitives, platform macros, diagnostics and fail-fast crash contracts, time measurement, pseudo-random generation, string-view hashing, 2D geometric rectangles, memory ownership aliases, math vectors/matrices, and semantic version constants decoupled from engine lifecycle coordination.
_Avoid_: Engine/Core, EngineCore, Base, Common, UtilityModule

**Viewport**:
The rasterization rectangle configured authoritatively via `Renderer::setViewport`, synchronizing OpenGL `glViewport` with the window's physical framebuffer bounds.
_Avoid_: RenderArea, ScreenBounds, WindowRect

**Framebuffer Dimensions**:
The physical pixel resolution of the window rendering surface queried via `Window::getFramebufferSize()`, distinct from logical window screen coordinates on high-DPI and Retina displays.
_Avoid_: WindowPixels, ScreenResolution, RenderTargetSize

**Scripting**:
The engine subsystem responsible for hosting the .NET 10 CoreCLR runtime via `hostfxr`, loading managed assemblies, and binding native-managed entry points with unconditional fail-fast crash semantics.
_Avoid_: Mono, ScriptEngine, DotNetManager, CSharpRuntime

**Fail-Fast Crash Contract**:
The engine-wide invariant that reaching any invalid state immediately logs a fatal error and terminates the process in both Debug and Release configurations, guaranteeing that all live engine objects and resources are valid without defensive state checks or boolean error returns.
_Avoid_: Soft assert, recoverable error return, defensive null guard, silent fallback
