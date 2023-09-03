#include "OrcPch.hpp"

#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::SceneData* Renderer::m_scene = new Renderer::SceneData;

void Renderer::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initLinesVertices();
	initSpritesVertices();
	initRectanglesVertices();
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(const Color& color)
{
	Vector4f normalizedColor = color.normalized();
	glClearColor(normalizedColor.x, normalizedColor.y, normalizedColor.z, normalizedColor.w);
}

void Renderer::begin(const Camera& camera)
{
	m_scene->viewProjectionMatrix = camera.getViewProjectionMatrix();

	batchStart();
}

void Renderer::end()
{
	batchFlush();
}

void Renderer::draw(Ref<Sprite> sprite)
{
	Renderer::SceneData::Sprites& sprites = m_scene->sprites;

	if (sprites.texturesCount + 1u >= MAX_TEXTURE_SLOTS)
	{
		ORC_LOG_INFO("Maximum amount of available textures used, sprite batch flushing");
		batchFlushSprites();
		batchStartSprites();
	}
	else if (sprites.verticesCount + 4u >= MAX_SPRITES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, sprite batch flushing");
		batchFlushSprites();
		batchStartSprites();
	}

	sprites.textures[sprites.texturesCount] = sprite->getTexture();
	std::array<Vertex, 4u> spriteVertices = sprite->getVertices();
	for (uint64 i = 0u; i < 4u; i++)
	{
		sprites.vertices[(uint64)sprites.verticesCount + i] = spriteVertices[i];
		sprites.vertices[(uint64)sprites.verticesCount + i].textureIndex = (float)sprites.texturesCount;
	}

	sprites.texturesCount += 1u;
	sprites.verticesCount += 4u;
}

void Renderer::draw(Ref<Rectangle> rectangle)
{
	Renderer::SceneData::Rectangles& rectangles = m_scene->rectangles;

	if (rectangle->isFilled())
	{
		if (rectangles.verticesCount + 4u >= MAX_RECTANGLES_VERTICES)
		{
			ORC_LOG_INFO("Maximum amount of available vertices used, rectangle batch flushing");
			batchFlushRectangles();
			batchStartRectangles();
		}

		if (rectangle->getTexture())
		{
			if (rectangles.texturesCount + 1u >= MAX_TEXTURE_SLOTS)
			{
				ORC_LOG_INFO("Maximum amount of available textures used, rectangle batch flushing");
				batchFlushRectangles();
				batchStartRectangles();
			}

			rectangles.textures[rectangles.texturesCount] = rectangle->getTexture();
			std::array<Vertex, 4u> rectangleVertices = rectangle->getVertices();
			for (uint64 i = 0u; i < 4u; i++)
			{
				rectangles.vertices[(uint64)rectangles.verticesCount + i] = rectangleVertices[i];
				rectangles.vertices[(uint64)rectangles.verticesCount + i].textureIndex = (float)rectangles.texturesCount;
			}

			rectangles.texturesCount += 1u;
			rectangles.verticesCount += 4u;
		}
		else
		{
			std::array<Vertex, 4u> rectangleVertices = rectangle->getVertices();
			for (uint64 i = 0u; i < 4u; i++)
			{
				rectangles.vertices[(uint64)rectangles.verticesCount + i] = rectangleVertices[i];
				rectangles.vertices[(uint64)rectangles.verticesCount + i].textureIndex = EMPTY_TEXTURE_INDEX;
			}

			rectangles.verticesCount += 4u;
		}
	}
	else
	{
		Color color = rectangle->getColor();
		Vector2f start = rectangle->getPosition();
		Vector2f end = start + rectangle->getSize();

		drawLine(start, Vector2f(end.x, start.y), color);
		drawLine(Vector2f(end.x, start.y), end, color);
		drawLine(end, Vector2f(start.x, end.y), color);
		drawLine(start, Vector2f(start.x, end.y), color);
	}
}

void Renderer::drawIndexed(Ref<VertexArray> vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color)
{
	Renderer::SceneData::Lines& lines = m_scene->lines;

	if (lines.verticesCount >= MAX_LINES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, lines batch flushing");
		batchFlushLines();
		batchStartLines();
	}

	lines.vertices[(uint64)lines.verticesCount + 0u] = LineVertex{ .color = color.normalized(), .position = start };
	lines.vertices[(uint64)lines.verticesCount + 1u] = LineVertex{ .color = color.normalized(), .position = end   };

	lines.verticesCount += 2u;
}

//void Renderer::draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform)
//{
//	shader->bind();
//	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
//	shader->uploadUniformMatrix3("u_transform", transform);
//
//	glBindVertexArray(vertexArray->getRendererID());
//
//	Renderer::drawIndexed(vertexArray);
//}

void Renderer::initLinesVertices()
{
	Renderer::SceneData::Lines& lines = m_scene->lines;

	lines.shader = createRef<Shader>("shaders/line.vert.glsl", "shaders/line.frag.glsl");
	lines.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32)(sizeof(LineVertex) * MAX_LINES_VERTICES));
	lines.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" }
	});

	lines.vertexArray = createRef<VertexArray>();
	lines.vertexArray->addVertexBuffer(lines.vertexBuffer);

	lines.verticesCount = 0u;
}

void Renderer::initSpritesVertices()
{
	Renderer::SceneData::Sprites& sprites = m_scene->sprites;

	sprites.shader = createRef<Shader>("shaders/sprite.vert.glsl", "shaders/sprite.frag.glsl");
	sprites.shader->bind();
	int32 samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32 i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;
	sprites.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	sprites.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32)(sizeof(Vertex) * MAX_SPRITES_VERTICES));
	sprites.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" }
		});

	std::vector<uint32> spriteIndices(MAX_SPRITES_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0u;
	for (uint64_t i = 0u; i < MAX_SPRITES_INDICES; i += 6u)
	{
		spriteIndices[i + 0u] = offset + 0u;
		spriteIndices[i + 1u] = offset + 1u;
		spriteIndices[i + 2u] = offset + 2u;
		spriteIndices[i + 3u] = offset + 2u;
		spriteIndices[i + 4u] = offset + 1u;
		spriteIndices[i + 5u] = offset + 3u;

		offset += 4u;
	}

	Ref<IndexBuffer> spriteIndicesBuffer = createRef<IndexBuffer>(spriteIndices.data(), MAX_SPRITES_INDICES);

	sprites.vertexArray = createRef<VertexArray>();
	sprites.vertexArray->addVertexBuffer(sprites.vertexBuffer);
	sprites.vertexArray->setIndexBuffer(spriteIndicesBuffer);

	sprites.verticesCount = 0u;
	sprites.texturesCount = 0u;
}

void Renderer::initRectanglesVertices()
{
	SceneData::Rectangles& rectangles = m_scene->rectangles;

	rectangles.shader = createRef<Shader>("shaders/rectangle.vert.glsl", "shaders/rectangle.frag.glsl");
	rectangles.shader->bind();
	int32 samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32 i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;
	rectangles.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	rectangles.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32(sizeof(Vertex) * MAX_RECTANGLES_INDICES)));
	rectangles.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" }
	});

	std::vector<uint32> rectangleIndices(MAX_RECTANGLES_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0u;
	for (uint64_t i = 0u; i < MAX_RECTANGLES_INDICES; i += 6u)
	{
		rectangleIndices[i + 0u] = offset + 0u;
		rectangleIndices[i + 1u] = offset + 1u;
		rectangleIndices[i + 2u] = offset + 2u;
		rectangleIndices[i + 3u] = offset + 2u;
		rectangleIndices[i + 4u] = offset + 1u;
		rectangleIndices[i + 5u] = offset + 3u;

		offset += 4u;
	}

	Ref<IndexBuffer> quadIndicesBuffer = createRef<IndexBuffer>(rectangleIndices.data(), MAX_RECTANGLES_INDICES);

	rectangles.vertexArray = createRef<VertexArray>();
	rectangles.vertexArray->addVertexBuffer(rectangles.vertexBuffer);
	rectangles.vertexArray->setIndexBuffer(quadIndicesBuffer);

	rectangles.verticesCount = 0u;
	rectangles.texturesCount = 0u;
}

void Renderer::batchStart()
{
	batchStartLines();
	batchStartSprites();
	batchStartRectangles();
}

void Renderer::batchFlush()
{
	batchFlushLines();
	batchFlushSprites();
	batchFlushRectangles();
}

void Renderer::batchStartLines()
{
	m_scene->lines.verticesCount = 0u;
}

void Renderer::batchStartSprites()
{
	m_scene->sprites.verticesCount = 0u;
	m_scene->sprites.texturesCount = 0u;
}

void Renderer::batchStartRectangles()
{
	m_scene->rectangles.verticesCount = 0u;
	m_scene->rectangles.texturesCount = 0u;
}

void Renderer::batchFlushLines()
{
	Renderer::SceneData::Lines& lines = m_scene->lines;
	if (lines.verticesCount)
	{
		lines.shader->bind();
		lines.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		lines.vertexBuffer->setData((void*)(lines.vertices.data()), (uint32)(lines.verticesCount * sizeof(LineVertex)));

		glBindVertexArray(lines.vertexArray->getRendererID());
		glDrawArrays(GL_LINES, 0, (int32)lines.verticesCount);
	}
}

void Renderer::batchFlushSprites()
{
	Renderer::SceneData::Sprites& sprites = m_scene->sprites;
	if (sprites.verticesCount)
	{
		for (uint32 i = 0u; i < sprites.texturesCount; i++)
			sprites.textures[i]->bind(i);

		sprites.shader->bind();
		sprites.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		sprites.vertexBuffer->setData((void*)(sprites.vertices.data()), (uint32)(sprites.verticesCount * sizeof(Vertex)));

		glBindVertexArray(sprites.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, (int32)sprites.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::batchFlushRectangles()
{
	Renderer::SceneData::Rectangles& rectangles = m_scene->rectangles;
	if (rectangles.verticesCount)
	{
		for (uint32 i = 0u; i < rectangles.texturesCount; i++)
			rectangles.textures[i]->bind(i);

		rectangles.shader->bind();
		rectangles.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		rectangles.vertexBuffer->setData((void*)(rectangles.vertices.data()), (uint32)(rectangles.verticesCount * sizeof(Vertex)));

		glBindVertexArray(rectangles.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, rectangles.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

}
