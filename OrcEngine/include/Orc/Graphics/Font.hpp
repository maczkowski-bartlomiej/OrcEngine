#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Texture.hpp"
#include "Graphics/FTLibrary.hpp"

#include <freetype/freetype.h>

namespace orc {

struct Character
{
	Vector2f bitmapCoordStart;
	Vector2f bitmapCoordEnd;
	Vector2f offset;
	uint32_t advance;
};

class Font
{
public:
	Font();
	Font(const FilePath& filePath, uint32_t size = 48);
	~Font();

	bool loadFromFile(const FilePath& filePath, uint32_t size = 48);

	uint32_t getSize() const;
	Ref<Texture> getBitmap() const;
	Character getCharacter(char character) const;

private:
	uint32_t m_size = 0;
	FT_Face m_face = nullptr;

	Ref<Texture> m_bitmap;
	std::unordered_map<char, Character> m_characters;
};

}
