#pragma once

#include <array>

#include "Core/Types.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"

namespace orc {

class Input
{
public:
	Input() = default;
	~Input() = default;

	bool isKeyPressed(Keyboard::Key key) const;
	bool wasKeyJustPressed(Keyboard::Key key) const;
	bool wasKeyJustReleased(Keyboard::Key key) const;

	const Keyboard::SpecialKeys& getSpecialKeys() const;

	bool isMouseButtonPressed(Mouse::Button button) const;
	bool wasMouseButtonJustPressed(Mouse::Button button) const;
	bool wasMouseButtonJustReleased(Mouse::Button button) const;

	const Vector2f& getMousePosition() const;
	Vector2f getMouseDelta() const;
	const Vector2f& getMouseScrollDelta() const;

	void beginFrame();
	void reset();

	void onKeyPressed(Keyboard::Key key, const Keyboard::SpecialKeys& specialKeys);
	void onKeyReleased(Keyboard::Key key, const Keyboard::SpecialKeys& specialKeys);
	void onMouseButtonPressed(Mouse::Button button);
	void onMouseButtonReleased(Mouse::Button button);
	void onMouseMoved(float x, float y);
	void onMouseScrolled(float xDelta, float yDelta);
	void setMousePosition(float x, float y);

private:
	void setKeyState(Keyboard::Key key, bool isPressed);
	void setMouseButtonState(Mouse::Button button, bool isPressed);

	static constexpr size_t MAX_KEYS = 256;
	static constexpr size_t MAX_MOUSE_BUTTONS = 16;

	std::array<bool, MAX_KEYS> m_currentKeys{};
	std::array<bool, MAX_KEYS> m_previousKeys{};

	std::array<bool, MAX_MOUSE_BUTTONS> m_currentMouseButtons{};
	std::array<bool, MAX_MOUSE_BUTTONS> m_previousMouseButtons{};

	Keyboard::SpecialKeys m_specialKeys = {};

	Vector2f m_mousePosition = {0.0f, 0.0f};
	Vector2f m_previousMousePosition = {0.0f, 0.0f};
	Vector2f m_mouseScrollDelta = {0.0f, 0.0f};
};

}
