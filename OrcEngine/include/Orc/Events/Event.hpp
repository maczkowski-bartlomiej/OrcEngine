#pragma once

#include <functional>
#include <ostream>

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

	virtual ~Event() = default;

	void setHandled(bool handled = true) const
	{
		m_handled = handled;
	}

	Type getType() const
	{
		return m_type;
	}

	bool isHandled() const
	{
		return m_handled;
	}

	virtual Type getEventType() const = 0;

protected:
	Event(const Type& type = Type::Invalid)
		: m_type(type)
	{
	}

private:
	Type m_type;
	mutable bool m_handled = false;

	friend std::ostream& operator<<(std::ostream& os, const Event& event);
};

template<Event::Type EventTypeValue>
class EventT : public Event
{
public:
	EventT() : Event(EventTypeValue) {}

	static Event::Type getStaticType() { return EventTypeValue; }
	Event::Type getEventType() const override { return EventTypeValue; }
};

template<typename DerivedEvent>
const DerivedEvent& getEvent(const Event& event)
{
	ORC_ASSERT(event.getEventType() == DerivedEvent::getStaticType(), "Invalid event type cast: attempted to cast event of type {} to type {}.", static_cast<int>(event.getEventType()), static_cast<int>(DerivedEvent::getStaticType()));
	return static_cast<const DerivedEvent&>(event);
}

inline std::ostream& operator<<(std::ostream& os, const Event& event)
{
	os << "Event(type=" << static_cast<int>(event.getEventType()) << ").";
	return os;
}

}
