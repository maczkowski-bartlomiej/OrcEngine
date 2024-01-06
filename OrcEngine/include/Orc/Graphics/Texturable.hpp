#pragma once

#include <array>

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Transformable.hpp"

namespace orc {

template<typename VertexType, uint64_t VerticesCount>
class Texturable : public Transformable
{
public:
	Texturable();
	Texturable(Ref<Texture> texture);

	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);
	void setVertices(const std::array<VertexType, VerticesCount>& vertices);

	Ref<Texture> getTexture() const;
	FloatRect getTextureRect() const;

	FloatRect getGlobalRect() const;
	virtual FloatRect getLocalRect() const = 0;

	VertexType getVertex(uint64_t index) const;
	const std::array<VertexType, VerticesCount>& getVertices() const;

protected:
	virtual void updateVerticesPositions() const = 0;

	Ref<Texture> m_texture;
	FloatRect m_textureRect;
	mutable FloatRect m_globalRect;
	mutable std::array<VertexType, VerticesCount> m_vertices;

private:
	void calculateGlobalRect() const;
	void updateVerticesTexCoords() const;
	void onTransformChangeCallback() override;
	
	mutable bool m_isVertexPositionsUpdateNeeded;
};

template<typename VertexType, uint64_t VerticesCount>
Texturable<VertexType, VerticesCount>::Texturable()
	: m_isVertexPositionsUpdateNeeded(true)
{
	updateVerticesTexCoords();
}

template<typename VertexType, uint64_t VerticesCount>
Texturable<VertexType, VerticesCount>::Texturable(Ref<Texture> texture)
	: m_isVertexPositionsUpdateNeeded(true)
{
	setTexture(texture);
	updateVerticesTexCoords();
}

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::setTexture(Ref<Texture> texture)
{
	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVerticesTexCoords();
}

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::setVertices(const std::array<VertexType, VerticesCount>& vertices)
{
	m_vertices = vertices;
}

template<typename VertexType, uint64_t VerticesCount>
Ref<Texture> Texturable<VertexType, VerticesCount>::getTexture() const
{
	return m_texture;
}

template<typename VertexType, uint64_t VerticesCount>
FloatRect Texturable<VertexType, VerticesCount>::getTextureRect() const
{
	return m_textureRect;
}

template<typename VertexType, uint64_t VerticesCount>
FloatRect Texturable<VertexType, VerticesCount>::getGlobalRect() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		calculateGlobalRect();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_globalRect;
}

template<typename VertexType, uint64_t VerticesCount>
VertexType Texturable<VertexType, VerticesCount>::getVertex(uint64_t index) const
{
	ORC_ASSERT(index >= 0 && index <= VerticesCount, "Invalid vertex index '{}'", index);

	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		calculateGlobalRect();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices[index];
}

template<typename VertexType, uint64_t VerticesCount>
const std::array<VertexType, VerticesCount>& Texturable<VertexType, VerticesCount>::getVertices() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		calculateGlobalRect();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices;
}

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::calculateGlobalRect() const
{
	FloatRect localRect = getLocalRect();
	const Matrix& transform = getTransformMatrix();

	Vector2f points[] = {
		transform * Vector3f(localRect.x, localRect.y, 1.0f),
		transform * Vector3f(localRect.x, localRect.y + localRect.height, 1.0f),
		transform * Vector3f(localRect.x + localRect.width, localRect.y, 1.0f),
		transform * Vector3f(localRect.x + localRect.width, localRect.y + localRect.height, 1.0f)
	};

	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left)
			left = points[i].x;
		else if (points[i].x > right)
			right = points[i].x;

		if (points[i].y < top)
			top = points[i].y;
		else if (points[i].y > bottom)
			bottom = points[i].y;
	}

	m_globalRect = FloatRect(left, top, right - left, bottom - top);
}

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::updateVerticesTexCoords() const
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

template<typename VertexType, uint64_t VerticesCount>
void Texturable<VertexType, VerticesCount>::onTransformChangeCallback()
{
	m_isVertexPositionsUpdateNeeded = true;
}

}
