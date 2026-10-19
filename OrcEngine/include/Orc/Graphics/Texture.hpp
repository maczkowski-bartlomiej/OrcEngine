#pragma once

#include "Engine/Core.hpp"

#include <cstdint>

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
	explicit Texture(const FilePath& filePath);
	~Texture();

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	bool loadFromFile(const FilePath& filePath);
	bool loadFromMemory(const void* data, uint32_t width, uint32_t height, TextureMode textureMode);

	void bind(uint32_t slot = 0) const;

	float getWidth() const;
	float getHeight() const;
	Vector2f getSize() const;

	RendererID getRendererID() const;

private:
	Vector2f m_size = { 0.0f, 0.0f };
	RendererID m_rendererID = 0;
};

}
