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

private:
	void updateVerticesPositions() const override;

	//std::array<Ref<Texture>, 8u> m_overlays;
};

}
