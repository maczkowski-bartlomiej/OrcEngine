#pragma once

#include "Events/Event.hpp"

namespace orc {

struct WindowResizedEvent : public Event
{
	WindowResizedEvent(uint32_t width, uint32_t height) 
		: Event(Event::Type::WindowResized), width(width), height(height) {}

	uint32_t width, height;
};

struct WindowClosedEvent : public Event
{
	WindowClosedEvent() 
		: Event(Event::Type::WindowClosed) {}
};

}
