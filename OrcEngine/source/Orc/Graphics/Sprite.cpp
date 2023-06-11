#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"

namespace orc {

Sprite::Sprite()
{

}

Sprite::Sprite(Reference<Texture> texture)
	: m_texture(texture)
{
}

Reference<Texture> Sprite::getTexture() const
{
	return m_texture;
}

}
