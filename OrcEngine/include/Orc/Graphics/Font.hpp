#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Texture.hpp"
#include "Graphics/FTLibrary.hpp"


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

	Ref<Texture> getBitmap() const
	{
		return m_bitmap;
	}

	Character getCharacter(char c) const
	{
		return m_characters.at(c);
	}

private:
	std::unordered_map<char, Character> m_characters;

	FT_Face m_face;
	Ref<Texture> m_bitmap;
};

}
