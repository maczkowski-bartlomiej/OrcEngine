#include "OrcPch.hpp"

#include "Graphics/Font.hpp"

#include <cstdint>
#include <vector>

#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <freetype/fttypes.h>

#include "Core/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
#include "Engine/Utility.hpp"

namespace orc {

Font::Font(const FilePath& filePath, uint32_t size)
{
	loadFromFile(filePath, size);
}

bool Font::loadFromFile(const FilePath& filePath, uint32_t size)
{
	FT_Library ftLibrary = nullptr;
	if (!ftCall(FT_Init_FreeType(&ftLibrary)))
	{
		ORC_LOG_ERROR("Failed to initialize FreeType Library.");
		return false;
	}

	FT_Face face = nullptr;
	if (!ftCall(FT_New_Face(ftLibrary, filePath.string().c_str(), 0, &face)))
	{
		ORC_LOG_ERROR("Failed to load font from {}", filePath.string());
		ftCall(FT_Done_FreeType(ftLibrary));
		return false;
	}

	if (!ftCall(FT_Set_Pixel_Sizes(face, 0, size)))
	{
		ORC_LOG_ERROR("Failed to set font pixel size of {}", size);
		ftCall(FT_Done_Face(face));
		ftCall(FT_Done_FreeType(ftLibrary));
		return false;
	}

	m_size = size;

	calculateBitmap(face);

	if (!ftCall(FT_Done_Face(face)))
	{
		ORC_LOG_WARNING("Failed to deinitialize font");
	}

	if (!ftCall(FT_Done_FreeType(ftLibrary)))
	{
		ORC_LOG_WARNING("Failed to deinitialize FreeType Library.");
	}

	return true;
}

void Font::calculateBitmap(FT_Face face)
{
	uint32_t fontHeightInPixels = static_cast<uint32_t>(face->size->metrics.height) >> 6; // 26.6 fixed point format, so we need to shift right by 6 to get the pixel value
	uint32_t charactersPerRow = static_cast<uint32_t>(std::ceilf(sqrtf(static_cast<float>(M_CHARACTER_COUNT))));
	uint32_t maxDimension = (1 + fontHeightInPixels) * charactersPerRow;

	uint32_t textureWidth = 1;
	while (textureWidth < maxDimension)
		textureWidth <<= 1;
	uint32_t textureHeight = textureWidth;

	std::vector<unsigned char> rawPixels(textureWidth * textureHeight);
	std::vector<unsigned char> buffer;

	uint32_t currentX = 0, currentY = 0;
	for (uint32_t i = 0; i < M_CHARACTER_COUNT; i++)
	{
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bitmap = &face->glyph->bitmap;

		if (currentX + bitmap->width >= textureWidth)
		{
			currentX = 0;
			currentY += (static_cast<uint32_t>(face->size->metrics.height) >> 6) + 1;
		}

		for (uint32_t row = 0; row < bitmap->rows; row++)
		{
			for (uint32_t col = 0; col < bitmap->width; col++)
			{
				uint32_t x = currentX + col;
				uint32_t y = currentY + row;
				rawPixels[y * textureWidth + x] = bitmap->buffer[row * bitmap->pitch + col];
			}
		}

		m_characters[i].bitmapCoordStart = Vector2f(currentX, currentY);
		m_characters[i].bitmapCoordEnd = Vector2f(currentX + bitmap->width, currentY + bitmap->rows);
		m_characters[i].offset = Vector2f(face->glyph->bitmap_left, face->glyph->bitmap_top);
		m_characters[i].advance = static_cast<uint32_t>(face->glyph->advance.x) >> 6;

		currentX += bitmap->width + 1;
	}

	m_bitmap = createRef<Texture>();
	m_bitmap->loadFromMemory(rawPixels.data(), textureWidth, textureHeight, Texture::TextureMode::RED);
}

uint32_t Font::getSize() const
{
	return m_size;
}

Ref<Texture> Font::getBitmap() const
{
	return m_bitmap;
}

Character Font::getCharacter(uint32_t character) const
{
	if (character > M_CHARACTER_COUNT)
	{
		ORC_LOG_WARNING("Attempted to get a non-ascii character from a font. Character code: {}.", character);
	}

	return m_characters.at(character);
}

}
