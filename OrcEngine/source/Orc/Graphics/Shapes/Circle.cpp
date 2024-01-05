#include "OrcPch.hpp"

#include "Graphics/Shapes/Circle.hpp"

namespace orc {

Circle::Circle()
	: m_radius(0.0f), m_innerRadius(0.0f), m_borderThickness(0.0)
{
	std::array<Vector2f, 4> localPositions = {
		Vector2f(-1.0f,  1.0f),
		Vector2f(1.0f,  1.0f),
		Vector2f(-1.0f, -1.0f),
		Vector2f(1.0f, -1.0f)
	};

	for (auto& vertex : m_vertices)
		vertex.localPosition = localPositions[&vertex - &m_vertices[0]];

	updateVerticesTexCoords();
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
	m_radius = radius;

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

void Circle::setTexture(Ref<Texture> texture)
{
	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

void Circle::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVerticesTexCoords();
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

Ref<Texture> Circle::getTexture() const
{
	return m_texture;
}

FloatRect Circle::getTextureRect() const
{
	return m_textureRect;
}

FloatRect Circle::getLocalRect() const
{
	return FloatRect(0.0f, 0.0f, m_radius * 2.0f, m_radius * 2.0f);
}

void Circle::updateVerticesTexCoords() const
{
	if (m_texture)
	{
		Vector2f textureSize(m_texture->getSize());
		FloatRect clipSpacedTextureRect(
			m_textureRect.x / textureSize.x,
			m_textureRect.y / textureSize.y,
			m_textureRect.width / textureSize.x,
			m_textureRect.height / textureSize.y
		);

		m_vertices[0].textureCoord = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.y);
		m_vertices[1].textureCoord = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.y);
		m_vertices[2].textureCoord = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.height);
		m_vertices[3].textureCoord = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.height);
	}
	else
	{
		m_vertices[0].textureCoord = Vector2f(0.0f, 0.0f);
		m_vertices[1].textureCoord = Vector2f(1.0f, 0.0f);
		m_vertices[2].textureCoord = Vector2f(0.0f, 1.0f);
		m_vertices[3].textureCoord = Vector2f(1.0f, 1.0f);
	}
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
