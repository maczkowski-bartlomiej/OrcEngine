#include "Graphics/Text.hpp"

namespace orc {

Text::Text(std::string_view string)
{
	setString(string);
}

Text::Text(Ref<Font> font)
{
	setFont(std::move(font));
}

Text::Text(Ref<Font> font, std::string_view string)
{
	setFont(std::move(font));
	setString(string);
}

void Text::setFont(Ref<Font> font)
{
	m_font = std::move(font);
	m_isGeometryUpdateNeeded = true;
}

void Text::setColor(const Color& color)
{
	m_color = color;

	Vector4f normColor = color.normalized();
	for (Vertex& vertex : m_vertices)
	{
		vertex.color = normColor;
	}
}

void Text::setString(std::string_view string)
{
	if (string == m_string)
	{
		return;
	}

	m_string = string;
	m_isGeometryUpdateNeeded = true;
}

const Ref<Font>& Text::getFont() const
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

const std::vector<Vertex>& Text::getVertices() const
{
	if (m_isGeometryUpdateNeeded)
	{
		updateVertices();
	}

	return m_vertices;
}

FloatRect Text::getLocalRect() const
{
	if (m_isGeometryUpdateNeeded)
	{
		updateVertices();
	}

	return m_localRect;
}

FloatRect Text::getGlobalRect() const
{
	if (m_isGeometryUpdateNeeded)
	{
		updateVertices();
	}

	return m_globalRect;
}

void Text::updateVertices() const
{
	if (!m_font)
	{
		return;
	}

	m_vertices.clear();
	m_localRect = FloatRect(0.0f, 0.0f, 0.0f, 0.0f);

	if (m_string.empty())
	{
		m_globalRect = FloatRect(0.0f, 0.0f, 0.0f, 0.0f);
		m_isGeometryUpdateNeeded = false;
		return;
	}

	m_vertices.reserve(m_string.size() * 4);

	Vector2f position;
	Vector2f bitmapSize = m_font->getBitmap()->getSize();
	const Matrix& transform = getTransformMatrix();
	Vector4f normColor = m_color.normalized();
	const float lineHeight = static_cast<float>(m_font->getSize());

	float maxAscender = 0.0f;
	for (char c : m_string)
	{
		if (c == '\n')
		{
			break;
		}
		const Character& ch = m_font->getCharacter(static_cast<unsigned char>(c));
		maxAscender = std::max(maxAscender, ch.offset.y);
	}
	if (maxAscender == 0.0f)
	{
		maxAscender = lineHeight;
	}

	for (size_t i = 0; i < m_string.size(); i++)
	{
		if (m_string[i] == '\n')
		{
			m_localRect.right = std::max(m_localRect.right, position.x);
			position.x = 0.0f;
			position.y += lineHeight;
			continue;
		}

		const Character& character = m_font->getCharacter(static_cast<unsigned char>(m_string[i]));
		Vector2f normalizedBitmapCoordStart = character.bitmapCoordStart / bitmapSize;
		Vector2f normalizedBitmapCoordEnd = character.bitmapCoordEnd / bitmapSize;

		float width = character.bitmapCoordEnd.x - character.bitmapCoordStart.x;
		float height = character.bitmapCoordEnd.y - character.bitmapCoordStart.y;

		Vector2f glyphPos(position.x, position.y + maxAscender - character.offset.y);

		m_vertices.push_back(Vertex{
			.position = transform * Vector3f(glyphPos, 1.0f),
			.color = normColor,
			.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			.textureCoord = normalizedBitmapCoordStart,
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = 32.0f,
			.shapeType = 3.0f,
			.borderThickness = 0.0f,
			.innerRadius = 0.0f,
			.radius = 0.0f});

		m_vertices.push_back(Vertex{
			.position = transform * Vector3f(glyphPos + Vector2f(width, 0.0f), 1.0f),
			.color = normColor,
			.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			.textureCoord = Vector2f(normalizedBitmapCoordEnd.x, normalizedBitmapCoordStart.y),
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = 32.0f,
			.shapeType = 3.0f,
			.borderThickness = 0.0f,
			.innerRadius = 0.0f,
			.radius = 0.0f});

		m_vertices.push_back(Vertex{
			.position = transform * Vector3f(glyphPos + Vector2f(0.0f, height), 1.0f),
			.color = normColor,
			.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			.textureCoord = Vector2f(normalizedBitmapCoordStart.x, normalizedBitmapCoordEnd.y),
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = 32.0f,
			.shapeType = 3.0f,
			.borderThickness = 0.0f,
			.innerRadius = 0.0f,
			.radius = 0.0f});

		m_vertices.push_back(Vertex{
			.position = transform * Vector3f(glyphPos + Vector2f(width, height), 1.0f),
			.color = normColor,
			.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			.textureCoord = normalizedBitmapCoordEnd,
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = 32.0f,
			.shapeType = 3.0f,
			.borderThickness = 0.0f,
			.innerRadius = 0.0f,
			.radius = 0.0f});

		position.x += character.advance;

		m_localRect.right = std::max(m_localRect.right, position.x);
		m_localRect.bottom = std::max(m_localRect.bottom, position.y + lineHeight);
	}

	calculateGlobalRect();
	m_isGeometryUpdateNeeded = false;
}

void Text::calculateGlobalRect() const
{
	const Matrix& transform = getTransformMatrix();

	Vector2f points[] = {
		transform * Vector3f(m_localRect.left, m_localRect.top, 1.0f),
		transform * Vector3f(m_localRect.left, m_localRect.top + m_localRect.bottom, 1.0f),
		transform * Vector3f(m_localRect.left + m_localRect.right, m_localRect.top, 1.0f),
		transform * Vector3f(m_localRect.left + m_localRect.right, m_localRect.top + m_localRect.bottom, 1.0f)};

	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left)
		{
			left = points[i].x;
		}
		else if (points[i].x > right)
		{
			right = points[i].x;
		}

		if (points[i].y < top)
		{
			top = points[i].y;
		}
		else if (points[i].y > bottom)
		{
			bottom = points[i].y;
		}
	}

	m_globalRect = FloatRect(left, top, right - left, bottom - top);
}

void Text::onTransformChangeCallback()
{
	m_isGeometryUpdateNeeded = true;
}

}
