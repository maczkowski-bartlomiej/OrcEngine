#pragma once

#include "Engine/Debug.hpp"

namespace orc {

class Event
{
public:
	enum class Type
	{
		Invalid = -1,
		WindowResized,
		WindowClosed,
		KeyboardKeyPressed,
		KeyboardKeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseWheelScrolled
	};

	explicit Event(Type type)
		: m_type(type)
	{
	}

	virtual ~Event() = default;

	Type getType() const noexcept
	{
		return m_type;
	}

private:
	Type m_type;
};

template <typename EventType>
const EventType& getEvent(const Event& event)
{
	ORC_ASSERT(dynamic_cast<const EventType*>(&event) != nullptr, "Invalid event downcast!");
	return static_cast<const EventType&>(event);
}

}
