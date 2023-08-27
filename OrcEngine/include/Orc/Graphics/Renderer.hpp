#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Vertices.hpp"
#include "Graphics/VertexArray.hpp"

#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

class Renderer
{
	static constexpr uint32 MAX_QUAD_AMOUNT = 1000u;
	static constexpr uint32 MAX_QUAD_INDICES = MAX_QUAD_AMOUNT * 6;
	static constexpr uint32 MAX_QUAD_VERTICES = MAX_QUAD_AMOUNT * 4;

	static constexpr uint32 MAX_LINE_AMOUNT = 5000u;
	static constexpr uint32 MAX_LINE_VERTICES = MAX_LINE_AMOUNT * 2;

	static constexpr uint32 MAX_SPRITE_AMOUNT = 1000u;
	static constexpr uint32 MAX_SPRITE_INDICES = MAX_SPRITE_AMOUNT * 6;
	static constexpr uint32 MAX_SPRITE_VERTICES = MAX_SPRITE_AMOUNT * 4;
	static constexpr uint32 MAX_SPRITE_TEXTURE_SLOTS = 32u;
	static constexpr uint32 MAX_SPRITE_TEXTURE_SLOTS_SHADER = 64u;

public:
	static void init();

	static void clear();
	static void setClearColor(const Vector4f& color);

	static void begin(const Camera& camera);
	static void end();

	static void drawIndexed(Reference<VertexArray> vertexArray);

	static void draw(Reference<Sprite> sprite);
	static void draw(Reference<Rectangle> rectangle);

	static void drawLine(const Vector2f& start, const Vector2f& end, const Vector4f& color);

	static void draw(Reference<VertexArray> vertexArray, Reference<Shader> shader, const Matrix& transform = Matrix(1.0f));

private:
	static void batchStart();
	static void batchFlush();

	static void initQuadVertices();
	static void initLinesVertices();
	static void initSpriteVertices();

	struct SceneData
	{
		Matrix viewProjectionMatrix;

		//QUADS
		Reference<Shader> quadShader;
		Reference<VertexArray> quadVertexArray;
		Reference<VertexBuffer> quadVertexBuffer;

		std::array<Vertex, MAX_QUAD_VERTICES> quadVertices;

		uint32 quadVerticesCount = 0u;
		//QUADS

		//LINES
		Reference<Shader> lineShader;
		Reference<VertexArray> lineVertexArray;
		Reference<VertexBuffer> lineVertexBuffer;

		std::array<Vertex, MAX_LINE_VERTICES> lineVertices;

		uint32 lineVerticesCount = 0u;
		//LINES

		//SPRITES
		Reference<Shader> spriteTextureShader;
		Reference<VertexArray> spriteVertexArray;
		Reference<VertexBuffer> spriteVertexBuffer;

		std::array<TexturedVertex, MAX_SPRITE_VERTICES> spriteVertices;
		std::array<Reference<Texture>, MAX_SPRITE_TEXTURE_SLOTS> spriteTextures;

		uint32 spriteTexturesCount = 0u;
		uint32 spriteVerticesCount = 0u;
		//SPRITES
	};

	static SceneData* m_scene;
};

}
