# 03: Collapse Event to a Pure Enum, Centralize Camera Resize Sync in Engine & Synchronize Docs

Status: resolved
Type: task
Blocked by: 02

**What to build:** Delete all polymorphic keyboard, mouse, and window event structs and `Input::onEvent`, collapse `Event` into `enum class Event { Invalid = -1, WindowResized, WindowClosed }` passed by value, deduplicate `Window` resize callback notifications, centralize active `GameLayer` camera resize synchronization inside `Engine::onEvent`, and synchronize domain and architecture documentation (`GLOSSARY.md`, `AGENTS.md`, `CODING_STANDARDS.md`, and `docs/adr/`).

## Acceptance Criteria

- [x] Derived event headers (`KeyboardEvents`, `MouseEvents`, `WindowEvents`), `getEvent<T>()`, and `Input::onEvent` are deleted and removed from the umbrella header.
- [x] `Event` is defined as `enum class Event { Invalid = -1, WindowResized, WindowClosed };` and passed by value in `Window::EventCallback`, `Engine::onEvent(Event)`, and `GameLayer::onEvent(Event)`.
- [x] `Window` resize callbacks update both logical window dimensions and `Framebuffer Dimensions` and emit a single `Event::WindowResized` notification per resize occurrence rather than firing twice.
- [x] `Engine::onEvent` invokes `gameLayer->syncCameraWithWindow()` alongside `Renderer::setViewport` on `Event::WindowResized`, and `GameLayer::onEvent(Event)` becomes an optional no-op virtual hook with no mandatory base-call constraint.
- [x] `GLOSSARY.md`, `AGENTS.md`, `CODING_STANDARDS.md`, and a superseding ADR in `docs/adr/` are updated to reflect the deepened `Input` module, pure-enum `Event`, and updated initialization order.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments
