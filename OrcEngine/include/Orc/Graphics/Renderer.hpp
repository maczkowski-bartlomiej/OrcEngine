#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Text.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"

#include "Graphics/Shapes/Circle.hpp"
#include "Graphics/Shapes/Rectangle.hpp"

#include <array>

namespace orc {

class Renderer
{
public:
	Renderer();

	void clear();
	void setClearColor(const Color& color);

	void begin(const Camera& camera);
	void end();

	void draw(const Text& text);
	void draw(const Sprite& sprite);
	void draw(const Circle& circle);
	void draw(const Rectangle& rectangle);

	void drawLine(const Vector2f& start, const Vector2f& end, const Color& color);

	//void drawIndexed(Ref<VertexArray> vertexArray);
	//static void draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform = Matrix(1.0f));

private:
	void initLinesVertices();
	void initGlyphsVertices();
	void initCircleVertices();
	void initSpritesVertices();
	void initRectanglesVertices();

	void batchStart();
	void batchFlush();

	void batchStartLines();
	void batchStartGlyphs();
	void batchStartCircles();
	void batchStartSprites();
	void batchStartRectangles();

	void batchFlushLines();
	void batchFlushGlyphs();
	void batchFlushCircles();
	void batchFlushSprites();
	void batchFlushRectangles();

	static constexpr uint32_t MAX_LINES_COUNT = 500;
	static constexpr uint32_t MAX_LINES_VERTICES = MAX_LINES_COUNT * 2;

	static constexpr uint32_t MAX_CIRCLES_COUNT = 500;
	static constexpr uint32_t MAX_CIRCLES_INDICES = MAX_CIRCLES_COUNT * 6;
	static constexpr uint32_t MAX_CIRCLES_VERTICES = MAX_CIRCLES_COUNT * 4;

	static constexpr uint32_t MAX_SPRITES_COUNT = 500;
	static constexpr uint32_t MAX_SPRITES_INDICES = MAX_SPRITES_COUNT * 6;
	static constexpr uint32_t MAX_SPRITES_VERTICES = MAX_SPRITES_COUNT * 4;

	static constexpr uint32_t MAX_RECTANGLES_COUNT = 500;
	static constexpr uint32_t MAX_RECTANGLES_INDICES = MAX_RECTANGLES_COUNT * 6;
	static constexpr uint32_t MAX_RECTANGLES_VERTICES = MAX_RECTANGLES_COUNT * 4;

	static constexpr uint32_t MAX_GLYPHS_COUNT = 500;
	static constexpr uint32_t MAX_GLYPHS_INDICES = MAX_GLYPHS_COUNT * 6;
	static constexpr uint32_t MAX_GLYPHS_VERTICES = MAX_GLYPHS_COUNT * 4;

	static constexpr uint32_t MAX_TEXTURE_SLOTS = 32; //glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	static constexpr uint32_t EMPTY_TEXTURE_INDEX = 32;

	struct Lines
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<LineVertex, MAX_LINES_VERTICES> vertices;

		uint32_t verticesCount = 0;
	} m_lines;

	template<typename VertexType, uint32_t MAX_VERTICES>
	struct Shape
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<VertexType, MAX_VERTICES> vertices;
		std::unordered_map<RendererID, Ref<Texture>> textures;

		uint32_t verticesCount = 0;
		uint32_t texturesCount = 0;
	};

	Shape<CircleVertex, MAX_CIRCLES_VERTICES> m_circles;
	Shape<SpriteVertex, MAX_SPRITES_VERTICES> m_sprites;
	Shape<RectangleVertex, MAX_RECTANGLES_VERTICES> m_rectangles;
	Shape<GlyphVertex, MAX_GLYPHS_VERTICES> m_glyphs;

	uint32_t zIndex = 1;
	Matrix4 m_viewProjectionMatrix;
};

}
