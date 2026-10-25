#pragma once

#include "Core/Types.hpp"

namespace orc {

struct Color
{
	Color()
		: r(255), g(255), b(255), a(255)
	{
	}

	explicit Color(uint8_t r)
		: r(r), g(255), b(255), a(255)
	{
	}

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: r(r), g(g), b(b), a(a)
	{
	}

	Vector4f normalized() const
	{
		return Vector4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	bool operator==(const Color& other) const
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;
	uint8_t a = 255;
};

}
