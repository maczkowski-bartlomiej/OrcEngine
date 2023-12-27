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
	if (!sprite->getTexture())
		return;

	if (m_sprites.texturesCount + 1 >= MAX_TEXTURE_SLOTS)
	{
		ORC_LOG_INFO("Maximum amount of available textures used, batch flushing");
		batchFlush();
		batchStart();
	}

	if (m_sprites.verticesCount + 4 >= MAX_SPRITES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, batch flushing");
		batchFlush();
		batchStart();
	}

	m_sprites.textures[m_sprites.texturesCount] = sprite->getTexture();
	std::array<SpriteVertex, 4> spriteVertices = sprite->getVertices();
	for (uint64_t i = 0; i < 4; i++)
	{
		m_sprites.vertices[(uint64_t)m_sprites.verticesCount + i] = spriteVertices[i];
		m_sprites.vertices[(uint64_t)m_sprites.verticesCount + i].textureIndex = (float)m_sprites.texturesCount;
	}

	m_sprites.texturesCount += 1;
	m_sprites.verticesCount += 4;
}

void Renderer::draw(Ref<Circle> circle)
{
	if (m_circles.verticesCount + 4 >= MAX_CIRCLES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, batch flushing");
		batchFlush();
		batchStart();
	}

	float textureIndex = EMPTY_TEXTURE_INDEX;
	if (circle->getTexture())
	{
		if (m_circles.texturesCount + 1 >= MAX_TEXTURE_SLOTS)
		{
			ORC_LOG_INFO("Maximum amount of available textures used, batch flushing");
			batchFlush();
			batchStart();
		}

		textureIndex = (float)m_circles.texturesCount;
		m_circles.textures[m_circles.texturesCount] = circle->getTexture();

		m_circles.texturesCount += 1;
	}

	std::array<CircleVertex, 4> circleVertices = circle->getVertices();
	for (uint64_t i = 0; i < 4; i++)
	{
		m_circles.vertices[(uint64_t)m_circles.verticesCount + i] = circleVertices[i];
		m_circles.vertices[(uint64_t)m_circles.verticesCount + i].textureIndex = textureIndex;
	}

	m_circles.verticesCount += 4;
}

void Renderer::draw(Ref<Rectangle> rectangle)
{
	if (m_rectangles.verticesCount + 4 >= MAX_RECTANGLES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, batch flushing");
		batchFlush();
		batchStart();
	}

	float textureIndex = EMPTY_TEXTURE_INDEX;
	if (rectangle->getTexture())
	{
		if (m_rectangles.texturesCount + 1 >= MAX_TEXTURE_SLOTS)
		{
			ORC_LOG_INFO("Maximum amount of available textures used, batch flushing");
			batchFlush();
			batchStart();
		}

		textureIndex = (float)m_rectangles.texturesCount;
		m_rectangles.textures[m_rectangles.texturesCount] = rectangle->getTexture();

		m_rectangles.texturesCount += 1;
	}

	std::array<RectangleVertex, 4> rectangleVertices = rectangle->getVertices();
	for (uint64_t i = 0; i < 4; i++)
	{
		m_rectangles.vertices[(uint64_t)m_rectangles.verticesCount + i] = rectangleVertices[i];
		m_rectangles.vertices[(uint64_t)m_rectangles.verticesCount + i].textureIndex = textureIndex;
	}

	m_rectangles.verticesCount += 4;
}

void Renderer::drawIndexed(Ref<VertexArray> vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color)
{
	if (m_lines.verticesCount >= MAX_LINES_VERTICES)
	{
		ORC_LOG_INFO("Maximum amount of available vertices used, batch flushing");
		batchFlush();
		batchStart();
	}

	m_lines.vertices[(size_t)m_lines.verticesCount + 0] = LineVertex{ .color = color.normalized(), .position = start };
	m_lines.vertices[(size_t)m_lines.verticesCount + 1] = LineVertex{ .color = color.normalized(), .position = end   };

	m_lines.verticesCount += 2;
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
	m_lines.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32_t)(sizeof(LineVertex) * MAX_LINES_VERTICES));
	m_lines.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" }
	});

	m_lines.vertexArray = createRef<VertexArray>();
	m_lines.vertexArray->addVertexBuffer(m_lines.vertexBuffer);

	m_lines.verticesCount = 0;
}

void Renderer::initCircleVertices()
{
	m_circles.shader = createRef<Shader>("shaders/circle.vert.glsl", "shaders/circle.frag.glsl");
	m_circles.shader->bind();

	int32_t samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;

	m_circles.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_circles.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32_t)(sizeof(CircleVertex) * MAX_CIRCLES_VERTICES));

	m_circles.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_fillColor" },
		{ BufferLayout::ShaderDataType::Float4, "a_borderColor" },
		{ BufferLayout::ShaderDataType::Float2, "a_localPosition" },
		{ BufferLayout::ShaderDataType::Float2, "a_globalPosition" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" },
		{ BufferLayout::ShaderDataType::Float,  "a_radius" },
		{ BufferLayout::ShaderDataType::Float,  "a_innerRadius" },
		{ BufferLayout::ShaderDataType::Float,  "a_borderThickness" }
	});

	std::vector<uint32_t> circleIndices(MAX_CIRCLES_INDICES);
	uint32_t offset = 0;
	for (uint64_t i = 0; i < MAX_CIRCLES_INDICES; i += 6)
	{
		circleIndices[i + 0] = offset + 0;
		circleIndices[i + 1] = offset + 1;
		circleIndices[i + 2] = offset + 2;
		circleIndices[i + 3] = offset + 2;
		circleIndices[i + 4] = offset + 1;
		circleIndices[i + 5] = offset + 3;

		offset += 4;
	}

	Ref<IndexBuffer> circleIndicesBuffer = createRef<IndexBuffer>(circleIndices.data(), MAX_CIRCLES_INDICES);

	m_circles.vertexArray = createRef<VertexArray>();
	m_circles.vertexArray->addVertexBuffer(m_circles.vertexBuffer);
	m_circles.vertexArray->setIndexBuffer(circleIndicesBuffer);

	m_circles.verticesCount = 0;
	m_circles.texturesCount = 0;
}

void Renderer::initSpritesVertices()
{
	m_sprites.shader = createRef<Shader>("shaders/sprite.vert.glsl", "shaders/sprite.frag.glsl");
	m_sprites.shader->bind();

	int32_t samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;

	m_sprites.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_sprites.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32_t)(sizeof(SpriteVertex) * MAX_SPRITES_VERTICES));
	m_sprites.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" }
		});

	std::vector<uint32_t> spriteIndices(MAX_SPRITES_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0;
	for (uint64_t i = 0; i < MAX_SPRITES_INDICES; i += 6)
	{
		spriteIndices[i + 0] = offset + 0;
		spriteIndices[i + 1] = offset + 1;
		spriteIndices[i + 2] = offset + 2;
		spriteIndices[i + 3] = offset + 2;
		spriteIndices[i + 4] = offset + 1;
		spriteIndices[i + 5] = offset + 3;

		offset += 4;
	}

	Ref<IndexBuffer> spriteIndicesBuffer = createRef<IndexBuffer>(spriteIndices.data(), MAX_SPRITES_INDICES);

	m_sprites.vertexArray = createRef<VertexArray>();
	m_sprites.vertexArray->addVertexBuffer(m_sprites.vertexBuffer);
	m_sprites.vertexArray->setIndexBuffer(spriteIndicesBuffer);

	m_sprites.verticesCount = 0;
	m_sprites.texturesCount = 0;
}

void Renderer::initRectanglesVertices()
{
	m_rectangles.shader = createRef<Shader>("shaders/rectangle.vert.glsl", "shaders/rectangle.frag.glsl");
	m_rectangles.shader->bind();
	int32_t samplers[MAX_TEXTURE_SLOTS] = {};
	for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;
	m_rectangles.shader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_rectangles.vertexBuffer = createRef<VertexBuffer>(nullptr, (int32_t(sizeof(RectangleVertex) * MAX_RECTANGLES_INDICES)));
	m_rectangles.vertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_fillColor" },
		{ BufferLayout::ShaderDataType::Float4, "a_borderColor" },
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" },
		{ BufferLayout::ShaderDataType::Float,  "a_borderThickness" }
	});	

	std::vector<uint32_t> rectangleIndices(MAX_RECTANGLES_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0;
	for (uint64_t i = 0; i < MAX_RECTANGLES_INDICES; i += 6)
	{
		rectangleIndices[i + 0] = offset + 0;
		rectangleIndices[i + 1] = offset + 1;
		rectangleIndices[i + 2] = offset + 2;
		rectangleIndices[i + 3] = offset + 2;
		rectangleIndices[i + 4] = offset + 1;
		rectangleIndices[i + 5] = offset + 3;

		offset += 4;
	}

	Ref<IndexBuffer> quadIndicesBuffer = createRef<IndexBuffer>(rectangleIndices.data(), MAX_RECTANGLES_INDICES);

	m_rectangles.vertexArray = createRef<VertexArray>();
	m_rectangles.vertexArray->addVertexBuffer(m_rectangles.vertexBuffer);
	m_rectangles.vertexArray->setIndexBuffer(quadIndicesBuffer);

	m_rectangles.verticesCount = 0;
	m_rectangles.texturesCount = 0;
}

void Renderer::batchStart()
{
	batchStartSprites();
	batchStartLines();
	batchStartCircles();
	batchStartRectangles();
}

void Renderer::batchFlush()
{
	batchFlushSprites();
	batchFlushLines();
	batchFlushCircles();
	batchFlushRectangles();
}

void Renderer::batchStartLines()
{
	m_lines.verticesCount = 0;
}

void Renderer::batchStartCircles()
{
	m_circles.verticesCount = 0;
	m_circles.texturesCount = 0;
}

void Renderer::batchStartSprites()
{
	m_sprites.verticesCount = 0;
	m_sprites.texturesCount = 0;
}

void Renderer::batchStartRectangles()
{
	m_rectangles.verticesCount = 0;
	m_rectangles.texturesCount = 0;
}

void Renderer::batchFlushLines()
{
	if (m_lines.verticesCount)
	{
		m_lines.shader->bind();
		m_lines.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_lines.vertexBuffer->setData((void*)(m_lines.vertices.data()), (uint32_t)(m_lines.verticesCount * sizeof(LineVertex)));

		glBindVertexArray(m_lines.vertexArray->getRendererID());
		glDrawArrays(GL_LINES, 0, (int32_t)m_lines.verticesCount);
	}
}

void Renderer::batchFlushCircles()
{
	if (m_circles.verticesCount)
	{
		for (uint32_t i = 0; i < m_circles.texturesCount; i++)
			m_circles.textures[i]->bind(i);

		m_circles.shader->bind();
		m_circles.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_circles.vertexBuffer->setData((void*)(m_circles.vertices.data()), (uint32_t)(m_circles.verticesCount * sizeof(CircleVertex)));

		glBindVertexArray(m_circles.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, (int32_t)m_circles.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::batchFlushSprites()
{
	if (m_sprites.verticesCount)
	{
		for (uint32_t i = 0; i < m_sprites.texturesCount; i++)
			m_sprites.textures[i]->bind(i);

		m_sprites.shader->bind();
		m_sprites.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_sprites.vertexBuffer->setData((void*)(m_sprites.vertices.data()), (uint32_t)(m_sprites.verticesCount * sizeof(SpriteVertex)));

		glBindVertexArray(m_sprites.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, (int32_t)m_sprites.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::batchFlushRectangles()
{
	if (m_rectangles.verticesCount)
	{
		for (uint32_t i = 0; i < m_rectangles.texturesCount; i++)
			m_rectangles.textures[i]->bind(i);

		m_rectangles.shader->bind();
		m_rectangles.shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
		m_rectangles.vertexBuffer->setData((void*)(m_rectangles.vertices.data()), (uint32_t)(m_rectangles.verticesCount * sizeof(RectangleVertex)));

		glBindVertexArray(m_rectangles.vertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, m_rectangles.vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

}
