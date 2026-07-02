#pragma once

#include "Input/Mouse.hpp"
#include "Events/Event.hpp"

namespace orc {

struct MouseButtonPressedEvent final : public EventT<Event::Type::MouseButtonPressed>
{
	MouseButtonPressedEvent(Mouse::Button button) 
		: button(button) {}

	const Mouse::Button button;
};

struct MouseButtonReleasedEvent final : public EventT<Event::Type::MouseButtonReleased>
{
	MouseButtonReleasedEvent(Mouse::Button button) 
		: button(button) {}

	const Mouse::Button button;
};

struct MouseMovedEvent final : public EventT<Event::Type::MouseMoved>
{
	MouseMovedEvent(float x, float y) 
		: x(x), y(y) {}

	const float x, y;
};

struct MouseWheelScrolledEvent final : public EventT<Event::Type::MouseWheelScrolled>
{
	MouseWheelScrolledEvent(float xDelta, float yDelta) 
		: xDelta(xDelta), yDelta(yDelta) {}

	const float xDelta;
	const float yDelta;
};

}
