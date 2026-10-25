#include "Input/Input.hpp"

#include "Events/KeyboardEvents.hpp"
#include "Events/MouseEvents.hpp"

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
	switch (event.getType())
	{
		case Event::Type::KeyboardKeyPressed:
		{
			const auto& e = getEvent<KeyboardKeyPressedEvent>(event);
			setKeyState(e.key, true);
			break;
		}
		case Event::Type::KeyboardKeyReleased:
		{
			const auto& e = getEvent<KeyboardKeyReleasedEvent>(event);
			setKeyState(e.key, false);
			break;
		}
		case Event::Type::MouseButtonPressed:
		{
			const auto& e = getEvent<MouseButtonPressedEvent>(event);
			setMouseButtonState(e.button, true);
			break;
		}
		case Event::Type::MouseButtonReleased:
		{
			const auto& e = getEvent<MouseButtonReleasedEvent>(event);
			setMouseButtonState(e.button, false);
			break;
		}
		case Event::Type::MouseMoved:
		{
			const auto& e = getEvent<MouseMovedEvent>(event);
			m_mousePosition = Vector2f(e.x, e.y);
			break;
		}
		default:
			break;
	}
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
