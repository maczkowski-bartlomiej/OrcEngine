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

	float getWidth() const;
	float getHeight() const;
	Vector2f getSize() const;

private:
	Vector2f m_size;
	RendererID m_rendererID;
};

}
