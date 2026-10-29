#include "Graphics/Texturable.hpp"

namespace orc {

Texturable::Texturable()
{
	updateVerticesTexCoords();
}

Texturable::Texturable(Ref<Texture> texture)
{
	setTexture(std::move(texture));
	updateVerticesTexCoords();
}

void Texturable::setTexture(Ref<Texture> texture)
{
	m_texture = std::move(texture);
	if (m_texture)
	{
		setTextureRect(FloatRect(0.0f, 0.0f, m_texture->getSize().x, m_texture->getSize().y));
	}
	else
	{
		updateVerticesTexCoords();
	}
}

void Texturable::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVerticesTexCoords();
}

const Ref<Texture>& Texturable::getTexture() const
{
	return m_texture;
}

FloatRect Texturable::getTextureRect() const
{
	return m_textureRect;
}

FloatRect Texturable::getGlobalRect() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		calculateGlobalRect();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_globalRect;
}

const std::array<Vertex, 4>& Texturable::getVertices() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		calculateGlobalRect();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices;
}

void Texturable::calculateGlobalRect() const
{
	FloatRect localRect = getLocalRect();
	const Matrix& transform = getTransformMatrix();

	Vector2f points[] = {
		transform * Vector3f(localRect.left, localRect.top, 1.0f),
		transform * Vector3f(localRect.left, localRect.top + localRect.height, 1.0f),
		transform * Vector3f(localRect.left + localRect.width, localRect.top, 1.0f),
		transform * Vector3f(localRect.left + localRect.width, localRect.top + localRect.height, 1.0f)};

	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left)
		{
			left = points[i].x;
		}
		else if (points[i].x > right)
		{
			right = points[i].x;
		}

		if (points[i].y < top)
		{
			top = points[i].y;
		}
		else if (points[i].y > bottom)
		{
			bottom = points[i].y;
		}
	}

	m_globalRect = FloatRect(left, top, right - left, bottom - top);
}

void Texturable::updateVerticesTexCoords() const
{
	if (m_texture)
	{
		const Vector2f textureSize(m_texture->getSize());
		const float u0 = m_textureRect.left / textureSize.x;
		const float v0 = m_textureRect.top / textureSize.y;
		const float u1 = (m_textureRect.left + m_textureRect.width) / textureSize.x;
		const float v1 = (m_textureRect.top + m_textureRect.height) / textureSize.y;

		m_vertices[0].textureCoord = Vector2f(u0, v0);
		m_vertices[1].textureCoord = Vector2f(u1, v0);
		m_vertices[2].textureCoord = Vector2f(u0, v1);
		m_vertices[3].textureCoord = Vector2f(u1, v1);
	}
	else
	{
		m_vertices[0].textureCoord = Vector2f(0.0f, 0.0f);
		m_vertices[1].textureCoord = Vector2f(1.0f, 0.0f);
		m_vertices[2].textureCoord = Vector2f(0.0f, 1.0f);
		m_vertices[3].textureCoord = Vector2f(1.0f, 1.0f);
	}
}

void Texturable::onTransformChangeCallback()
{
	m_isVertexPositionsUpdateNeeded = true;
}

}
