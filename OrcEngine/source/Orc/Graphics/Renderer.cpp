#include "OrcPch.hpp"

#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::SceneData* Renderer::m_scene = new Renderer::SceneData;

void Renderer::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, 800, 600);

	initQuadVertices();
	initLinesVertices();
	initSpriteVertices();
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(const Vector4f& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::drawIndexed(Reference<VertexArray> vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
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

void Renderer::draw(Reference<Sprite> sprite)
{
	m_scene->spriteTextures[m_scene->spriteTexturesCount] = sprite->getTexture();
	std::array<TexturedVertex, 4u> spriteVertices = sprite->getVertices();
	for (uint32 i = 0u; i < 4u; i++)
	{
		m_scene->spriteVertices[m_scene->spriteVerticesCount + i] = spriteVertices[i];
		m_scene->spriteVertices[m_scene->spriteVerticesCount + i].textureIndex = (float)m_scene->spriteTexturesCount;
	}

	m_scene->spriteTexturesCount += 1u;
	m_scene->spriteVerticesCount += 4u;

	if (m_scene->spriteTexturesCount >= MAX_SPRITE_TEXTURE_SLOTS)
	{
		ORC_LOG_INFO("Amount of maximum available textures for drawing textures used, batch flushing");
		batchFlush();
		batchStart();
	}
	else if (m_scene->spriteVerticesCount >= MAX_SPRITE_VERTICES)
	{
		ORC_LOG_INFO("Amount of maximum available vertices for drawing sprites used, batch flushing");
		batchFlush();
		batchStart();
	}
}

void Renderer::draw(Reference<Rectangle> rectangle)
{
	if (rectangle->isFilled())
	{
		Vector4f color = { 0.0f, 1.0, 0.0f, 1.0f };
		Vector2f start = rectangle->getPosition();
		Vector2f end = start + rectangle->getSize();

		m_scene->quadVertices[0] = Vertex{ .position = { start.x, end.y }, .color = color };
		m_scene->quadVertices[1] = Vertex{ .position = { end.x, start.y }, .color = color };
		m_scene->quadVertices[2] = Vertex{ .position = start, .color = color };
		m_scene->quadVertices[3] = Vertex{ .position = end, .color = color };

		m_scene->quadVerticesCount += 4u;

		if (m_scene->quadVerticesCount >= MAX_QUAD_VERTICES)
		{
			ORC_LOG_INFO("Amount of maximum available vertices for drawing quads used, batch flushing");
			batchFlush();
			batchStart();
		}
	}
	else
	{
		Vector4f color = { 0.0f, 1.0, 0.0f, 1.0f };
		Vector2f start = rectangle->getPosition();
		Vector2f end = start + rectangle->getSize();

		drawLine(start, Vector2f(end.x, start.y), color);
		drawLine(Vector2f(end.x, start.y), end, color);
		drawLine(end, Vector2f(start.x, end.y), color);
		drawLine(start, Vector2f(start.x, end.y), color);
	}
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Vector4f& color)
{
	m_scene->lineVertices[m_scene->lineVerticesCount + 0] = Vertex{ .position = start, .color = color };
	m_scene->lineVertices[m_scene->lineVerticesCount + 1] = Vertex{ .position = end,   .color = color };

	m_scene->lineVerticesCount += 2u;

	if (m_scene->lineVerticesCount >= MAX_LINE_VERTICES)
	{
		ORC_LOG_INFO("Amount of maxium avaible vertices for drawing lines used, batch flusing");
		batchFlush();
		batchStart();
	}
}

void Renderer::draw(Reference<VertexArray> vertexArray, Reference<Shader> shader, const Matrix& transform)
{
	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
	shader->uploadUniformMatrix4("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());

	Renderer::drawIndexed(vertexArray);
}

void Renderer::batchStart()
{
	m_scene->spriteVerticesCount = 0u;
	m_scene->spriteTexturesCount = 0u;

	m_scene->lineVerticesCount = 0u;
}

void Renderer::batchFlush()
{
	if (m_scene->spriteVerticesCount)
	{
		for (uint32 i = 0u; i < m_scene->spriteTexturesCount; i++)
			m_scene->spriteTextures[i]->bind(i);

		m_scene->spriteTextureShader->bind();
		m_scene->spriteTextureShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		m_scene->spriteVertexBuffer->setData((void*)(m_scene->spriteVertices.data()), m_scene->spriteVerticesCount * sizeof(TexturedVertex));

		glBindVertexArray(m_scene->spriteVertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, m_scene->spriteVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	if (m_scene->lineVerticesCount)
	{
		m_scene->lineShader->bind();
		m_scene->lineShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		m_scene->lineVertexBuffer->setData((void*)(m_scene->lineVertices.data()), m_scene->lineVerticesCount * sizeof(Vertex));

		glBindVertexArray(m_scene->lineVertexArray->getRendererID());
		glDrawArrays(GL_LINES, 0, m_scene->lineVerticesCount);
	}

	if (m_scene->quadVerticesCount)
	{
		m_scene->quadShader->bind();
		m_scene->quadShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_scene->viewProjectionMatrix);
		m_scene->quadVertexBuffer->setData((void*)(m_scene->quadVertices.data()), m_scene->quadVerticesCount * sizeof(Vertex));

		glBindVertexArray(m_scene->quadVertexArray->getRendererID());
		glDrawElements(GL_TRIANGLES, m_scene->quadVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::initQuadVertices()
{
	m_scene->quadShader = createReference<Shader>("shaders/quad.vert.glsl", "shaders/quad.frag.glsl");
	m_scene->quadVertexBuffer = createReference<VertexBuffer>((void*)(m_scene->quadVertices.data()), (uint32)sizeof(Vertex) * MAX_QUAD_VERTICES);
	m_scene->quadVertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float4, "a_color" }
		});

	std::vector<uint32> quadIndices(MAX_SPRITE_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0u;
	for (uint64_t i = 0u; i < MAX_QUAD_INDICES; i += 6u)
	{
		quadIndices[i + 0u] = offset + 0u;
		quadIndices[i + 1u] = offset + 1u;
		quadIndices[i + 2u] = offset + 2u;
		quadIndices[i + 3u] = offset + 0u;
		quadIndices[i + 4u] = offset + 3u;
		quadIndices[i + 5u] = offset + 1u;

		offset += 4u;
	}

	Reference<IndexBuffer> quadIndicesBuffer = createReference<IndexBuffer>(quadIndices.data(), MAX_SPRITE_INDICES);

	m_scene->quadVertexArray = createReference<VertexArray>();
	m_scene->quadVertexArray->addVertexBuffer(m_scene->quadVertexBuffer);
	m_scene->quadVertexArray->setIndexBuffer(quadIndicesBuffer);

	m_scene->lineVerticesCount = 0u;
}

void Renderer::initLinesVertices()
{
	m_scene->lineShader = createReference<Shader>("shaders/line.vert.glsl", "shaders/line.frag.glsl");
	m_scene->lineVertexBuffer = createReference<VertexBuffer>((void*)(m_scene->lineVertices.data()), (uint32)sizeof(Vertex) * MAX_LINE_VERTICES);
	m_scene->lineVertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float4, "a_color" }
		});

	m_scene->lineVertexArray = createReference<VertexArray>();
	m_scene->lineVertexArray->addVertexBuffer(m_scene->lineVertexBuffer);

	m_scene->lineVerticesCount = 0u;
}

void Renderer::initSpriteVertices()
{
	m_scene->spriteTextureShader = createReference<Shader>("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	m_scene->spriteTextureShader->bind();
	int32 samplers[64]{};
	for (int32 i = 0; i < 64; i++)
		samplers[i] = i;
	m_scene->spriteTextureShader->uploadUniformIntArray("u_textures", samplers, 64u);

	m_scene->spriteVertexBuffer = createReference<VertexBuffer>((void*)(m_scene->spriteVertices.data()), (uint32)sizeof(TexturedVertex) * MAX_SPRITE_VERTICES);
	m_scene->spriteVertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float2, "a_position" },
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoords" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" }
		});

	std::vector<uint32> spriteIndices(MAX_SPRITE_INDICES); //Use std::vector instead of std::array to do not exeed stack memory limit
	uint32_t offset = 0u;
	for (uint64_t i = 0u; i < MAX_SPRITE_INDICES; i += 6u)
	{
		spriteIndices[i + 0u] = offset + 0u;
		spriteIndices[i + 1u] = offset + 1u;
		spriteIndices[i + 2u] = offset + 2u;
		spriteIndices[i + 3u] = offset + 0u;
		spriteIndices[i + 4u] = offset + 3u;
		spriteIndices[i + 5u] = offset + 1u;

		offset += 4u;
	}

	Reference<IndexBuffer> spriteIndicesBuffer = createReference<IndexBuffer>(spriteIndices.data(), MAX_SPRITE_INDICES);

	m_scene->spriteVertexArray = createReference<VertexArray>();
	m_scene->spriteVertexArray->addVertexBuffer(m_scene->spriteVertexBuffer);
	m_scene->spriteVertexArray->setIndexBuffer(spriteIndicesBuffer);

	m_scene->spriteTexturesCount = 0u;
	m_scene->spriteVerticesCount = 0u;
}

}
