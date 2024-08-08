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

	Event(const Type& type)
		: m_type(type)
	{
	}

	virtual ~Event() = default;

	Type getType() const
	{
		return m_type;
	}

private:
	Type m_type;
};


template<typename EventType>
const EventType& getEvent(Event& event)
{
	return static_cast<EventType&>(event);
}

}
