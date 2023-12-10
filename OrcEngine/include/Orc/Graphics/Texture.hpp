#pragma once

#include "Engine/Core.hpp"

namespace orc {

class Texture
{
public:
	Texture();
	Texture(const FilePath& filePath, bool repeat = false);
	~Texture();

	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	Texture operator=(Texture&&) = delete;
	Texture operator=(const Texture&) = delete;


	void setTextureWrapping(bool repeat);
	bool loadFromFile(const FilePath& filePath, bool repeat = false);

	void bind(uint32 slot = 0);

	float getWidth() const;
	float getHeight() const;
	Vector2f getSize() const;
	bool getTextureWrapping() const;

private:
	Vector2f m_size;
	RendererID m_rendererID;
};

}
