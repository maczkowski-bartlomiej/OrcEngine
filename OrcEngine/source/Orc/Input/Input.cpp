#include "Input/Input.hpp"

namespace orc {

bool Input::isKeyPressed(Keyboard::Key key) const
{
	const int index = static_cast<int>(key);
	if (index < 0 || static_cast<size_t>(index) >= MAX_KEYS)
	{
		return false;
	}

	return m_currentKeys[static_cast<size_t>(index)];
}

bool Input::wasKeyJustPressed(Keyboard::Key key) const
{
	const int index = static_cast<int>(key);
	if (index < 0 || static_cast<size_t>(index) >= MAX_KEYS)
	{
		return false;
	}

	const size_t i = static_cast<size_t>(index);
	return m_currentKeys[i] && !m_previousKeys[i];
}

bool Input::wasKeyJustReleased(Keyboard::Key key) const
{
	const int index = static_cast<int>(key);
	if (index < 0 || static_cast<size_t>(index) >= MAX_KEYS)
	{
		return false;
	}

	const size_t i = static_cast<size_t>(index);
	return !m_currentKeys[i] && m_previousKeys[i];
}

const Keyboard::SpecialKeys& Input::getSpecialKeys() const
{
	return m_specialKeys;
}

bool Input::isMouseButtonPressed(Mouse::Button button) const
{
	const int index = static_cast<int>(button);
	if (index < 0 || static_cast<size_t>(index) >= MAX_MOUSE_BUTTONS)
	{
		return false;
	}

	return m_currentMouseButtons[static_cast<size_t>(index)];
}

bool Input::wasMouseButtonJustPressed(Mouse::Button button) const
{
	const int index = static_cast<int>(button);
	if (index < 0 || static_cast<size_t>(index) >= MAX_MOUSE_BUTTONS)
	{
		return false;
	}

	const size_t i = static_cast<size_t>(index);
	return m_currentMouseButtons[i] && !m_previousMouseButtons[i];
}

bool Input::wasMouseButtonJustReleased(Mouse::Button button) const
{
	const int index = static_cast<int>(button);
	if (index < 0 || static_cast<size_t>(index) >= MAX_MOUSE_BUTTONS)
	{
		return false;
	}

	const size_t i = static_cast<size_t>(index);
	return !m_currentMouseButtons[i] && m_previousMouseButtons[i];
}

const Vector2f& Input::getMousePosition() const
{
	return m_mousePosition;
}

Vector2f Input::getMouseDelta() const
{
	return m_mousePosition - m_previousMousePosition;
}

const Vector2f& Input::getMouseScrollDelta() const
{
	return m_mouseScrollDelta;
}

void Input::beginFrame()
{
	m_previousKeys = m_currentKeys;
	m_previousMouseButtons = m_currentMouseButtons;
	m_previousMousePosition = m_mousePosition;
	m_mouseScrollDelta = {0.0f, 0.0f};
}

void Input::reset()
{
	m_currentKeys.fill(false);
	m_previousKeys.fill(false);
	m_currentMouseButtons.fill(false);
	m_previousMouseButtons.fill(false);
	m_specialKeys = {};
	m_mouseScrollDelta = {0.0f, 0.0f};
}

void Input::onKeyPressed(Keyboard::Key key, const Keyboard::SpecialKeys& specialKeys)
{
	setKeyState(key, true);
	m_specialKeys = specialKeys;
}

void Input::onKeyReleased(Keyboard::Key key, const Keyboard::SpecialKeys& specialKeys)
{
	setKeyState(key, false);
	m_specialKeys = specialKeys;
}

void Input::onMouseButtonPressed(Mouse::Button button)
{
	setMouseButtonState(button, true);
}

void Input::onMouseButtonReleased(Mouse::Button button)
{
	setMouseButtonState(button, false);
}

void Input::onMouseMoved(float x, float y)
{
	m_mousePosition = {x, y};
}

void Input::onMouseScrolled(float xDelta, float yDelta)
{
	m_mouseScrollDelta.x += xDelta;
	m_mouseScrollDelta.y += yDelta;
}

void Input::setMousePosition(float x, float y)
{
	m_mousePosition = {x, y};
	m_previousMousePosition = m_mousePosition;
}

void Input::setKeyState(Keyboard::Key key, bool isPressed)
{
	const int index = static_cast<int>(key);
	if (index >= 0 && static_cast<size_t>(index) < MAX_KEYS)
	{
		m_currentKeys[static_cast<size_t>(index)] = isPressed;
	}
}

void Input::setMouseButtonState(Mouse::Button button, bool isPressed)
{
	const int index = static_cast<int>(button);
	if (index >= 0 && static_cast<size_t>(index) < MAX_MOUSE_BUTTONS)
	{
		m_currentMouseButtons[static_cast<size_t>(index)] = isPressed;
	}
}

}
