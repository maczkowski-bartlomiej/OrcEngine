#include "OrcPch.hpp"

#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::Renderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initLinesVertices();
	initCircleVertices();
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
	m_viewProjectionMatrix = camera.getViewProjectionMatrix();

	batchStart();
}

void Renderer::end()
{
	batchFlush();
}

void Renderer::draw(Ref<Sprite> sprite)
{
	if (m_sprites.texturesCount + 1u >= MAX_TEXTURE_SLOTS)
	{
		ORC_LOG_INFO("Maximum amount of available textures used, sprite batch flushing");
		batchFlushSprites();
		batchStartSprites();
	}

	if (m_sprites.verticesCount + 4u >= MAX_SPRITES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, sprite batch flushing");
		batchFlushSprites();
		batchStartSprites();
	}

	m_sprites.textures[m_sprites.texturesCount] = sprite->getTexture();
	std::array<Vertex, 4u> spriteVertices = sprite->getVertices();
	for (uint64 i = 0u; i < 4u; i++)
	{
		m_sprites.vertices[(uint64)m_sprites.verticesCount + i] = spriteVertices[i];
		m_sprites.vertices[(uint64)m_sprites.verticesCount + i].textureIndex = (float)m_sprites.texturesCount;
	}

	m_sprites.texturesCount += 1u;
	m_sprites.verticesCount += 4u;
}

void Renderer::draw(Ref<Circle> circle)
{
	if (m_circles.verticesCount + 4u >= MAX_CIRCLES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, circle batch flushing");
		batchFlushCircles();
		batchStartCircles();
	}

	if (circle->getTexture())
	{
		if (m_circles.texturesCount + 1u >= MAX_TEXTURE_SLOTS)
		{
			ORC_LOG_INFO("Maximum amount of available textures used, circle batch flushing");
			batchFlushCircles();
			batchStartCircles();
		}

		m_circles.textures[m_circles.texturesCount] = circle->getTexture();
		std::array<CircleVertex, 4u> circleVertices = circle->getVertices();
		for (uint64 i = 0u; i < 4u; i++)
		{
			m_circles.vertices[(uint64)m_circles.verticesCount + i] = circleVertices[i];
			m_circles.vertices[(uint64)m_circles.verticesCount + i].textureIndex = (float)m_circles.texturesCount;
		}

		m_circles.texturesCount += 1u;
		m_circles.verticesCount += 4u;
	}
	else
	{
		std::array<CircleVertex, 4u> circleVertices = circle->getVertices();
		for (uint64 i = 0u; i < 4u; i++)
		{
			m_circles.vertices[(uint64)m_circles.verticesCount + i] = circleVertices[i];
			m_circles.vertices[(uint64)m_circles.verticesCount + i].textureIndex = EMPTY_TEXTURE_INDEX;
		}

		m_circles.verticesCount += 4u;
	}
}

void Renderer::draw(Ref<Rectangle> rectangle)
{
	if (m_rectangles.verticesCount + 4u >= MAX_RECTANGLES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, rectangle batch flushing");
		batchFlushRectangles();
		batchStartRectangles();
	}

	if (rectangle->getTexture())
	{
		if (m_rectangles.texturesCount + 1u >= MAX_TEXTURE_SLOTS)
		{
			ORC_LOG_INFO("Maximum amount of available textures used, rectangle batch flushing");
			batchFlushRectangles();
			batchStartRectangles();
		}

		m_rectangles.textures[m_rectangles.texturesCount] = rectangle->getTexture();
		std::array<RectangleVertex, 4u> rectangleVertices = rectangle->getVertices();
		for (uint64 i = 0u; i < 4u; i++)
		{
			m_rectangles.vertices[(uint64)m_rectangles.verticesCount + i] = rectangleVertices[i];
			m_rectangles.vertices[(uint64)m_rectangles.verticesCount + i].textureIndex = (float)m_rectangles.texturesCount;
		}

		m_rectangles.texturesCount += 1u;
		m_rectangles.verticesCount += 4u;
	}
	else
	{
		std::array<RectangleVertex, 4u> rectangleVertices = rectangle->getVertices();
		for (uint64 i = 0u; i < 4u; i++)
		{
			m_rectangles.vertices[(uint64)m_rectangles.verticesCount + i] = rectangleVertices[i];
			m_rectangles.vertices[(uint64)m_rectangles.verticesCount + i].textureIndex = EMPTY_TEXTURE_INDEX;
		}
		m_rectangles.verticesCount += 4u;
	}
}

void Renderer::drawIndexed(Ref<VertexArray> vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color)
{
	if (m_lines.verticesCount >= MAX_LINES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, lines batch flushing");
		batchFlushLines();
		batchStartLines();
	}

	m_lines.vertices[(uint64)m_lines.verticesCount + 0u] = LineVertex{ .color = color.normalized(), .position = start };
	m_lines.vertices[(uint64)m_lines.verticesCount + 1u] = LineVertex{ .color = color.normalized(), .position = end   };

	m_lines.verticesCount += 2u;
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
	m_lines.shader = createRef<Shader>("shaders/line.vert.glsl", "shaders/line.frag.glsl");
	m_lines.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32)(sizeof(LineVertex) * MAX_LINES_VERTICES));
	m_lines.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" }
	});

	m_lines.vertexArray = createRef<VertexArray>();
	m_lines.vertexArray->addVertexBuffer(m_lines.vertexBuffer);

	m_lines.verticesCount = 0u;
}

void Renderer::initCircleVertices()
{
	m_circles.shader = createRef<Shader>("shaders/circle.vert.glsl", "shaders/circle.frag.glsl");
	m_circles.shader->bind();

	int32 samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32 i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;

	m_circles.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_circles.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32)(sizeof(Vertex) * MAX_CIRCLES_VERTICES));

	m_circles.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_fillColor" },
		{ BufferLayout::ShaderDataType::Float4, "a_borderColor" },
		{ BufferLayout::ShaderDataType::Float2, "a_localPosition" },
		{ BufferLayout::ShaderDataType::Float2, "a_globalPosition" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" },
		{ BufferLayout::ShaderDataType::Float,  "a_radius" },
		{ BufferLayout::ShaderDataType::Float,  "a_thickness" },
		{ BufferLayout::ShaderDataType::Float,  "a_borderThickness" }
	});

	std::vector<uint32> circleIndices(MAX_CIRCLES_INDICES);
	uint32_t offset = 0u;
	for (uint64_t i = 0u; i < MAX_CIRCLES_INDICES; i += 6u)
	{
		circleIndices[i + 0u] = offset + 0u;
		circleIndices[i + 1u] = offset + 1u;
		circleIndices[i + 2u] = offset + 2u;
		circleIndices[i + 3u] = offset + 2u;
		circleIndices[i + 4u] = offset + 1u;
		circleIndices[i + 5u] = offset + 3u;

		offset += 4u;
	}

	Ref<IndexBuffer> circleIndicesBuffer = createRef<IndexBuffer>(circleIndices.data(), MAX_CIRCLES_INDICES);

	m_circles.vertexArray = createRef<VertexArray>();
	m_circles.vertexArray->addVertexBuffer(m_circles.vertexBuffer);
	m_circles.vertexArray->setIndexBuffer(circleIndicesBuffer);

	m_circles.verticesCount = 0u;
	m_circles.texturesCount = 0u;
}

void Renderer::initSpritesVertices()
{
	m_sprites.shader = createRef<Shader>("shaders/sprite.vert.glsl", "shaders/sprite.frag.glsl");
	m_sprites.shader->bind();

	int32 samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32 i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;

	m_sprites.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_sprites.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32)(sizeof(Vertex) * MAX_SPRITES_VERTICES));
	m_sprites.vertexBuffer->setLayout(BufferLayout{
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

	m_sprites.vertexArray = createRef<VertexArray>();
	m_sprites.vertexArray->addVertexBuffer(m_sprites.vertexBuffer);
	m_sprites.vertexArray->setIndexBuffer(spriteIndicesBuffer);

	m_sprites.verticesCount = 0u;
	m_sprites.texturesCount = 0u;
}

void Renderer::initRectanglesVertices()
{
	m_rectangles.shader = createRef<Shader>("shaders/rectangle.vert.glsl", "shaders/rectangle.frag.glsl");
	m_rectangles.shader->bind();
	int32 samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32 i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;
	m_rectangles.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_rectangles.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32(sizeof(Vertex) * MAX_RECTANGLES_INDICES)));
	m_rectangles.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_fillColor" },
		{ BufferLayout::ShaderDataType::Float4, "a_borderColor" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" },
		{ BufferLayout::ShaderDataType::Float,  "a_borderThickness" }
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

	m_rectangles.vertexArray = createRef<VertexArray>();
	m_rectangles.vertexArray->addVertexBuffer(m_rectangles.vertexBuffer);
	m_rectangles.vertexArray->setIndexBuffer(quadIndicesBuffer);

	m_rectangles.verticesCount = 0u;
	m_rectangles.texturesCount = 0u;
}

void Renderer::batchStart()
{
	batchStartLines();
	batchStartCircles();
	batchStartSprites();
	batchStartRectangles();
}

void Renderer::batchFlush()
{
	batchFlushLines();
	batchFlushCircles();
	batchFlushSprites();
	batchFlushRectangles();
}

void Renderer::batchStartLines()
{
	m_lines.verticesCount = 0u;
}

void Renderer::batchStartCircles()
{
	m_circles.verticesCount = 0u;
	m_circles.texturesCount = 0u;
}

void Renderer::batchStartSprites()
{
	m_sprites.verticesCount = 0u;
	m_sprites.texturesCount = 0u;
}

void Renderer::batchStartRectangles()
{
	m_rectangles.verticesCount = 0u;
	m_rectangles.texturesCount = 0u;
}

void Renderer::batchFlushLines()
{
	if (m_lines.verticesCount)
	{
		m_lines.shader->bind();
		m_lines.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_lines.vertexBuffer->setData((void*)(m_lines.vertices.data()), (uint32)(m_lines.verticesCount * sizeof(LineVertex)));

		glBindVertexArray(m_lines.vertexArray->getRendererID());
		glDrawArrays(GL_LINES, 0, (int32)m_lines.verticesCount);
	}
}

void Renderer::batchFlushCircles()
{
	if (m_circles.verticesCount)
	{
		for (uint32 i = 0u; i < m_circles.texturesCount; i++)
			m_circles.textures[i]->bind(i);

		m_circles.shader->bind();
		m_circles.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_circles.vertexBuffer->setData((void*)(m_circles.vertices.data()), (uint32)(m_circles.verticesCount * sizeof(CircleVertex)));

		glBindVertexArray(m_circles.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, (int32)m_circles.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::batchFlushSprites()
{
	if (m_sprites.verticesCount)
	{
		for (uint32 i = 0u; i < m_sprites.texturesCount; i++)
			m_sprites.textures[i]->bind(i);

		m_sprites.shader->bind();
		m_sprites.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_sprites.vertexBuffer->setData((void*)(m_sprites.vertices.data()), (uint32)(m_sprites.verticesCount * sizeof(Vertex)));

		glBindVertexArray(m_sprites.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, (int32)m_sprites.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::batchFlushRectangles()
{
	if (m_rectangles.verticesCount)
	{
		for (uint32 i = 0u; i < m_rectangles.texturesCount; i++)
			m_rectangles.textures[i]->bind(i);

		m_rectangles.shader->bind();
		m_rectangles.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_rectangles.vertexBuffer->setData((void*)(m_rectangles.vertices.data()), (uint32)(m_rectangles.verticesCount * sizeof(RectangleVertex)));

		glBindVertexArray(m_rectangles.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, m_rectangles.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

}
