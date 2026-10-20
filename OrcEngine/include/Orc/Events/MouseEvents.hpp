#pragma once

#include "Events/Event.hpp"
#include "Input/Mouse.hpp"

namespace orc {

struct MouseButtonPressedEvent : public Event
{
	explicit MouseButtonPressedEvent(Mouse::Button button)
		: Event(Event::Type::MouseButtonPressed), button(button)
	{
	}

	Mouse::Button button = Mouse::Button::Invalid;
};

struct MouseButtonReleasedEvent : public Event
{
	explicit MouseButtonReleasedEvent(Mouse::Button button)
		: Event(Event::Type::MouseButtonReleased), button(button)
	{
	}

	Mouse::Button button = Mouse::Button::Invalid;
};

struct MouseMovedEvent : public Event
{
	MouseMovedEvent(float x, float y)
		: Event(Event::Type::MouseMoved), x(x), y(y)
	{
	}

	float x = 0.0f;
	float y = 0.0f;
};

struct MouseWheelScrolledEvent : public Event
{
	MouseWheelScrolledEvent(float xDelta, float yDelta)
		: Event(Event::Type::MouseWheelScrolled), xDelta(xDelta), yDelta(yDelta)
	{
	}

	float xDelta = 0.0f;
	float yDelta = 0.0f;
};

}
