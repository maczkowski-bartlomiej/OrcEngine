#pragma once

#include "Graphics/Font.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Transformable.hpp"

#include <string>
#include <vector>

namespace orc {

class Text : public Transformable
{
public:
	Text(Ref<Font> font)
		: m_font(font) {}

	void setString(const std::string& string)
	{
		if (string == m_string)
			return;

		m_string = string;
		updateVertices();
	}


	Ref<Font> getFont() const
	{
		return m_font;
	}

	const std::vector<GlyphVertex>& getVertices() const
	{
		return m_vertices;
	}

	std::string_view getString() const
	{
		return m_string;
	}

	void updateVertices()
	{
		if (!m_font)
			return;

		m_vertices.resize(m_string.size() * 4);

		Vector2f position;
		Vector2f bitmapSize = m_font->getBitmap()->getSize();
		const Matrix& transformMatrix = getTransformMatrix();

		for (int i = 0; i < m_string.size(); i++)
		{
			Character character = m_font->getCharacter(m_string[i]);

			Vector2f normalizedBitmapCoordStart = character.bitmapCoordStart / bitmapSize;
			Vector2f normalizedBitmapCoordEnd = character.bitmapCoordEnd / bitmapSize;
			
			float width = character.bitmapCoordEnd.x - character.bitmapCoordStart.x;
			float height = character.bitmapCoordEnd.y - character.bitmapCoordStart.y;

			position.y -= character.offset.y;

			m_vertices[static_cast<size_t>(i) * 4 + 0].color = m_color.normalized();
			m_vertices[static_cast<size_t>(i) * 4 + 0].position = transformMatrix * Vector3f(position, 1.0f);
			m_vertices[static_cast<size_t>(i) * 4 + 0].textureCoord = normalizedBitmapCoordStart;

			m_vertices[static_cast<size_t>(i) * 4 + 1].color = m_color.normalized();
			m_vertices[static_cast<size_t>(i) * 4 + 1].position = transformMatrix * Vector3f(position + Vector2f(width, 0.0f), 1.0f);
			m_vertices[static_cast<size_t>(i) * 4 + 1].textureCoord = Vector2f(normalizedBitmapCoordEnd.x, normalizedBitmapCoordStart.y);

			m_vertices[static_cast<size_t>(i) * 4 + 2].color = m_color.normalized();
			m_vertices[static_cast<size_t>(i) * 4 + 2].position = transformMatrix * Vector3f(position + Vector2f(0.0f, height), 1.0f);
			m_vertices[static_cast<size_t>(i) * 4 + 2].textureCoord = Vector2f(normalizedBitmapCoordStart.x, normalizedBitmapCoordEnd.y);

			m_vertices[static_cast<size_t>(i) * 4 + 3].color = m_color.normalized();
			m_vertices[static_cast<size_t>(i) * 4 + 3].position = transformMatrix * Vector3f(position + Vector2f(width, height), 1.0f);
			m_vertices[static_cast<size_t>(i) * 4 + 3].textureCoord = normalizedBitmapCoordEnd;

			position.y = 0.0f;
			position.x += character.advance;
		}
	}

//private:
	void onTransformChangeCallback() override
	{
		updateVertices();
	}

	std::vector<GlyphVertex> m_vertices;
	std::string m_string;
	Ref<Font> m_font;
	Color m_color;
};

}
