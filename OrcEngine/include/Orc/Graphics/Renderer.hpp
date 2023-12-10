#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexArray.hpp"

#include "Graphics/Shapes/Circle.hpp"
#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

class Renderer
{
public:
	Renderer();

	void clear();
	void setClearColor(const Color& color);

	void begin(const Camera& camera);
	void end();

	void draw(Ref<Sprite> sprite);
	void draw(Ref<Circle> circle);
	void draw(Ref<Rectangle> rectangle);

	void drawIndexed(Ref<VertexArray> vertexArray);
	void drawLine(const Vector2f& start, const Vector2f& end, const Color& color);

	//static void draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform = Matrix(1.0f));

private:
	void initLinesVertices();
	void initCircleVertices();
	void initSpritesVertices();
	void initRectanglesVertices();

	void batchStart();
	void batchFlush();

	void batchStartLines();
	void batchStartCircles();
	void batchStartSprites();
	void batchStartRectangles();

	void batchFlushLines();
	void batchFlushCircles();
	void batchFlushSprites();
	void batchFlushRectangles();

	static constexpr uint32 MAX_LINES_COUNT = 5000u;
	static constexpr uint32 MAX_LINES_VERTICES = MAX_LINES_COUNT * 2;

	static constexpr uint32 MAX_CIRCLES_COUNT = 1000u;
	static constexpr uint32 MAX_CIRCLES_INDICES = MAX_CIRCLES_COUNT * 6;
	static constexpr uint32 MAX_CIRCLES_VERTICES = MAX_CIRCLES_COUNT * 4;

	static constexpr uint32 MAX_SPRITES_COUNT = 1000u;
	static constexpr uint32 MAX_SPRITES_INDICES = MAX_SPRITES_COUNT * 6;
	static constexpr uint32 MAX_SPRITES_VERTICES = MAX_SPRITES_COUNT * 4;

	static constexpr uint32 MAX_RECTANGLES_COUNT = 1000u;
	static constexpr uint32 MAX_RECTANGLES_INDICES = MAX_RECTANGLES_COUNT * 6;
	static constexpr uint32 MAX_RECTANGLES_VERTICES = MAX_RECTANGLES_COUNT * 4;

	static constexpr uint32 MAX_TEXTURE_SLOTS = 32u;
	static constexpr uint32 EMPTY_TEXTURE_INDEX = 32u;

	Matrix4 m_viewProjectionMatrix;

	struct Lines
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<LineVertex, MAX_LINES_VERTICES> vertices;

		uint32 verticesCount = 0u;
	} m_lines;

	struct Circles
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<CircleVertex, MAX_CIRCLES_VERTICES> vertices;
		std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> textures;

		uint32 verticesCount = 0u;
		uint32 texturesCount = 0u;
	} m_circles;

	struct Sprites
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<Vertex, MAX_SPRITES_VERTICES> vertices;
		std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> textures;

		uint32 verticesCount = 0u;
		uint32 texturesCount = 0u;
	} m_sprites;
		
	struct Rectangles
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		std::array<RectangleVertex, MAX_RECTANGLES_VERTICES> vertices;
		std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> textures;

		uint32 verticesCount = 0u;
		uint32 texturesCount = 0u;
	} m_rectangles;
};

}
