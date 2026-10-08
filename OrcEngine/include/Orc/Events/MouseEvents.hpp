#pragma once

#include "Input/Mouse.hpp"

namespace orc {

struct MouseButtonPressedEvent
{
	Mouse::Button button = Mouse::Button::Invalid;

	MouseButtonPressedEvent() = default;
	MouseButtonPressedEvent(Mouse::Button button) 
		: button(button) {}
};

struct MouseButtonReleasedEvent
{
	Mouse::Button button = Mouse::Button::Invalid;

	MouseButtonReleasedEvent() = default;
	MouseButtonReleasedEvent(Mouse::Button button) 
		: button(button) {}
};

struct MouseMovedEvent
{
	float x = 0.0f;
	float y = 0.0f;

	MouseMovedEvent() = default;
	MouseMovedEvent(float x, float y) 
		: x(x), y(y) {}
};

struct MouseWheelScrolledEvent
{
	float xDelta = 0.0f;
	float yDelta = 0.0f;

	MouseWheelScrolledEvent() = default;
	MouseWheelScrolledEvent(float xDelta, float yDelta) 
		: xDelta(xDelta), yDelta(yDelta) {}
};

}

