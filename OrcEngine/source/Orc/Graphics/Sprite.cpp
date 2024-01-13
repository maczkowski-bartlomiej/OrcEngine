#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"

namespace orc {

Sprite::Sprite()
{
}

Sprite::Sprite(Ref<Texture> texture)
	: Texturable(texture)
{
}

Sprite::Sprite(Ref<Texture> texture, const Vector2f& position)
	: Texturable(texture)
{
	setPosition(position);
}

void Sprite::setColor(const Color& color)
{
	m_color = color;

	for (auto& vertex : m_vertices)
		vertex.color = color.normalized();
}

Color Sprite::getColor() const
{
	return m_color;
}

FloatRect Sprite::getLocalRect() const
{
	return getTextureRect();
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
