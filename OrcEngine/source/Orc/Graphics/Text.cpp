#include "OrcPch.hpp"

#include "Graphics/Text.hpp"


namespace orc {

Text::Text(const std::string& string)
{
	setString(string);
}

Text::Text(Ref<Font> font)
{
	setFont(font);
}

Text::Text(Ref<Font> font, const std::string& string)
{
	setFont(font);
	setString(string);
}

void Text::setFont(Ref<Font> font)
{
	m_font = font;
	m_isGeometryUpdateNeeded = true;
}

void Text::setColor(const Color& color)
{
	m_color = color;

	for (GlyphVertex& vertex : m_vertices)
		vertex.color = color.normalized();
}

void Text::setString(const std::string& string)
{
	if (string == m_string)
		return;

	m_string = string;
	m_isGeometryUpdateNeeded = true;
}

Ref<Font> Text::getFont() const
{
	return m_font;
}

const Color& Text::getColor() const
{
	return m_color;
}

std::string_view Text::getString() const
{
	return m_string;
}

const std::vector<GlyphVertex>& Text::getVertices() const
{
	if (m_isGeometryUpdateNeeded)
		updateVertices();

	return m_vertices;
}

FloatRect Text::getLocalRect() const
{
	return m_localRect;
}

FloatRect Text::getGlobalRect() const
{
	if (m_isGeometryUpdateNeeded)
		updateVertices();

	return m_globalRect;
}

void Text::updateVertices() const
{
	if (!m_font)
		return;

	m_vertices.resize(m_string.size() * 4);

	if (m_string.empty())
		return;

	Vector2f position;
	Vector2f bitmapSize = m_font->getBitmap()->getSize();
	const Matrix& transform = getTransformMatrix();

	for (int i = 0; i < m_string.size(); i++)
	{
		Character character = m_font->getCharacter(m_string[i]);

		Vector2f normalizedBitmapCoordStart = character.bitmapCoordStart / bitmapSize;
		Vector2f normalizedBitmapCoordEnd = character.bitmapCoordEnd / bitmapSize;

		float width = character.bitmapCoordEnd.x - character.bitmapCoordStart.x;
		float height = character.bitmapCoordEnd.y - character.bitmapCoordStart.y;

		position.y -= character.offset.y;

		m_vertices[static_cast<size_t>(i) * 4 + 0].color = m_color.normalized();
		m_vertices[static_cast<size_t>(i) * 4 + 0].position = transform * Vector3f(position, 1.0f);
		m_vertices[static_cast<size_t>(i) * 4 + 0].textureCoord = normalizedBitmapCoordStart;

		m_vertices[static_cast<size_t>(i) * 4 + 1].color = m_color.normalized();
		m_vertices[static_cast<size_t>(i) * 4 + 1].position = transform * Vector3f(position + Vector2f(width, 0.0f), 1.0f);
		m_vertices[static_cast<size_t>(i) * 4 + 1].textureCoord = Vector2f(normalizedBitmapCoordEnd.x, normalizedBitmapCoordStart.y);

		m_vertices[static_cast<size_t>(i) * 4 + 2].color = m_color.normalized();
		m_vertices[static_cast<size_t>(i) * 4 + 2].position = transform * Vector3f(position + Vector2f(0.0f, height), 1.0f);
		m_vertices[static_cast<size_t>(i) * 4 + 2].textureCoord = Vector2f(normalizedBitmapCoordStart.x, normalizedBitmapCoordEnd.y);

		m_vertices[static_cast<size_t>(i) * 4 + 3].color = m_color.normalized();
		m_vertices[static_cast<size_t>(i) * 4 + 3].position = transform * Vector3f(position + Vector2f(width, height), 1.0f);
		m_vertices[static_cast<size_t>(i) * 4 + 3].textureCoord = normalizedBitmapCoordEnd;

		position.y = 0.0f;
		position.x += character.advance;

		m_localRect.width = std::max(m_localRect.width, position.x);
		m_localRect.height = std::max(m_localRect.height, height);
	}

	//Not sure how to skip this offset crap so it can be more efficient...
	for (GlyphVertex& vertex : m_vertices)
	{
		vertex.position.y += m_localRect.height - 1;
	}

	calculateGlobalRect();
	m_isGeometryUpdateNeeded = false;
}

void Text::calculateGlobalRect() const
{
	const Matrix& transform = getTransformMatrix();

	Vector2f points[] = {
		transform * Vector3f(m_localRect.x, m_localRect.y, 1.0f),
		transform * Vector3f(m_localRect.x, m_localRect.y + m_localRect.height, 1.0f),
		transform * Vector3f(m_localRect.x + m_localRect.width, m_localRect.y, 1.0f),
		transform * Vector3f(m_localRect.x + m_localRect.width, m_localRect.y + m_localRect.height, 1.0f)
	};

	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left)
			left = points[i].x;
		else if (points[i].x > right)
			right = points[i].x;

		if (points[i].y < top)
			top = points[i].y;
		else if (points[i].y > bottom)
			bottom = points[i].y;
	}

	m_globalRect = FloatRect(left, top, right - left, bottom - top);

}
void Text::onTransformChangeCallback()
{
	m_isGeometryUpdateNeeded = true;
}

}
