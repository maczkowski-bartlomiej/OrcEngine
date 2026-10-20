#pragma once

#include "Events/Event.hpp"

#include <cstdint>

namespace orc {

struct WindowResizedEvent : public Event
{
	WindowResizedEvent(uint32_t width, uint32_t height)
		: Event(Event::Type::WindowResized), width(width), height(height)
	{
	}

	uint32_t width = 0;
	uint32_t height = 0;
};

struct WindowClosedEvent : public Event
{
	WindowClosedEvent()
		: Event(Event::Type::WindowClosed)
	{
	}
};

}
