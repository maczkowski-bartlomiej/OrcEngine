#pragma once

#include <string>
#include <vector>

#include "Core/Core.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/Rect.hpp"
#include "Graphics/Transformable.hpp"
#include "Graphics/Vertex.hpp"

namespace orc {

class Text : public Transformable
{
public:
	Text() = default;
	explicit Text(std::string_view string);
	explicit Text(Ref<Font> font);
	Text(Ref<Font> font, std::string_view string);

	void setFont(Ref<Font> font);
	void setColor(const Color& color);
	void setString(std::string_view string);

	const Ref<Font>& getFont() const;
	const Color& getColor() const;
	std::string_view getString() const;
	const std::vector<Vertex>& getVertices() const;

	FloatRect getLocalRect() const;
	FloatRect getGlobalRect() const;

private:
	void updateVertices() const;
	void calculateGlobalRect() const;
	void onTransformChangeCallback() override;

	Color m_color;
	Ref<Font> m_font;

	std::string m_string;
	mutable std::vector<Vertex> m_vertices;

	mutable FloatRect m_localRect;
	mutable FloatRect m_globalRect;

	mutable bool m_isGeometryUpdateNeeded = false;
};

}
