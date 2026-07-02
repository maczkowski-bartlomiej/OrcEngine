#include "OrcPch.hpp"
#include "Graphics/Shapes/Circle.hpp"

#include <cmath>

namespace orc {

Circle::Circle()
{
	constexpr std::array<Vector2f, 4> localPositions = {
		Vector2f(-1.0f,  1.0f),
		Vector2f(1.0f,  1.0f),
		Vector2f(-1.0f, -1.0f),
		Vector2f(1.0f, -1.0f)
	};

	for (size_t i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i].localPosition = localPositions[i];
	}
}

Circle::Circle(float radius, const Color& color)
	: Circle()
{
	setRadius(radius);
	setFillColor(color);
}

Circle::Circle(const Vector2f& position, float radius, const Color& color)
	: Circle()
{
	setPosition(position);
	setRadius(radius);
	setFillColor(color);
}

void Circle::setFillColor(const Color& fillColor)
{
	m_fillColor = fillColor;

	for (auto& vertex : m_vertices)
		vertex.fillColor = m_fillColor.normalized();
}

void Circle::setBorderColor(const Color& borderColor)
{
	m_borderColor = borderColor;

	for (auto& vertex : m_vertices)
		vertex.borderColor = m_borderColor.normalized();
}

void Circle::setRadius(float radius)
{
	m_radius = std::abs(radius);

	for (auto& vertex : m_vertices)
		vertex.radius = m_radius;
}

void Circle::setInnerRadius(float innerRadius)
{
	m_innerRadius = innerRadius;

	for (auto& vertex : m_vertices)
		vertex.innerRadius = m_innerRadius;
}

void Circle::setBorderThickness(float borderThickness)
{
	m_borderThickness = borderThickness;

	for (auto& vertex : m_vertices)
		vertex.borderThickness = m_borderThickness;
}

Color Circle::getFillColor() const
{
	return m_fillColor;
}

Color Circle::getBorderColor() const
{
	return m_borderColor;
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
