# Revert Event Base and Concrete Event Structs

Status: resolved
Type: task
Blocked by: none

## Description

Revert the base `Event` class and concrete event structs in `OrcEngine/include/Orc/Events/` to the classic polymorphic inheritance hierarchy:

1. **`Event.hpp`**:
   - Revert `Event` to a polymorphic base class with `enum class Type`, `explicit Event(Type type)`, `virtual ~Event() = default;`, and `Type getType() const noexcept`.
   - Provide standalone `template<typename EventType> const EventType& getEvent(const Event& event)` performing `static_cast<const EventType&>(event)` with an `ORC_ASSERT(dynamic_cast<const EventType*>(&event) != nullptr, ...)` safety check.
   - Delete `std::variant`, `std::monostate`, `visit()`, `Overload`, `is<T>()`, `getIf<T>()`, `as<T>()`, `EventDispatcher`, `raw()`, and unused `m_handled` state (`isHandled()`, `setHandled()`).
   - Remove concrete event includes from `Event.hpp`.

2. **`WindowEvents.hpp`**:
   - Include `Events/Event.hpp`.
   - `WindowResizedEvent : public Event`: `width`, `height` non-const fields, constructor initializes `Event(Event::Type::WindowResized)`.
   - `WindowClosedEvent : public Event`: constructor initializes `Event(Event::Type::WindowClosed)`.

3. **`KeyboardEvents.hpp`**:
   - Include `Events/Event.hpp` and `Input/Keyboard.hpp`.
   - `KeyboardKeyPressedEvent : public Event`: `key`, `specialKeys` non-const fields, `explicit` constructor with default `specialKeys`, initializes `Event(Event::Type::KeyboardKeyPressed)`.
   - `KeyboardKeyReleasedEvent : public Event`: `key`, `specialKeys` non-const fields, `explicit` constructor with default `specialKeys`, initializes `Event(Event::Type::KeyboardKeyReleased)`.

4. **`MouseEvents.hpp`**:
   - Include `Events/Event.hpp` and `Input/Mouse.hpp`.
   - `MouseButtonPressedEvent : public Event`: `button` non-const field, `explicit` constructor, initializes `Event(Event::Type::MouseButtonPressed)`.
   - `MouseButtonReleasedEvent : public Event`: `button` non-const field, `explicit` constructor, initializes `Event(Event::Type::MouseButtonReleased)`.
   - `MouseMovedEvent : public Event`: `x`, `y` non-const fields, initializes `Event(Event::Type::MouseMoved)`.
   - `MouseWheelScrolledEvent : public Event`: `xDelta`, `yDelta` non-const fields, initializes `Event(Event::Type::MouseWheelScrolled)`.

## Completion Criteria
- Headers compile cleanly and are decoupled.
- All single-arg constructors are explicit.

## Answer

Reverted Event base class to polymorphic hierarchy with getEvent helper and updated concrete event structs.
