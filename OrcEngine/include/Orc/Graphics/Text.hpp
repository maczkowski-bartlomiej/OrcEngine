#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Transformable.hpp"

#include <string>
#include <vector>

namespace orc {

class Text : public Transformable
{
public:
	Text() = default;
	Text(const std::string& string);
	Text(Ref<Font> font);
	Text(Ref<Font> font, const std::string& string);

	void setFont(Ref<Font> font);
	void setColor(const Color& color);
	void setString(const std::string& string);

	Ref<Font> getFont() const;
	const Color& getColor() const;
	std::string_view getString() const;
	const std::vector<GlyphVertex>& getVertices() const;

	FloatRect getLocalRect() const;
	FloatRect getGlobalRect() const;

private:
	void updateVertices() const;
	void calculateGlobalRect() const;
	void onTransformChangeCallback() override;

	Color m_color;
	Ref<Font> m_font;

	std::string m_string;
	mutable std::vector<GlyphVertex> m_vertices;

	mutable FloatRect m_localRect;
	mutable FloatRect m_globalRect;

	mutable bool m_isGeometryUpdateNeeded = false;
};

}
