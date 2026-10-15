# Issue 02: Decouple Layer Runtime from Global Engine Singleton

Status: needs-triage

## Description

`Engine::get()` is used as a global service locator. `GameLayer` automatically pulls 7 engine references in its constructor, and static input helpers (`Keyboard::isKeyPressed`, `Mouse::isButtonPressed`) query the native window directly through the singleton. This prevents headless unit testing and couples game logic tightly to window/audio contexts.

## Objective

1. Pass an explicit `FrameContext` across the layer lifecycle seam (`onUpdate`, `onRender`) containing delta time, input state, and scoped resources.
2. Decouple input polling from GLFW window handles so input states can be provided or simulated.
3. Allow layers to be instantiated and verified in isolated test harnesses.

See `.scratch/architecture-review/ideas.md#candidate-2-decouple-layer-runtime-from-global-engine-singleton` for detailed analysis and grilling questions.

## Comments
