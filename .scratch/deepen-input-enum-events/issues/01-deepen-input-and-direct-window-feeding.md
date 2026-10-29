# 01: Deepen Input with Frame-Edge State, SpecialKeys, Deltas & Direct Window Feeding

Status: resolved
Type: task
Blocked by: none

**What to build:** Deepen the `Input` module so callers can query single-frame key and mouse button transitions (`wasKeyJustPressed`, `wasKeyJustReleased`, `wasMouseButtonJustPressed`, `wasMouseButtonJustReleased`), keyboard modifier state (`getSpecialKeys`), per-frame cursor movement delta (`getMouseDelta`), and per-frame scroll wheel delta (`getMouseScrollDelta`) across frame boundaries (`beginFrame`), fed directly from `Window` with GLFW translation helpers sealed behind an internal engine seam.

## Acceptance Criteria

- [x] `Input` tracks current-frame and previous-frame key state arrays (`MAX_KEYS`) and mouse button state arrays (`MAX_MOUSE_BUTTONS`), exposing `isKeyPressed`, `wasKeyJustPressed`, `wasKeyJustReleased`, `isMouseButtonPressed`, `wasMouseButtonJustPressed`, and `wasMouseButtonJustReleased`.
- [x] `Input` tracks `Keyboard::SpecialKeys` (`alt`, `shift`, `system`, `control`, `numLock`, `capsLock`) and exposes `getSpecialKeys() const`.
- [x] `Input` tracks current and previous mouse cursor positions and per-frame mouse wheel scroll offsets, exposing `getMousePosition() const`, `getMouseDelta() const`, and `getMouseScrollDelta() const`.
- [x] `Input::beginFrame()` copies current key, mouse button, and cursor state into previous-frame state and zeroes the per-frame scroll delta accumulator immediately before `Window` polls OS events each frame.
- [x] `Engine` constructs `Input` before `Window` and wires `Window`'s GLFW callbacks to update `Input` directly for key presses/releases, modifier keys, mouse button presses/releases, cursor movement, and scroll wheel offsets.
- [x] `Keyboard::SpecialKeys` remains part of the public `Keyboard` domain interface, while `orc::glfw` translation helpers (`glfwKeyToOrcKey`, `glfwKeyModsToOrcSpecialKeys`, `glfwButtonToOrcButton`) are moved out of public headers into an internal engine header.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments
