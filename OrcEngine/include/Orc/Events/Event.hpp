#pragma once

namespace orc {

class Event
{
public:
	enum class Type
	{
		Invalid = -1,
		WindowResized, WindowClosed,
		KeyboardKeyPressed, KeyboardKeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseWheelScrolled
	};

	Event(const Type& type);
	virtual ~Event() = default;

	void markHandled();
	bool isHandled() const;

	Type getType() const;

private:
	Type m_type;
	bool m_handled;
};

}
