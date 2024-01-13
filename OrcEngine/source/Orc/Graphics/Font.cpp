#include "OrcPch.hpp"

#include "Graphics/Font.hpp"

#include "Engine/Debug.hpp"

#include <vector>
#include <algorithm>

//Credits for help
//https://gist.github.com/baines/b0f9e4be04ba4e6f56cab82eef5008ff and https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/Font.cpp#L352

namespace orc {

Font::Font()
{
		
}

Font::Font(const FilePath& filePath, uint32_t size)
{
	loadFromFile(filePath, size);
}

Font::~Font()
{
	if (m_face)
	{
		FT_Done_Face(m_face);
	}
}

bool Font::loadFromFile(const FilePath& filePath, uint32_t size)
{
	FT_Library& ftLibrary = FTLibrary::getLibrary();

	FT_Error error;
	error = FT_New_Face(ftLibrary, filePath.string().c_str(), 0, &m_face);
	if (error)
	{
		ORC_ERROR("Failed to load font {}\n\treason: {}", filePath.string(), FT_Error_String(error));
		return false;
	}

	error = FT_Set_Pixel_Sizes(m_face, 0, size);
	if (error == FT_Err_Invalid_Pixel_Size)
	{
		if (!FT_IS_SCALABLE(m_face))
		{
			ORC_LOG_ERROR("Failed to set font size {}\n\treason: {}", size, FT_Error_String(error));
			ORC_LOG_ERROR("Available sizes are: ");
			for (int i = 0; i < m_face->num_fixed_sizes; ++i)
			{
				const long size = (m_face->available_sizes[i].y_ppem + 32) >> 6;
				ORC_LOG_ERROR("{}", size);
			}
		}
		else
		{
			ORC_ERROR("Failed to set font size {}\n\treason: {}", size, FT_Error_String(error));
			return false;
		}
	}
	else if (error)
	{
		ORC_ERROR("Failed to set font size {}\n\treason: {}", size, FT_Error_String(error));
		return false;
	}

	constexpr size_t CHARACTER_COUNT = 128;
	uint32_t fontHeightInPixels = m_face->size->metrics.height >> 6;
	uint32_t charactersPerRow = static_cast<uint32_t>(std::ceilf(sqrtf(CHARACTER_COUNT)));
	uint32_t maxDimension = (1 + fontHeightInPixels) * charactersPerRow;

	// Find the smallest power of 2 greater than or equal to maxDimension
	uint32_t textureWidth = 1;
	while (textureWidth < maxDimension)
		textureWidth <<= 1;

	uint32_t textureHeight = textureWidth;

	std::vector<unsigned char> pixels(textureWidth * textureHeight);
	std::vector<unsigned char> buffer;

	int32_t currentX = 0, currentY = 0;
	for (int32_t i = 0; i < CHARACTER_COUNT; ++i)
	{
		FT_Load_Char(m_face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bitmap = &m_face->glyph->bitmap;

		if (currentX + bitmap->width >= textureWidth)
		{
			currentX = 0;
			currentY += ((m_face->size->metrics.height >> 6) + 1);
		}

		for (uint32_t row = 0; row < bitmap->rows; row++)
		{
			for (uint32_t col = 0; col < bitmap->width; col++)
			{
				uint32_t x = currentX + col;
				uint32_t y = currentY + row;
				pixels[y * textureWidth + x] = bitmap->buffer[row * bitmap->pitch + col];
			}
		}

		m_characters[i].bitmapCoordStart = Vector2f(currentX, currentY);
		m_characters[i].bitmapCoordEnd = Vector2f(currentX + bitmap->width, currentY + bitmap->rows);
		m_characters[i].offset = Vector2f(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top);
		m_characters[i].advance = m_face->glyph->advance.x >> 6;

		currentX += bitmap->width + 1;
	}

	m_bitmap = createRef<Texture>();
	m_bitmap->loadFromMemory(pixels.data(), textureWidth, textureHeight, Texture::TextureMode::RED);
}

}
