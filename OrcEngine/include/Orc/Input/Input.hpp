#pragma once

#include <array>

#include "Core/Types.hpp"
#include "Events/Event.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"

namespace orc {

class Input
{
public:
	Input() = default;
	~Input() = default;

	bool isKeyPressed(Keyboard::Key key) const;
	bool isMouseButtonPressed(Mouse::Button button) const;
	Vector2f getMousePosition() const;

	void onEvent(const Event& event);
	void reset();

private:
	void setKeyState(Keyboard::Key key, bool isPressed);
	void setMouseButtonState(Mouse::Button button, bool isPressed);

	static constexpr size_t MAX_KEYS = 256;
	static constexpr size_t MAX_MOUSE_BUTTONS = 16;

	std::array<bool, MAX_KEYS> m_keys{};
	std::array<bool, MAX_MOUSE_BUTTONS> m_mouseButtons{};
	Vector2f m_mousePosition{0.0f, 0.0f};
};

}
