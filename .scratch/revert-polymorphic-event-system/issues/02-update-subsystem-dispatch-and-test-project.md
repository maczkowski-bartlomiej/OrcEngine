# Update Subsystem Dispatch and TestProject Layers

Status: resolved
Type: task
Blocked by: 01

## Description

Update all call sites across `OrcEngine` and `TestProject` to dispatch and handle events using `event.getType()` and `getEvent<T>()`:

1. **`Engine.cpp`**:
   - `Engine::onEvent`: Call `m_input->onEvent(event)`, call `gameLayer->onEvent(event)`, and check `if (event.getType() == Event::Type::WindowClosed) m_running = false;`.
2. **`GameLayer.cpp`**:
   - In `GameLayer::onEvent`: Check `if (event.getType() == Event::Type::WindowResized)`, retrieve `const auto& resized = getEvent<WindowResizedEvent>(event);`, and update `m_camera.setViewportSize(...)`.
3. **`Input.cpp`**:
   - In `Input::onEvent`: Replace `event.visit` with a `switch (event.getType())` handling `KeyboardKeyPressed`, `KeyboardKeyReleased`, `MouseButtonPressed`, `MouseButtonReleased`, and `MouseMoved` via `getEvent<T>(event)`.
4. **`TestProject` Demo Layers**:
   - Replace all `event.getIf<orc::KeyboardKeyPressedEvent>()` and `event.visit(...)` occurrences in `CameraTest.cpp`, `AnimationTest.cpp`, `AudioTest.cpp`, `CirclesTest.cpp`, `InputTest.cpp`, `Menu.cpp`, `RectanglesTest.cpp`, and `SpritesTest.cpp` with `if (event.getType() == orc::Event::Type::...)` and `orc::getEvent<T>(event)`.
5. **Unified Verification**:
   - Run `./scripts/verify.sh` to ensure 100% clean conventions, compilation, and smoke test pass.

## Completion Criteria
- Engine and demo layers compile with zero warnings or errors.
- All tests and demo smoke runs pass 100%.

## Answer

Updated Engine, GameLayer, Input, and all TestProject layers to dispatch and handle events using event.getType() and getEvent<T>()
