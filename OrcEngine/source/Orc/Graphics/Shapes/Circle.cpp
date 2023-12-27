#include "OrcPch.hpp"

#include "Graphics/Shapes/Circle.hpp"

namespace orc {

Circle::Circle()
	: m_radius(0.0f), m_innerRadius(0), m_borderThickness(0.05f)
{
	std::array<Vector2f, 4> localPositions = {
		Vector2f(-1.0f,  1.0f),
		Vector2f( 1.0f,  1.0f),
		Vector2f(-1.0f, -1.0f),
		Vector2f( 1.0f, -1.0f)
	};

	for (auto& vertex : m_vertices)
	{
		vertex.fillColor = m_fillColor.normalized();
		vertex.borderColor = m_borderColor.normalized();
		vertex.localPosition = localPositions[&vertex - &m_vertices[0]];
		vertex.radius = m_radius;
		vertex.borderThickness = m_borderThickness;
	}
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

	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].radius = m_radius;
	}
}

void Circle::setInnerRadius(float innerRadius)
{
	m_innerRadius = innerRadius;

	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].innerRadius = m_innerRadius;
	}
}

void Circle::setBorderThickness(float borderThickness)
{
	m_borderThickness = borderThickness;

	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].borderThickness = m_borderThickness;
	}
}

void Circle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;
	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].fillColor = fillColor.normalized();
	}
}

void Circle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;
	for (uint64_t i = 0; i < 4; i++)
	{
		m_vertices[i].borderColor = m_borderColor.normalized();
	}
}

float Circle::getRadius() const
{
	return m_radius;
}

float Circle::getInnerRadius() const
{
	return m_innerRadius;
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
