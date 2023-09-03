#include "OrcPch.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

Rectangle::Rectangle()
	: m_isFilled(false)
{
}

Rectangle::Rectangle(const Vector2f& position, const Vector2f& size, Color color, bool fillColor)
	: m_isFilled(fillColor)
{
	setSize(size);
	setColor(color);
	setPosition(position);
}

void Rectangle::setSize(const Vector2f& size)
{
	m_size = size;
}

bool Rectangle::isFilled()
{
	return m_isFilled;
}

Vector2f Rectangle::getSize() const
{
	return m_size;
}

void Rectangle::updateVerticesPositions() const
{
	const Matrix& transformMatrix = getTransformMatrix();

	m_vertices[0].position = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector3f(m_size.x, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector3f(0.0f, m_size.y, 1.0f);
	m_vertices[3].position = transformMatrix * Vector3f(m_size.x, m_size.y, 1.0f);
}

FloatRect Rectangle::getLocalRect() const
{
	return FloatRect(0.0f, 0.0f, m_size.x, m_size.y);
}

}
