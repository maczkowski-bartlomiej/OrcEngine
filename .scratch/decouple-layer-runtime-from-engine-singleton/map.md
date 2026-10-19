# Decouple Layer Runtime from Global Engine Singleton Map

## Notes
Complete decoupling of `GameLayer`, `Input`, `Gui`, and `Font` from `Engine::get()`, removing the engine singleton, `FTLibrary`, `FrameContext`, and redundant `GameLayer` getters/parameters.

## Decisions so far
- [02-remove-frame-context-simplify-game-layer-and-sync-docs](issues/02-remove-frame-context-simplify-game-layer-and-sync-docs.md): Removed FrameContext, simplified GameLayer lifecycle to onUpdate(float) and onRender(), standardized m_layerContext and m_camera, updated all TestProject layers, and synchronized AGENTS.md, GLOSSARY.md, and ADR 0004.
- [01-encapsulate-freetype-inject-window-into-gui-and-remove-engine-singleton](issues/01-encapsulate-freetype-inject-window-into-gui-and-remove-engine-singleton.md): Encapsulated local FT_Library/FT_Face in Font::loadFromFile, deleted FTLibrary, injected Window& into Gui, and removed Engine::get() and individual getters from Engine.
- ADR 0004: Decouple Layer Runtime from Engine Singleton (`docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`).
- Single injection channel via `LayerContext& layerContext` (`m_layerContext`); `FrameContext` eliminated.
- `GameLayer` uses `onUpdate(float deltaTime)`, parameterless `onRender()`, and protected `m_layerContext` / `m_camera` without `getCamera()` / `getContext()` or fallback viewport dimensions.
- `Font::loadFromFile` manages local `FT_Library` and `FT_Face`; `FTLibrary` deleted.
- `Gui` takes `Window& window` via constructor injection.
- `Engine::get()` and individual subsystem getters removed; only `Engine::getLayerContext()` remains.

## Fog
None. Architecture decisions locked during grilling.
