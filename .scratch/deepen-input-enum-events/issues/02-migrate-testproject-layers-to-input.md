# 02: Migrate TestProject Layers to Query Input in onUpdate & Remove Layer onEvent Overrides

Status: resolved
Type: task
Blocked by: 01

**What to build:** Migrate all `TestProject` layers (`AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, `SpritesTest`, `ScriptTest`) so that discrete `Left`/`Right` arrow-key layer navigation and scroll-wheel camera zooming are driven through `LayerContext::input` inside `onUpdate`, expand `InputTest` to demonstrate the deepened `Input` capabilities, and remove all `onEvent` overrides from `TestProject`.

## Acceptance Criteria

- [x] `AnimationTest`, `AudioTest`, `CameraTest`, `CirclesTest`, `InputTest`, `Menu`, `RectanglesTest`, and `SpritesTest` switch active layers on `Left` and `Right` arrow keys inside `onUpdate` using `m_layerContext.input.wasKeyJustPressed`.
- [x] `CameraTest` zooms its camera inside `onUpdate` using `m_layerContext.input.getMouseScrollDelta()`.
- [x] `InputTest` exercises and displays held keys, rising/falling key edges (`wasKeyJustPressed` / `wasKeyJustReleased`), `Keyboard::SpecialKeys`, mouse button states/edges, cursor position/delta, and scroll wheel delta via `LayerContext::input`.
- [x] All `onEvent` method declarations and definitions are deleted from all 9 `TestProject` `GameLayer` subclasses.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments
