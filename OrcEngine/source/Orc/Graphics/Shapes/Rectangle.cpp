#include "OrcPch.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

Rectangle::Rectangle()
{
	for (auto& vertex : m_vertices)
	{
		vertex.fillColor = m_fillColor.normalized();
		vertex.borderColor = m_borderColor.normalized();
		vertex.borderThickness = m_borderThickness;
	}
}

Rectangle::Rectangle(const Vector2f& size)
	: Rectangle()
{
	setSize(size);
}

Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
	: Rectangle()
{
	setSize(size);
	setPosition(position);
}

void Rectangle::setSize(const Vector2f& size)
{
	m_size = size;
}

void Rectangle::setBorderThickness(float borderThickness)
{
	m_borderThickness = borderThickness;

	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].borderThickness = m_borderThickness;
	}
}

void Rectangle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;
	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].fillColor = fillColor.normalized();
	}
}

void Rectangle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;
	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].borderColor = m_borderColor.normalized();
	}
}

Vector2f Rectangle::getSize() const
{
	return m_size;
}

float Rectangle::getBorderThickness() const
{
	return m_borderThickness;
}

Color Rectangle::getFillColor() const
{
	return m_fillColor;
}

Color Rectangle::getBorderColor() const
{
	return m_borderColor;
}

FloatRect Rectangle::getLocalRect() const
{
	return FloatRect(0.0f, 0.0f, m_size.x, m_size.y);
}

void Rectangle::updateVerticesPositions() const
{
	const Matrix& transformMatrix = getTransformMatrix();

	m_vertices[0].position = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector3f(m_size.x, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector3f(0.0f, m_size.y, 1.0f);
	m_vertices[3].position = transformMatrix * Vector3f(m_size.x, m_size.y, 1.0f);
}

}
