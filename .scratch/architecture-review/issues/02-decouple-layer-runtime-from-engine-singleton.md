# Issue 02: Decouple Layer Runtime from Global Engine Singleton

Status: resolved

## Description

`Engine::get()` was used as a global service locator across `GameLayer`, `Keyboard`, `Mouse`, `Gui`, and `Font`. Additionally, the first pass of layer decoupling introduced redundant `FrameContext` and `onRender(Renderer&)` parameters, left unused subsystem getters and `FTLibrary` on `Engine`, kept `getCamera()` / `getContext()` and unprefixed `camera` on `GameLayer`, and included unnecessary fallback viewport dimensions.

## Objective

1. Inject `LayerContext& layerContext` into `GameLayer`'s constructor (`m_layerContext`) as the single channel for subsystem access; remove `FrameContext` completely.
2. Use `onUpdate(float deltaTime)` and parameterless `onRender()` on `GameLayer`, accessing `m_layerContext.input` and `m_layerContext.renderer` directly.
3. Remove `getCamera()` and `getContext()` from `GameLayer`, rename `camera` to `m_camera`, and initialize `m_camera` directly from `m_layerContext.window.getWidth()` and `getHeight()` without fallback constants.
4. Encapsulate `FT_Library` and `FT_Face` locally within `Font::loadFromFile` and eliminate the `FTLibrary` class.
5. Inject `Window&` into `Gui`'s constructor (`explicit Gui(Window& window)`).
6. Remove `Engine::get()`, `Engine::m_instance`, and all individual subsystem getters from `Engine`, leaving only `Engine::getLayerContext()`.
7. Synchronize `GLOSSARY.md`, `docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`, and `AGENTS.md`.

## Settled Decisions

- Full specification published at `.scratch/decouple-layer-runtime-from-engine-singleton/spec.md`.
- Architectural decision recorded in `docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`.
- Domain terms updated in `GLOSSARY.md`.

## Answer

Completed decoupling of layer runtime, Gui, and Font from Engine singleton; removed Engine::get(), FTLibrary, and FrameContext; synchronized docs.
