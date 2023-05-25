#pragma once

#include "Engine/Core.hpp"

namespace orc {

class Texture
{
public:
	Texture();
	Texture(const FilePath& filePath);
	~Texture();

	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	Texture operator=(Texture&&) = delete;
	Texture operator=(const Texture&) = delete;

	bool loadFromFile(const FilePath& filePath);

	void bind(uint32 slot = 0);

	int32 getWidth() const;
	int32 getHeight() const;

private:
	int32 m_width;
	int32 m_height;
	RendererID m_rendererID;
};

}
