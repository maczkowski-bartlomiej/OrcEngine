#pragma once

#include "Events/Event.hpp"

#include <cstdint>

namespace orc {

struct WindowResizedEvent final : public EventT<Event::Type::WindowResized>
{
	WindowResizedEvent(uint32_t width, uint32_t height) 
		: width(width), height(height) {}

	uint32_t width, height;
};

struct WindowClosedEvent final : public EventT<Event::Type::WindowClosed>
{
	WindowClosedEvent() {}
};

}
