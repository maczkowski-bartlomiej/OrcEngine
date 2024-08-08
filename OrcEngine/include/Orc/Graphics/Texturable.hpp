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

}

#include "Graphics/Texturable.inl"
