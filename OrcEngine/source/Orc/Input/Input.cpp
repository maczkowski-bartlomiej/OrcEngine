#include "OrcPch.hpp"

#include "Input/Input.hpp"

namespace orc {

bool Input::isKeyPressed(Keyboard::Key key) const
{
	const int index = static_cast<int>(key);
	if (index < 0 || static_cast<size_t>(index) >= MAX_KEYS)
	{
		return false;
	}

	return m_keys[static_cast<size_t>(index)];
}

bool Input::isMouseButtonPressed(Mouse::Button button) const
{
	const int index = static_cast<int>(button);
	if (index < 0 || static_cast<size_t>(index) >= MAX_MOUSE_BUTTONS)
	{
		return false;
	}

	return m_mouseButtons[static_cast<size_t>(index)];
}

Vector2f Input::getMousePosition() const
{
	return m_mousePosition;
}

void Input::onEvent(const Event& event)
{
	event.visit(
		[this](const KeyboardKeyPressedEvent& e)
		{
			setKeyState(e.key, true);
		},
		[this](const KeyboardKeyReleasedEvent& e)
		{
			setKeyState(e.key, false);
		},
		[this](const MouseButtonPressedEvent& e)
		{
			setMouseButtonState(e.button, true);
		},
		[this](const MouseButtonReleasedEvent& e)
		{
			setMouseButtonState(e.button, false);
		},
		[this](const MouseMovedEvent& e)
		{
			m_mousePosition = Vector2f(e.x, e.y);
		},
		[](auto&&) {}
	);
}

void Input::reset()
{
	m_keys.fill(false);
	m_mouseButtons.fill(false);
}

void Input::setKeyState(Keyboard::Key key, bool isPressed)
{
	const int index = static_cast<int>(key);
	if (index >= 0 && static_cast<size_t>(index) < MAX_KEYS)
	{
		m_keys[static_cast<size_t>(index)] = isPressed;
	}
}

void Input::setMouseButtonState(Mouse::Button button, bool isPressed)
{
	const int index = static_cast<int>(button);
	if (index >= 0 && static_cast<size_t>(index) < MAX_MOUSE_BUTTONS)
	{
		m_mouseButtons[static_cast<size_t>(index)] = isPressed;
	}
}

}
