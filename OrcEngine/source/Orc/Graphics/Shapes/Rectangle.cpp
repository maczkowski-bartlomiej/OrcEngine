#include "OrcPch.hpp"

#include "Engine/Debug.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

Rectangle::Rectangle(const Vector2f& size)
{
	setSize(size);
}

Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
{
	setSize(size);
	setPosition(position);
}

void Rectangle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;
	for (auto& vertex : m_vertices)
		vertex.fillColor = m_fillColor.normalized();
}

void Rectangle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;

	for (auto& vertex : m_vertices)
		vertex.borderColor = m_borderColor.normalized();
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
