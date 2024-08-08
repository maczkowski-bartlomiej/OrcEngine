#pragma once

#include "Engine/Core.hpp"

namespace orc {

struct Color
{
	constexpr Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255)
		: r(r), g(g), b(b), a(a) {}

	constexpr Vector4f normalized() const
	{
		return Vector4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	constexpr bool operator==(const Color& other) const
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	uint8_t r, g, b, a;
};

}
