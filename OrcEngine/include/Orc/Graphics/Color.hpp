#pragma once

#include "Engine/Core.hpp"

namespace orc {

struct Color
{
	constexpr Color(uint8 r = 255, uint8 g = 255, uint8 b = 255, uint8 a = 255)
		: r(r), g(g), b(b), a(a) {}

	constexpr Vector4f normalized() const
	{
		return Vector4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	uint8 r, g, b, a;
};

}
