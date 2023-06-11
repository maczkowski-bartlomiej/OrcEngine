#include "Graphics/Texture.hpp"
#include "Graphics/Transform.hpp"
#include "Graphics/VertexArray.hpp"

//#include <array>

namespace orc {

class Sprite : public Transform
{
public:
	Sprite();
	Sprite(Reference<Texture> texture);

	Reference<Texture> getTexture() const;

private:
	Reference<Texture> m_texture;
	Reference<VertexArray> m_vertexArray;
	//std::array<Reference<Texture>, 8u> m_overlays;
};

}
