#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Vertex.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Sprite : public Drawable<Vertex, 4>
{
public:
	Sprite() = default;
	Sprite(Ref<Texture> texture);

	FloatRect getLocalRect() const override;

	Color getColor() const
	{
		return m_color;
	}
	void setColor(const Color& color)
	{
		m_color = color;
		for (int i = 0; i < 4; i++)
		{
			m_vertices[i].color = color.normalized();
		}
	}

private:
	Color m_color;
	void updateVerticesPositions() const override;

	//std::array<Ref<Texture>, 8u> m_overlays;
};

}
