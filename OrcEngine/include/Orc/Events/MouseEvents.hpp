#pragma once

#include "Input/Mouse.hpp"
#include "Events/Event.hpp"

namespace orc {

struct MouseButtonPressedEvent : public Event
{
	MouseButtonPressedEvent(Mouse::Button button) 
		: Event(Event::Type::MouseButtonPressed), button(button) {}

	const Mouse::Button button;
};

struct MouseButtonReleasedEvent : public Event
{
	MouseButtonReleasedEvent(Mouse::Button button) 
		: Event(Event::Type::MouseButtonReleased), button(button) {}

	const Mouse::Button button;
};

struct MouseMovedEvent : public Event
{
	MouseMovedEvent(float x, float y) 
		: Event(Event::Type::MouseMoved), x(x), y(y) {}

	const float x, y;
};

struct MouseWheelScrolledEvent : public Event
{
	MouseWheelScrolledEvent(float xDelta, float yDelta) 
		: Event(Event::Type::MouseWheelScrolled), xDelta(xDelta), yDelta(yDelta) {}

	const float xDelta;
	const float yDelta;
};

}
