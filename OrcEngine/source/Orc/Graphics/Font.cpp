#include "Graphics/Font.hpp"

#include <cstdint>
#include <vector>

#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <freetype/fttypes.h>

#include "Core/Core.hpp"
#include "Core/Debug.hpp"
#include "Core/Logger.hpp"

#define ORC_FT_CALL(call, message, ...) \
	do \
	{ \
		const FT_Error ftResult = (call); \
		ORC_FATAL_CHECK(ftResult == FT_Err_Ok, message "\n\tFreeType Error: {}" __VA_OPT__(, ) __VA_ARGS__, FT_Error_String(ftResult)); \
	} while (0)

namespace orc {

Font::Font(const FilePath& filePath, uint32_t size)
{
	loadFromFile(filePath, size);
}

void Font::loadFromFile(const FilePath& filePath, uint32_t size)
{
	FT_Library ftLibrary = nullptr;
	ORC_FT_CALL(
		FT_Init_FreeType(&ftLibrary),
		"Failed to initialize FreeType Library.");

	FT_Face face = nullptr;
	ORC_FT_CALL(
		FT_New_Face(ftLibrary, filePath.string().c_str(), 0, &face),
		"Failed to load font from '{}'.", filePath.string());
	ORC_FT_CALL(
		FT_Set_Pixel_Sizes(face, 0, size),
		"Failed to set font pixel size of {}.", size);

	m_size = size;

	calculateBitmap(face);

	ORC_FT_CALL(
		FT_Done_Face(face),
		"Failed to deinitialize font.");
	ORC_FT_CALL(
		FT_Done_FreeType(ftLibrary),
		"Failed to deinitialize FreeType Library.");
}

void Font::calculateBitmap(FT_Face face)
{
	uint32_t fontHeightInPixels = static_cast<uint32_t>(face->size->metrics.height) >> 6; // 26.6 fixed point format, so we need to shift right by 6 to get the pixel value
	uint32_t charactersPerRow = static_cast<uint32_t>(std::ceilf(sqrtf(static_cast<float>(CHARACTER_COUNT))));
	uint32_t maxDimension = (1 + fontHeightInPixels) * charactersPerRow;

	uint32_t textureWidth = 1;
	while (textureWidth < maxDimension)
	{
		textureWidth <<= 1;
	}
	uint32_t textureHeight = textureWidth;

	std::vector<unsigned char> rawPixels(textureWidth * textureHeight);

	uint32_t currentX = 0, currentY = 0;
	for (uint32_t i = 0; i < CHARACTER_COUNT; i++)
	{
		ORC_FT_CALL(
			FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT),
			"Failed to load character code {} from font.", i);

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
	m_bitmap->loadFromMemory(rawPixels, textureWidth, textureHeight, Texture::TextureMode::Red);
}

uint32_t Font::getSize() const
{
	return m_size;
}

const Ref<Texture>& Font::getBitmap() const
{
	return m_bitmap;
}

const Character& Font::getCharacter(uint32_t character) const
{
	if (character >= CHARACTER_COUNT)
	{
		ORC_LOG_WARNING("Attempted to get a non-ascii character from a font. Character code: {}.", character);
		static constexpr Character fallback{};
		if (auto it = m_characters.find('?'); it != m_characters.end())
		{
			return it->second;
		}
		if (!m_characters.empty())
		{
			return m_characters.begin()->second;
		}
		return fallback;
	}

	auto it = m_characters.find(character);
	if (it != m_characters.end())
	{
		return it->second;
	}

	static constexpr Character fallback{};
	return fallback;
}

}
