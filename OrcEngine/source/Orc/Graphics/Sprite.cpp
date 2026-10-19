#include "OrcPch.hpp"
#include "Graphics/Sprite.hpp"

namespace orc {

Sprite::Sprite()
	: m_animator(this)
{
	for (auto& vertex : m_vertices)
		vertex.shapeType = 0.0f;
}

Sprite::Sprite(Ref<Texture> texture)
	: Sprite()
{
	setTexture(std::move(texture));
}

Sprite::Sprite(Ref<Texture> texture, const Vector2f& position)
	: Sprite(std::move(texture))
{
	setPosition(position);
}

void Sprite::setColor(const Color& color)
{
	m_color = color;

	Vector4f normColor = color.normalized();
	for (auto& vertex : m_vertices)
		vertex.color = normColor;
}

Color Sprite::getColor() const
{
	return m_color;
}

FloatRect Sprite::getLocalRect() const
{
	return getTextureRect();
}

Animator& Sprite::getAnimator()
{
	return m_animator;
}

const Animator& Sprite::getAnimator() const
{
	return m_animator;
}

void Sprite::updateVerticesPositions() const
{
	Vector2f size = Vector2f(m_textureRect.right, m_textureRect.bottom);
	const Matrix& transformMatrix = getTransformMatrix();

	m_vertices[0].position = transformMatrix * Vector3f(0.0f, 0.0f, 1.0f);
	m_vertices[1].position = transformMatrix * Vector3f(size.x, 0.0f, 1.0f);
	m_vertices[2].position = transformMatrix * Vector3f(0.0f, size.y, 1.0f);
	m_vertices[3].position = transformMatrix * Vector3f(size.x, size.y, 1.0f);
}

}
