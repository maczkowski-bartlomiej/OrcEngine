#include "OrcPch.hpp"

#include "Engine/Debug.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

Rectangle::Rectangle()
	: m_borderThickness(0.0f)
{
	updateVerticesTexCoords();
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

void Rectangle::setTexture(Ref<Texture> texture)
{
	ORC_ASSERT(texture, "Texture is nullptr");

	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

void Rectangle::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVerticesTexCoords();
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

Ref<Texture> Rectangle::getTexture() const
{
	return m_texture;
}

FloatRect Rectangle::getTextureRect() const
{
	return m_textureRect;
}

FloatRect Rectangle::getLocalRect() const
{
	return FloatRect(0.0f, 0.0f, m_size.x, m_size.y);
}

void Rectangle::updateVerticesTexCoords() const
{
	if (m_texture)
	{
		Vector2f textureSize(m_texture->getSize());
		FloatRect clipSpacedTextureRect(
					m_textureRect.x      / textureSize.x,
					m_textureRect.y      / textureSize.y,
					m_textureRect.width  / textureSize.x,
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

void Rectangle::updateVerticesPositions() const
{
	const Matrix& transformMatrix = getTransformMatrix();
	m_vertices[0].position = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector3f(m_size.x, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector3f(0.0f, m_size.y, 1.0f);
	m_vertices[3].position = transformMatrix * Vector3f(m_size.x, m_size.y, 1.0f);
}

}
