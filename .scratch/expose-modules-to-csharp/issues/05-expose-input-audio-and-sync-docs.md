# 05: Expose Input, Audio & Bus Subsystems to C#, Complete ScriptTest Showcase & Final Documentation Sync

Status: ready-for-agent
Type: task
Blocked by: 04

**What to build:** Expose `Orc.Input` (`Key`, `SpecialKeys`, `MouseButton`) and `Orc.Audio` / `Orc.Bus` (`IDisposable`) on `Orc.ScriptContext`, complete the interactive `ScriptTestScript` showcase in the existing `ScriptTest` layer, and perform the final documentation sync for the complete binding surface.

## Acceptance Criteria

- [ ] **Input enums & structs**
  - `Orc.Key` and `Orc.MouseButton` enums are exposed in `namespace Orc` with explicit numeric values mirroring `orc::Keyboard::Key` and `orc::Mouse::Button`.
  - The native glue `static_assert`s the value of the last enumerator of each enum to catch drift.
  - `Orc.SpecialKeys` (`Alt`, `Shift`, `System`, `Control`, `NumLock`, `CapsLock`) is a blittable struct that mirrors `InteropSpecialKeys` (1-byte `bool`s).
- [ ] **Input:** `Context.Input` exposes `IsKeyPressed(Key)`, `WasKeyJustPressed(Key)`, `WasKeyJustReleased(Key)`, `SpecialKeys`, `IsMouseButtonPressed(MouseButton)`, `WasMouseButtonJustPressed(MouseButton)`, `WasMouseButtonJustReleased(MouseButton)`, `MousePosition`, `MouseDelta`, and `MouseScrollDelta`.
- [ ] **Audio**
  - `Context.Audio` exposes `Play(string eventPath)` and `GetBus(string busName) -> Bus`.
  - `Orc.Bus` (`IDisposable`, no finalizer, registered in the live-wrapper registry) exposes `Stop()`, `Pause()`, `Resume()`, `Volume`, and `IsPaused`.
- [ ] **Smoke test**
  - `SmokeTestScript` queries keyboard and mouse state without needing input.
  - It gets and reads a `Bus` (`Volume`, `IsPaused`) without audible playback, then disposes it.
  - The run exits cleanly via `Window::close()` with zero live wrappers (`Scripting: shutdown verified`).
- [ ] **Showcase:** `ScriptTestScript` in `TestProject.Scripts` provides keyboard/mouse camera control, audio playback and bus control on key presses, plus rendering of sprites, text, shapes, and lines.
- [ ] **Final documentation sync:** `GLOSSARY.md`, `CODING_STANDARDS.md` (C# section), `AGENTS.md`, and `docs/adr/0012-csharp-module-bindings-and-script-lifecycle.md` reflect the complete binding surface, including the future `ResourceLoader` direction and its address-stability requirement for resource handles.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and graceful `TestProject` smoke test).

## Comments
