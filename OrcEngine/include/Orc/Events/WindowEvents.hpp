#pragma once

#include <cstdint>

namespace orc {

struct WindowResizedEvent
{
	uint32_t width = 0;
	uint32_t height = 0;

	WindowResizedEvent() = default;
	WindowResizedEvent(uint32_t width, uint32_t height)
		: width(width), height(height) {}
};

struct WindowClosedEvent
{
};

}

