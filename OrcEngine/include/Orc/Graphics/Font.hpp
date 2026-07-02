#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Texture.hpp"
#include <cstdint>
#include <unordered_map>
#include <freetype/freetype.h>

namespace orc {

struct Character
{
	Vector2f bitmapCoordStart;
	Vector2f bitmapCoordEnd;
	Vector2f offset;
	uint32_t advance = 0u;
};

class Font
{
public:
	Font() = default;
	Font(const FilePath& filePath, uint32_t size = 48);
	~Font();

	bool loadFromFile(const FilePath& filePath, uint32_t size = 48);

	uint32_t getSize() const;
	Ref<Texture> getBitmap() const;
    Character getCharacter(uint32_t character) const;

private:
	void calculateBitmap();

	uint32_t m_size = 0;
	FT_Face m_face = nullptr;

	Ref<Texture> m_bitmap;
	std::unordered_map<uint32_t, Character> m_characters;

	const uint32_t M_CHARACTER_COUNT = 127; //Load only ascii characters.
};

}
