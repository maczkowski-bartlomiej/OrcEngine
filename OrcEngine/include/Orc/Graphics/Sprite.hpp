#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Sprite : public Drawable<SpriteVertex, 4>
{
public:
	Sprite();
	Sprite(Ref<Texture> texture);

	void setColor(const Color& color);
	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);

	Color getColor() const;
	Ref<Texture> getTexture() const;
	FloatRect getTextureRect() const;
	FloatRect getLocalRect() const override;

	//void setVertices(std::array<SpriteVertex, 4> vertices);
private:
	Color m_color;
	Ref<Texture> m_texture;
	FloatRect m_textureRect;

	void updateVerticesTexCoords() const;
	void updateVerticesPositions() const override;

	//std::array<Ref<Texture>, 8> m_overlays;
};

}
