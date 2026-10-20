#pragma once

#include <array>

#include "Engine/Core.hpp"
#include "Graphics/Rect.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Transformable.hpp"
#include "Graphics/Vertex.hpp"

namespace orc {

class Texturable : public Transformable
{
public:
	Texturable();
	explicit Texturable(Ref<Texture> texture);
	~Texturable() override = default;

	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);

	Ref<Texture> getTexture() const;
	FloatRect getTextureRect() const;

	FloatRect getGlobalRect() const;
	virtual FloatRect getLocalRect() const = 0;

	const std::array<Vertex, 4>& getVertices() const;

protected:
	virtual void updateVerticesPositions() const = 0;

	Ref<Texture> m_texture;
	FloatRect m_textureRect;
	mutable FloatRect m_globalRect;
	mutable std::array<Vertex, 4> m_vertices;

private:
	void calculateGlobalRect() const;
	void updateVerticesTexCoords() const;
	void onTransformChangeCallback() override;

	mutable bool m_isVertexPositionsUpdateNeeded = true;
};

}
