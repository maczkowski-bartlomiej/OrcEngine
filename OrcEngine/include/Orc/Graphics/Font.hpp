#pragma once

#include <cstdint>
#include <unordered_map>

#include "Core/Core.hpp"
#include "Graphics/Texture.hpp"

typedef struct FT_FaceRec_* FT_Face;

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
	explicit Font(const FilePath& filePath, uint32_t size = 48);

	void loadFromFile(const FilePath& filePath, uint32_t size = 48);

	uint32_t getSize() const;
	const Ref<Texture>& getBitmap() const;
	const Character& getCharacter(uint32_t character) const;

private:
	void calculateBitmap(FT_Face face);

	uint32_t m_size = 0;

	Ref<Texture> m_bitmap;
	std::unordered_map<uint32_t, Character> m_characters;

	static constexpr uint32_t CHARACTER_COUNT = 127; // Load only ascii characters.
};

}
