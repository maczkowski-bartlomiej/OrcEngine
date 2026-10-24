# 02: Remove FrameContext, Simplify GameLayer Lifecycle and Members, and Synchronize Docs

Status: resolved
Type: task
Blocked by: 01

**What to build:** Remove `FrameContext` completely, restore `GameLayer::onUpdate(float deltaTime)` and parameterless `GameLayer::onRender()`, remove `getCamera()`, `getContext()`, and fallback viewport constants from `GameLayer`, standardize `GameLayer` protected members as `m_layerContext` and `m_camera`, standardize constructor parameter naming to `LayerContext& layerContext` across engine and demo layers, and synchronize project documentation (`AGENTS.md`, `GLOSSARY.md`, and ADR 0004).

- [x] `FrameContext` is completely removed from the engine and the umbrella header.
- [x] `GameLayer` constructor accepts `explicit GameLayer(LayerContext& layerContext)` and stores `LayerContext& m_layerContext` and `Camera m_camera` as protected members.
- [x] `GameLayer::getCamera()`, `GameLayer::getContext()`, and fallback viewport dimension constants/helpers are removed; `m_camera` is initialized directly from `m_layerContext.window.getWidth()` and `m_layerContext.window.getHeight()`.
- [x] `GameLayer` lifecycle methods use `virtual void onUpdate(float deltaTime) {}` and `virtual void onRender() {}`, invoked by `Engine::run()`.
- [x] All `TestProject` demo layers standardize constructor parameter naming to `LayerContext& layerContext`, implement `onUpdate(float deltaTime)` and `onRender()`, and access `m_layerContext` (`input`, `renderer`, etc.) and `m_camera` directly.
- [x] `AGENTS.md`, `GLOSSARY.md`, and ADR 0004 are synchronized with the finalized architecture and verified by building, running the demo smoke test, and passing the conventions linter with zero errors.

## Answer

Removed FrameContext, simplified GameLayer lifecycle to onUpdate(float) and onRender(), standardized m_layerContext and m_camera, updated all TestProject layers, and synchronized AGENTS.md, GLOSSARY.md, and ADR 0004.
