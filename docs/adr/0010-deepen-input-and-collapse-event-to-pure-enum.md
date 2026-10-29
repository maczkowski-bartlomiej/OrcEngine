# Deepen Input Subsystem and Collapse Event to Pure Enum

> [!NOTE]
> Supersedes [ADR 0005](0005-revert-to-polymorphic-event-hierarchy.md).

OrcEngine previously dispatched keyboard, mouse, and window events through a polymorphic `Event` hierarchy, requiring `getEvent<T>` runtime downcasting whenever a layer needed discrete key transitions or scroll wheel events. Additionally, `Window`'s GLFW callbacks created transient polymorphic event wrappers purely to pass through to `Input::onEvent`, and window resize fired duplicate events while imposing a fragile requirement for derived `GameLayer` classes to call `GameLayer::onEvent(event)` to synchronize camera viewports.

We decided to deepen the `Input` module into an authoritative frame-snapshot subsystem tracking current- and previous-frame key and mouse button arrays (`MAX_KEYS`, `MAX_MOUSE_BUTTONS`), `Keyboard::SpecialKeys` modifier state, cursor position and per-frame delta, and mouse wheel scroll delta across deterministic frame boundaries (`Input::beginFrame()`). `Window` constructs with an `Input&` reference and updates `Input` directly from its GLFW callbacks, while GLFW translation helpers (`glfwKeyToOrcKey`, `glfwKeyModsToOrcSpecialKeys`, `glfwButtonToOrcButton`) are sealed inside an internal engine header (`GlfwInputMapping.hpp`) decoupled from public engine headers.

Because `Input` absorbs all input queries and callers query `Window` directly for physical and logical dimensions, `Event` is collapsed into a pure value `enum class Event { Invalid = -1, WindowResized, WindowClosed };` passed by value. Window resize callbacks are deduplicated to emit a single `Event::WindowResized`, and `Engine::onEvent` coordinates active layer camera viewport synchronization alongside `Renderer::setViewport`, making `GameLayer::onEvent(Event)` an optional no-op hook. All `GameLayer` implementations query `LayerContext::input` inside `onUpdate` with zero dynamic casting or event struct overhead.
