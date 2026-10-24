# Spec: Revert to Polymorphic Event System

Status: ready-for-agent

## Problem Statement

The current event system in OrcEngine utilizes a closed-set `std::variant`-based wrapper (`EventVariant`), visitation metaprogramming (`visit`, `Overload`), query templates (`is<T>`, `getIf<T>`, `as<T>`), and an `EventDispatcher` helper. This model introduces substantial complexity without tangible benefit:

1. **Header Coupling**: The `Event.hpp` header must include all concrete event headers (`WindowEvents.hpp`, `KeyboardEvents.hpp`, `MouseEvents.hpp`) to define the variant type, creating tight coupling and making the event system difficult to extend.
2. **Template & Cognitive Overhead**: Handling events requires writing multi-lambda visitation boilerplate (`event.visit(Overload{ ... })`), template downcasting helpers, or complex dispatchers rather than straightforward C++ control flow.
3. **Dead / Unused State**: The event class carries a `mutable bool m_handled` flag along with `isHandled()` and `setHandled()` methods that no subsystem or game layer in the engine or integration harness ever inspects or acts upon.
4. **Consistency Gaps**: Event struct fields previously suffered from inconsistent const-qualification across event types, and single-argument constructors lacked explicit qualification.

The user desires to revert the event system to the simpler, classic polymorphic inheritance model where `Event` is an open base class with an event type tag, concrete events inherit from it, and downcasting is handled cleanly via a fast helper.

## Solution

Revert the event architecture to a lightweight polymorphic base class hierarchy:

1. **Polymorphic Base Class**: Define `Event` as a base class containing an `Event::Type` tag and a virtual destructor, completely decoupled from concrete event declarations.
2. **Concrete Event Structs**: Have all concrete event structs (`WindowResizedEvent`, `WindowClosedEvent`, `KeyboardKeyPressedEvent`, `KeyboardKeyReleasedEvent`, `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, `MouseMovedEvent`, `MouseWheelScrolledEvent`) inherit publicly from `Event` and initialize the base type tag.
3. **Ergonomic Downcasting Helper**: Provide a lightweight `getEvent<T>(event)` free helper function returning `static_cast<const T&>(event)`, backed by a debug assertion verifying the cast against the polymorphic runtime type.
4. **Eliminate Variant, Metaprogramming & Handled State**: Remove `std::variant`, `std::monostate`, `std::visit`, `Overload`, `EventDispatcher`, `is<T>`, `getIf<T>`, `as<T>`, and the unused `m_handled` state.
5. **Standardize Event Struct Interfaces**: Enforce repository standards across all event structs—explicit single-argument constructors, pass small enums and primitives by value, and plain (non-const) fields for uniform copy/move assignment.
6. **Decoupled Subsystem Event Flow**: Maintain clean subsystem boundaries where `Engine::onEvent` routes to `Input::onEvent` before forwarding to the active `GameLayer`, `GameLayer::onEvent` handles viewport synchronization on window resize, and `Input` updates its internal state snapshot using clean switch statements.

## User Stories

1. As a game developer, I want `Event` to be a lightweight base class with an `Event::Type` enum, so that I can inspect event types with simple `switch` or `if` statements without template visitation boilerplate.
2. As a game developer, I want to use `getEvent<T>(event)` to downcast an event reference, so that accessing event-specific data is concise and readable.
3. As a game developer, I want debug builds to assert that `getEvent<T>(event)` matches the underlying runtime event type, so that bad downcasts are caught immediately during development.
4. As a game developer, I want `GameLayer::onEvent` to handle window resize events and update `m_camera` viewport dimensions directly, so that layer cameras stay synchronized without manual engine polling.
5. As a game developer, I want interactive demo layers to process keyboard and mouse wheel events using standard C++ control flow, so that demo layer code remains simple and approachable.
6. As an engine maintainer, I want concrete event types to include `Event.hpp` rather than `Event.hpp` including all concrete event types, so that there are no circular dependencies and adding new event types does not require modifying the base header.
7. As an engine maintainer, I want to eliminate `std::variant`, `std::monostate`, `Overload`, and `EventDispatcher` from `Event.hpp`, so that compile times and header bloat are minimized.
8. As an engine maintainer, I want unused mutable handled state (`m_handled`, `setHandled()`, `isHandled()`) removed, so that events remain pure, stateless data carriers.
9. As an engine maintainer, I want all single-argument event constructors marked `explicit`, so that implicit conversions between primitives and event structs are prohibited.
10. As an engine maintainer, I want all event struct data members to be non-const plain values, so that all event types have consistent copy and move assignment semantics.
11. As an engine maintainer, I want `Input::onEvent` to process discrete keyboard and mouse events using an explicit `switch` statement on `event.getType()`, so that input state tracking is decoupled, deterministic, and easily auditable.
12. As an engine maintainer, I want `Engine::onEvent` to dispatch events to `Input` and the active `GameLayer` before handling engine-level lifecycle events (`WindowClosedEvent`), so that layers receive events before engine shutdown.
13. As an engine maintainer, I want an Architectural Decision Record (ADR) recorded in `docs/adr/`, so that future contributors understand the rationale for adopting a polymorphic hierarchy over `std::variant`.
14. As an engine maintainer, I want `AGENTS.md` and `GLOSSARY.md` updated, so that codebase documentation remains 100% synchronized with the engine architecture.

## Implementation Decisions

- **Base `Event` Interface**:
  - The base `Event` class declares an `enum class Type` enumerating all supported engine event types (`WindowResized`, `WindowClosed`, `KeyboardKeyPressed`, `KeyboardKeyReleased`, `MouseButtonPressed`, `MouseButtonReleased`, `MouseMoved`, `MouseWheelScrolled`, `Invalid`).
  - Base constructor is `explicit Event(Type type)` taking the enum by value and initializing a private `Type m_type` member.
  - Base destructor is virtual (`virtual ~Event() = default;`) to ensure polymorphic cleanup and enable RTTI downcast checking in debug builds.
  - Base exposes `Type getType() const noexcept` returning `m_type`.
  - The unused `mutable bool m_handled`, `setHandled()`, and `isHandled()` members are deleted.

- **Downcasting Helper**:
  - `template<typename EventType> const EventType& getEvent(const Event& event)` is defined as a free function.
  - In debug builds, `getEvent` checks the validity of the cast via an assertion (`dynamic_cast<const EventType*>(&event) != nullptr`).
  - In all builds, it returns `static_cast<const EventType&>(event)`.
  - The helper templates `is<T>()`, `getIf<T>()`, and `as<T>()` are removed from the base class.

- **Concrete Event Struct Contracts**:
  - Concrete event structs inherit publicly from `Event`:
    - `WindowResizedEvent`: constructor takes `uint32_t width, uint32_t height`, initializes `Event(Event::Type::WindowResized)`, stores `uint32_t width, height`.
    - `WindowClosedEvent`: default constructor initializes `Event(Event::Type::WindowClosed)`.
    - `KeyboardKeyPressedEvent`: constructor takes `Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()`, marked `explicit`, initializes `Event(Event::Type::KeyboardKeyPressed)`, stores `Keyboard::Key key`, `Keyboard::SpecialKeys specialKeys`.
    - `KeyboardKeyReleasedEvent`: constructor takes `Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()`, marked `explicit`, initializes `Event(Event::Type::KeyboardKeyReleased)`, stores `Keyboard::Key key`, `Keyboard::SpecialKeys specialKeys`.
    - `MouseButtonPressedEvent`: constructor takes `Mouse::Button button`, marked `explicit`, initializes `Event(Event::Type::MouseButtonPressed)`, stores `Mouse::Button button`.
    - `MouseButtonReleasedEvent`: constructor takes `Mouse::Button button`, marked `explicit`, initializes `Event(Event::Type::MouseButtonReleased)`, stores `Mouse::Button button`.
    - `MouseMovedEvent`: constructor takes `float x, float y`, initializes `Event(Event::Type::MouseMoved)`, stores `float x, y`.
    - `MouseWheelScrolledEvent`: constructor takes `float xDelta, float yDelta`, initializes `Event(Event::Type::MouseWheelScrolled)`, stores `float xDelta, yDelta`.
  - All data members are non-const, mutable public fields conforming to standard aggregate-like event structs.

- **Subsystem & Integration Updates**:
  - `Engine::onEvent`: Calls `m_input->onEvent(event)`, invokes `gameLayer->onEvent(event)` if an active layer exists, and sets `m_running = false` when `event.getType() == Event::Type::WindowClosed`.
  - `GameLayer::onEvent`: When `event.getType() == Event::Type::WindowResized`, extracts `const auto& resized = getEvent<WindowResizedEvent>(event)` and adjusts `m_camera` viewport dimensions.
  - `Input::onEvent`: Replaces `event.visit` with a `switch (event.getType())` matching keyboard and mouse event types, downcasting via `getEvent<T>(event)` to update key and button bitsets and cursor position.
  - `TestProject` demo layers: Replace all usages of `event.getIf<T>()` and `event.visit` with `event.getType() == ...` and `getEvent<T>(event)`.
  - `Window::setCallbacks`: Passes concrete event structs by reference directly into `videoSettings.eventCallback(...)`, matching `std::function<void(const Event&)>`.

- **Documentation & Architectural Decision Synchronization**:
  - Record ADR `docs/adr/0005-revert-to-polymorphic-event-hierarchy.md` documenting the decision and trade-offs.
  - Update `AGENTS.md` (Section 4.3) to specify the polymorphic `Event` base class, `getType()`, `getEvent<T>()`, and the dispatch order.
  - Add `Event` to `GLOSSARY.md`.

## Testing Decisions

- **Single Primary Seam (`Engine & Layer Event Ingestion Seam`)**:
  - The highest and sole necessary integration seam is the entry point `Engine::onEvent(const Event&)` and `GameLayer::onEvent(const Event&)`.
  - Only observable behavior is tested:
    - Dispatching window close stops the engine run loop.
    - Dispatching window resize adjusts camera viewport bounds.
    - Dispatching keyboard and mouse events updates the `Input` snapshot (observable via `m_layerContext.input.isKeyPressed` and `getMousePosition`).
    - Interactive layer switching in demo layers responds to `KeyboardKeyPressedEvent`.
- **Smoke Testing & Automated Gate**:
  - Verification runs through `./scripts/verify.sh` (which executes convention linting, compiles the engine and demo suite with zero warnings/errors, and runs the 5-second `TestProject` smoke test).
  - Conventions linter (`./scripts/lint-conventions.sh`) validates tabs, header guards, formatting, and explicit constructors.

## Out of Scope

- Custom event queueing or deferred asynchronous event dispatching.
- Event filtering, bubbling, or event cancellation beyond immediate return.
- Multi-window event routing.
- Gamepad or joystick events.

## Further Notes

- Replaces and supersedes the temporary `std::variant`-based event system introduced during earlier cleanup.
- Does not affect `LayerContext` or `GameLayer` constructor injection established in ADR 0004.
