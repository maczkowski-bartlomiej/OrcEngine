#pragma once

#include "Events/Event.hpp"

namespace orc {

struct WindowResizedEvent : public Event
{
	WindowResizedEvent(uint32 width, uint32 height) 
		: Event(Event::Type::WindowResized), width(width), height(height) {}

	uint32 width, height;
};

struct WindowClosedEvent : public Event
{
	WindowClosedEvent() 
		: Event(Event::Type::WindowClosed) {}
};

}
