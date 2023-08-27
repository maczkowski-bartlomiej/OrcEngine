#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Transformable.hpp"

namespace orc {

class Rectangle : public Transformable
{
public:
	Rectangle() = default;
	Rectangle(const Vector2f& position, const Vector2f& size, Vector4f color, bool fillColor = true)
		: m_color(color), m_isFilled(fillColor)
	{
		setPosition(position);
		setSize(size);
	}

	bool isFilled()
	{
		return m_isFilled;
	}

private:
	Vector4f m_color;
	bool m_isFilled;

	std::array<TexturedVertex, 4u> m_vertices;
};

}

/*

Polygon - with 

*/
