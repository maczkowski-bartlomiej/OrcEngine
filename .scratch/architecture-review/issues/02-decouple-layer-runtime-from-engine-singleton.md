# Issue 02: Decouple Layer Runtime from Global Engine Singleton

Status: resolved

## Description

`Engine::get()` was used as a global service locator. `GameLayer` automatically pulled 7 engine references in its constructor, and static input helpers (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`, `Mouse::getPosition`) queried the native window directly through the singleton. This prevented headless unit testing and coupled game logic tightly to window/audio contexts.

## Objective

1. Adopt constructor reference injection of `LayerContext&` for `GameLayer`, eliminating parameterless constructor ambient reach-ins.
2. Pass an explicit `const FrameContext&` across `onUpdate` containing delta time, input state snapshot, and context reference.
3. Supply `Renderer&` directly to `GameLayer::onRender(Renderer& renderer)` at the scene submission seam.
4. Introduce an event-driven `Input` subsystem tracking key/button states and cursor coordinates without native GLFW polling.
5. Move camera viewport resizing into `GameLayer::onEvent` for `WindowResizedEvent`, decoupling camera dimensions from `Engine::onEvent`.
6. Record ADR 0004 (`docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`) and register `LayerContext`, `FrameContext`, and `Input` in `GLOSSARY.md`.

## Settled Decisions

- **Injection Model**: Constructor Reference Injection (`explicit GameLayer(LayerContext& context) : m_context(context)`). Guarantees non-null context throughout the layer's lifetime and permits resource loading directly in layer constructors.
- **Per-Frame Update**: `virtual void onUpdate(const FrameContext& ctx)` where `FrameContext` bundles `float deltaTime`, `const Input& input`, and `LayerContext& context`.
- **Render Seam**: `virtual void onRender(Renderer& renderer)` replaces parameterless `onRender()`.
- **Input Architecture**: Explicit `Input` service accumulates state via window callbacks/events. Eliminates static polling methods on `Keyboard` and `Mouse`.
- **Camera Resizing**: Initialized using context window dimensions (with fallback) and updated in `GameLayer::onEvent(WindowResizedEvent)`.

## Comments

- Architecture grilled and triaged. ADR 0004 recorded in `docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md`.
- Terms added to `GLOSSARY.md`.

## Answer

--answer
