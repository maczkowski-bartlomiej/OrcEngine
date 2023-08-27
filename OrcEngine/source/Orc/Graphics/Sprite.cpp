#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"

namespace orc {

Sprite::Sprite()
	: m_color(1.0f, 1.0f, 1.0f, 1.0f), m_isVertexPositionsUpdateNeeded(false)
{
}

Sprite::Sprite(Reference<Texture> texture)
	: m_color(1.0f, 1.0f, 1.0f, 1.0f), m_isVertexPositionsUpdateNeeded(false)
{
	setTexture(texture);
}

void Sprite::setColor(const Vector4f& color)
{
	m_color = color;
	updateVertexColors();
}

void Sprite::setTexture(Reference<Texture> texture)
{
	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

void Sprite::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	setSize(m_textureRect.width - textureRect.x, m_textureRect.height - textureRect.y);
	updateVertexTextureCoords();
}

Vector4f Sprite::getColor() const
{
	return m_color;
}

IntRect Sprite::getTextureRect() const
{
	return m_textureRect;
}

Reference<Texture> Sprite::getTexture() const
{
	return m_texture;
}

TexturedVertex Sprite::getVertex(uint32 index)
{
	ORC_ASSERT(index >= 0 && index <= 4, "Invalid vertex index '{}'", index);

	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVertexPositions();
	}

	return m_vertices[index];
}

const std::array<TexturedVertex, 4u>& Sprite::getVertices()
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVertexPositions();
	}

	return m_vertices;
}

void Sprite::updateVertexColors()
{
	m_vertices[0].color = m_color;
	m_vertices[1].color = m_color;
	m_vertices[2].color = m_color;
	m_vertices[3].color = m_color;
}

void Sprite::updateVertexPositions()
{
	const Matrix& transformMatrix = getTransformMatrix();
	m_vertices[0].position = transformMatrix * Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[3].position = transformMatrix * Vector4f(1.0f, 1.0f, 0.0f, 1.0f);

	m_isVertexPositionsUpdateNeeded = false;
}

void Sprite::updateVertexTextureCoords()
{
	FloatRect clipSpacedTextureRect = m_textureRect;
	clipSpacedTextureRect.x /= m_texture->getSize().x;
	clipSpacedTextureRect.y /= m_texture->getSize().y;
	clipSpacedTextureRect.width /= m_texture->getSize().x;
	clipSpacedTextureRect.height /= m_texture->getSize().y;

	m_vertices[0].textureCoords = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.y);
	m_vertices[1].textureCoords = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.height);
	m_vertices[2].textureCoords = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.height);
	m_vertices[3].textureCoords = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.y);
}

void Sprite::onTransformChangeCallback()
{
	m_isVertexPositionsUpdateNeeded = true;
}


}
