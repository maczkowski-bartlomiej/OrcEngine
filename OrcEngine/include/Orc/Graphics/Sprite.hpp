#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Transformable.hpp"
#include "Graphics/Vertices.hpp"
#include "Graphics/VertexArray.hpp"

namespace orc {

class Sprite : public Transformable
{
public:
	Sprite();
	Sprite(Reference<Texture> texture);
	
	void setColor(const Vector4f& color);
	void setTexture(Reference<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);

	Vector4f getColor() const;
	IntRect getTextureRect() const;

	Reference<Texture> getTexture() const;
	TexturedVertex getVertex(uint32 index);
	const std::array<TexturedVertex, 4u>& getVertices();

private:
	void updateVertexColors();
	void updateVertexPositions();
	void updateVertexTextureCoords();
	void onTransformChangeCallback() override;

	Vector4f m_color;
	FloatRect m_textureRect;

	Reference<Texture> m_texture;
	std::array<TexturedVertex, 4u> m_vertices;

	bool m_isVertexPositionsUpdateNeeded;

	//std::array<Reference<Texture>, 8u> m_overlays;
};

}
