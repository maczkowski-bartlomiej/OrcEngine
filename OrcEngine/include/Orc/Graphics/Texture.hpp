#pragma once

#include "Engine/Core.hpp"

namespace orc {

class Texture
{
public:
	enum class TextureMode
	{
		RED,
		GREEN,
		BLUE,
		RGB,
		RGBA
	};

	Texture();
	Texture(const FilePath& filePath);
	~Texture();

	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	Texture operator=(Texture&&) = delete;
	Texture operator=(const Texture&) = delete;

	bool loadFromFile(const FilePath& filePath);
	bool loadFromMemory(const void* data, uint32_t width, uint32_t height, const TextureMode& textureMode);

	void bind(uint32_t slot = 0);

	float getWidth() const;
	float getHeight() const;
	Vector2f getSize() const;

private:
	Vector2f m_size;
	RendererID m_rendererID;
};

}
