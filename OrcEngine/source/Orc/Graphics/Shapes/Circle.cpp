#include "OrcPch.hpp"

#include "Graphics/Shapes/Circle.hpp"

namespace orc {

Circle::Circle()
	: m_radius(1.0f), m_thickness(1.0f), m_borderThickness(0.05f)
{
	m_vertices[0].localPosition = Vector3f( -0.5f,  0.5f, 1.0f) * 2.0f;
	m_vertices[1].localPosition = Vector3f(  0.5f,  0.5f, 1.0f) * 2.0f;
	m_vertices[2].localPosition = Vector3f( -0.5f, -0.5f, 1.0f) * 2.0f;
	m_vertices[3].localPosition = Vector3f(  0.5f, -0.5f, 1.0f) * 2.0f;
}

Circle::Circle(float radius)
	: Circle()
{
	setRadius(radius);
}

Circle::Circle(const Vector2f& position, float radius)
	: Circle()
{
	setPosition(position);
	setRadius(radius);
}

void Circle::setRadius(float radius)
{
	m_radius = radius;

	for (uint64 i = 0; i < 4; i++)
	{
		m_vertices[i].radius = m_radius;
	}
}

void Circle::setThickness(float thickness)
{
	m_thickness = thickness;

	for (uint64 i = 0; i < 4; i++)
	{
		m_vertices[i].thickness = m_thickness;
	}
}

void Circle::setBorderThickness(float borderThickness)
{
	m_borderThickness = borderThickness;

	for (uint64 i = 0; i < 4; i++)
	{
		m_vertices[i].borderThickness = m_borderThickness;
	}
}

void Circle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;
	for (uint64 i = 0; i < 4; i++)
	{
		m_vertices[i].fillColor = fillColor.normalized();
	}
}

void Circle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;
	for (uint64 i = 0; i < 4; i++)
	{
		m_vertices[i].borderColor = m_borderColor.normalized();
	}
}

float Circle::getRadius() const
{
	return m_radius;
}

float Circle::getThickness() const
{
	return m_thickness;
}

float Circle::getBorderThickness() const
{
	return m_borderThickness;
}

Color Circle::getFillColor() const
{
	return m_fillColor;
}

Color Circle::getBorderColor() const
{
	return m_borderColor;
}

FloatRect Circle::getLocalRect() const
{
	return FloatRect(0.0f, 0.0f, m_radius * 2.0f, m_radius * 2.0f);
}

void Circle::updateVerticesPositions() const
{
	const Matrix& transformMatrix = getTransformMatrix();
	m_vertices[0].globalPosition = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].globalPosition = transformMatrix * Vector3f(m_radius * 2.0f, 0.0f, 1.0f);
	m_vertices[2].globalPosition = transformMatrix * Vector3f(0.0f, m_radius * 2.0f, 1.0f);
	m_vertices[3].globalPosition = transformMatrix * Vector3f(m_radius * 2.0f, m_radius * 2.0f, 1.0f);
}

}
