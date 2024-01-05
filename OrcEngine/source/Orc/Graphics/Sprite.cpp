#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"

namespace orc {

Sprite::Sprite()
{
	updateVerticesTexCoords();
}

Sprite::Sprite(Ref<Texture> texture)
	: Sprite()
{
	setTexture(texture);
}

void Sprite::setColor(const Color& color)
{
	m_color = color;

	for (auto& vertex : m_vertices)
		vertex.color = color.normalized();
}

void Sprite::setTexture(Ref<Texture> texture)
{
	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

void Sprite::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVerticesTexCoords();
}

Color Sprite::getColor() const
{
	return m_color;
}

Ref<Texture> Sprite::getTexture() const
{
	return m_texture;
}

FloatRect Sprite::getTextureRect() const
{
	return m_textureRect;
}

FloatRect Sprite::getLocalRect() const
{
	return getTextureRect();
}

void Sprite::updateVerticesTexCoords() const
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

void Sprite::updateVerticesPositions() const
{
	Vector2f size = Vector2f(m_textureRect.width, m_textureRect.height);
	const Matrix& transformMatrix = getTransformMatrix();

	m_vertices[0].position = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector3f(size.x, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector3f(0.0f, size.y, 1.0f);
	m_vertices[3].position = transformMatrix * Vector3f(size.x, size.y, 1.0f);
}

}
