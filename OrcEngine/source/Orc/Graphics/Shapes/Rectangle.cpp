#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

Rectangle::Rectangle()
{
	for (auto& vertex : m_vertices)
		vertex.shapeType = 1.0f;
}

Rectangle::Rectangle(const Vector2f& size, const Color& color)
	: Rectangle()
{
	setSize(size);
	setFillColor(color);
}

Rectangle::Rectangle(const Vector2f& position, const Vector2f& size, const Color& color)
	: Rectangle()
{
	setSize(size);
	setPosition(position);
	setFillColor(color);
}

void Rectangle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;
	Vector4f normColor = m_fillColor.normalized();
	for (auto& vertex : m_vertices)
		vertex.color = normColor;
}

void Rectangle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;
	Vector4f normColor = m_borderColor.normalized();
	for (auto& vertex : m_vertices)
		vertex.secondaryColor = normColor;
}

void Rectangle::setSize(const Vector2f& size)
{
	m_size = size;
}

void Rectangle::setBorderThickness(float borderThickness)
{
	m_borderThickness = borderThickness;

	for (auto& vertex : m_vertices)
		vertex.borderThickness = m_borderThickness;
}

Color Rectangle::getFillColor() const
{
	return m_fillColor;
}

Color Rectangle::getBorderColor() const
{
	return m_borderColor;
}

Vector2f Rectangle::getSize() const
{
	return m_size;
}

float Rectangle::getBorderThickness() const
{
	return m_borderThickness;
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
