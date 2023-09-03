#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexArray.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

class Renderer
{
public:
	static void init();

	static void clear();
	static void setClearColor(const Color& color);

	static void begin(const Camera& camera);
	static void end();

	static void draw(Ref<Sprite> sprite);
	static void draw(Ref<Rectangle> rectangle);

	static void drawIndexed(Ref<VertexArray> vertexArray);
	static void drawLine(const Vector2f& start, const Vector2f& end, const Color& color);

	//static void draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform = Matrix(1.0f));

private:
	static void initLinesVertices();
	static void initSpritesVertices();
	static void initRectanglesVertices();

	static void batchStart();
	static void batchFlush();

	static void batchStartLines();
	static void batchStartSprites();
	static void batchStartRectangles();

	static void batchFlushLines();
	static void batchFlushSprites();
	static void batchFlushRectangles();

	static constexpr uint32 MAX_LINES_COUNT = 5000u;
	static constexpr uint32 MAX_LINES_VERTICES = MAX_LINES_COUNT * 2;

	static constexpr uint32 MAX_SPRITES_COUNT = 1000u;
	static constexpr uint32 MAX_SPRITES_INDICES = MAX_SPRITES_COUNT * 6;
	static constexpr uint32 MAX_SPRITES_VERTICES = MAX_SPRITES_COUNT * 4;

	static constexpr uint32 MAX_RECTANGLES_COUNT = 1000u;
	static constexpr uint32 MAX_RECTANGLES_INDICES = MAX_RECTANGLES_COUNT * 6;
	static constexpr uint32 MAX_RECTANGLES_VERTICES = MAX_RECTANGLES_COUNT * 4;

	static constexpr uint32 MAX_TEXTURE_SLOTS = 32u;
	static constexpr uint32 EMPTY_TEXTURE_INDEX = 32u;

	struct SceneData
	{
		Matrix4 viewProjectionMatrix;

		struct Lines
		{
			Ref<Shader> shader;
			Ref<VertexArray> vertexArray;
			Ref<VertexBuffer> vertexBuffer;
			std::array<LineVertex, MAX_LINES_VERTICES> vertices;

			uint32 verticesCount = 0u;
		} lines;

		struct Sprites
		{
			Ref<Shader> shader;
			Ref<VertexArray> vertexArray;
			Ref<VertexBuffer> vertexBuffer;
			std::array<Vertex, MAX_SPRITES_VERTICES> vertices;
			std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> textures;

			uint32 verticesCount = 0u;
			uint32 texturesCount = 0u;
		} sprites;
		
		struct Rectangles
		{
			Ref<Shader> shader;
			Ref<VertexArray> vertexArray;
			Ref<VertexBuffer> vertexBuffer;
			std::array<Vertex, MAX_RECTANGLES_VERTICES> vertices;
			std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> textures;

			uint32 verticesCount = 0u;
			uint32 texturesCount = 0u;
		} rectangles;
	};

	static SceneData* m_scene;
};

}
