#include "OrcPch.hpp"
#include "Graphics/Renderer2D.hpp"
#include "Engine/Debug.hpp"

#include <glad/glad.h>
#include <span>

namespace orc {

Renderer2D::Renderer2D()
{
	ORC_LOG_INFO("Initializing 2D renderer.");

	// Quad pipeline setup
	m_quadShader = createRef<Shader>("shaders/composite2d.vert.glsl", "shaders/composite2d.frag.glsl");
	m_quadShader->bind();

	int32_t samplers[MAX_TEXTURE_SLOTS] = {};
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = static_cast<int32_t>(i);

	m_quadShader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_quadVertexBuffer = createRef<VertexBuffer>(nullptr, static_cast<int32_t>(sizeof(QuadVertex) * MAX_QUAD_VERTICES));
	m_quadVertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float3, "a_position" },
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float4, "a_secondaryColor" },
		{ BufferLayout::ShaderDataType::Float2, "a_textureCoord" },
		{ BufferLayout::ShaderDataType::Float2, "a_localPosition" },
		{ BufferLayout::ShaderDataType::Float,  "a_textureIndex" },
		{ BufferLayout::ShaderDataType::Float,  "a_shapeType" },
		{ BufferLayout::ShaderDataType::Float,  "a_borderThickness" },
		{ BufferLayout::ShaderDataType::Float,  "a_innerRadius" },
		{ BufferLayout::ShaderDataType::Float,  "a_radius" }
	});

	std::vector<uint32_t> quadIndices(MAX_QUAD_INDICES);
	uint32_t offset = 0;
	for (uint32_t i = 0; i < MAX_QUAD_INDICES; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;
		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 1;
		quadIndices[i + 5] = offset + 3;

		offset += 4;
	}

	Ref<IndexBuffer> quadIndexBuffer = createRef<IndexBuffer>(quadIndices.data(), MAX_QUAD_INDICES);

	m_quadVertexArray = createRef<VertexArray>();
	m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);
	m_quadVertexArray->setIndexBuffer(quadIndexBuffer);

	// Line pipeline setup
	m_lineShader = createRef<Shader>("shaders/line.vert.glsl", "shaders/line.frag.glsl");
	m_lineVertexBuffer = createRef<VertexBuffer>(nullptr, static_cast<int32_t>(sizeof(LineVertex) * MAX_LINE_VERTICES));
	m_lineVertexBuffer->setLayout(BufferLayout{
		{ BufferLayout::ShaderDataType::Float4, "a_color" },
		{ BufferLayout::ShaderDataType::Float3, "a_position" }
	});

	m_lineVertexArray = createRef<VertexArray>();
	m_lineVertexArray->addVertexBuffer(m_lineVertexBuffer);

	startQuadBatch();
	startLineBatch();
}

void Renderer2D::beginScene(const Camera& camera)
{
	m_viewProjectionMatrix = camera.getViewProjectionMatrix();
	m_isSceneActive = true;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	startQuadBatch();
	startLineBatch();
}

void Renderer2D::endScene()
{
	flush();
	m_isSceneActive = false;
}

void Renderer2D::flush()
{
	flushLines();
	flushQuads();
}

void Renderer2D::startQuadBatch()
{
	m_quadVerticesCount = 0;
	m_textures.clear();
}

void Renderer2D::startLineBatch()
{
	m_lineVerticesCount = 0;
}

void Renderer2D::flushQuads()
{
	if (m_quadVerticesCount == 0)
		return;

	for (uint32_t s = 0; s < m_textures.size(); ++s)
		m_textures[s]->bind(s);

	m_quadShader->bind();
	m_quadShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	m_quadVertexBuffer->setData(std::span{ m_quadVertices.data(), m_quadVerticesCount });

	glBindVertexArray(m_quadVertexArray->getRendererID());
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((m_quadVerticesCount / 4) * 6), GL_UNSIGNED_INT, nullptr);

	startQuadBatch();
}

void Renderer2D::flushLines()
{
	if (m_lineVerticesCount == 0)
		return;

	m_lineShader->bind();
	m_lineShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	m_lineVertexBuffer->setData(std::span{ m_lineVertices.data(), m_lineVerticesCount });

	glBindVertexArray(m_lineVertexArray->getRendererID());
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_lineVerticesCount));

	startLineBatch();
}

float Renderer2D::getTextureSlot(Ref<Texture> texture)
{
	if (!texture)
		return EMPTY_TEXTURE_SLOT;

	for (size_t i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i]->getRendererID() == texture->getRendererID())
			return static_cast<float>(i);
	}

	if (m_textures.size() >= MAX_TEXTURE_SLOTS)
	{
		flushQuads();
	}

	float slot = static_cast<float>(m_textures.size());
	m_textures.push_back(texture);
	return slot;
}

void Renderer2D::draw(const Sprite& sprite)
{
	if (!sprite.getTexture())
		return;

	if (m_lineVerticesCount > 0)
		flushLines();

	float texSlot = getTextureSlot(sprite.getTexture());
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flushQuads();

	const auto& spriteVertices = sprite.getVertices();
	for (size_t i = 0; i < 4; ++i)
	{
		m_quadVertices[m_quadVerticesCount + i] = QuadVertex{
			.position = spriteVertices[i].position,
			.color = spriteVertices[i].color,
			.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			.textureCoord = spriteVertices[i].textureCoord,
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = texSlot,
			.shapeType = 0.0f,
			.borderThickness = 0.0f,
			.innerRadius = 0.0f,
			.radius = 0.0f
		};
	}
	m_quadVerticesCount += 4;
}

void Renderer2D::draw(const Rectangle& rectangle)
{
	if (m_lineVerticesCount > 0)
		flushLines();

	float texSlot = rectangle.getTexture() ? getTextureSlot(rectangle.getTexture()) : EMPTY_TEXTURE_SLOT;
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flushQuads();

	const auto& rectVertices = rectangle.getVertices();
	for (size_t i = 0; i < 4; ++i)
	{
		m_quadVertices[m_quadVerticesCount + i] = QuadVertex{
			.position = rectVertices[i].position,
			.color = rectVertices[i].fillColor,
			.secondaryColor = rectVertices[i].borderColor,
			.textureCoord = rectVertices[i].textureCoord,
			.localPosition = Vector2f(0.0f, 0.0f),
			.textureIndex = texSlot,
			.shapeType = 1.0f,
			.borderThickness = rectVertices[i].borderThickness,
			.innerRadius = 0.0f,
			.radius = 0.0f
		};
	}
	m_quadVerticesCount += 4;
}

void Renderer2D::draw(const Circle& circle)
{
	if (m_lineVerticesCount > 0)
		flushLines();

	float texSlot = circle.getTexture() ? getTextureSlot(circle.getTexture()) : EMPTY_TEXTURE_SLOT;
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flushQuads();

	const auto& circleVertices = circle.getVertices();
	for (size_t i = 0; i < 4; ++i)
	{
		m_quadVertices[m_quadVerticesCount + i] = QuadVertex{
			.position = circleVertices[i].globalPosition,
			.color = circleVertices[i].fillColor,
			.secondaryColor = circleVertices[i].borderColor,
			.textureCoord = circleVertices[i].textureCoord,
			.localPosition = circleVertices[i].localPosition,
			.textureIndex = texSlot,
			.shapeType = 2.0f,
			.borderThickness = circleVertices[i].borderThickness,
			.innerRadius = circleVertices[i].innerRadius,
			.radius = circleVertices[i].radius
		};
	}
	m_quadVerticesCount += 4;
}

void Renderer2D::draw(const Text& text)
{
	if (!text.getFont() || !text.getFont()->getBitmap())
		return;

	if (m_lineVerticesCount > 0)
		flushLines();

	float texSlot = getTextureSlot(text.getFont()->getBitmap());
	const auto& glyphVertices = text.getVertices();
	size_t count = glyphVertices.size();

	for (size_t i = 0; i < count; i += 4)
	{
		if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
			flushQuads();

		for (size_t j = 0; j < 4 && (i + j) < count; ++j)
		{
			m_quadVertices[m_quadVerticesCount + j] = QuadVertex{
				.position = glyphVertices[i + j].position,
				.color = glyphVertices[i + j].color,
				.secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
				.textureCoord = glyphVertices[i + j].textureCoord,
				.localPosition = Vector2f(0.0f, 0.0f),
				.textureIndex = texSlot,
				.shapeType = 3.0f,
				.borderThickness = 0.0f,
				.innerRadius = 0.0f,
				.radius = 0.0f
			};
		}
		m_quadVerticesCount += 4;
	}
}

void Renderer2D::drawQuad(const Vector3f& position, const Vector2f& size, const Color& color)
{
	Matrix transform = glm::translate(Matrix(1.0f), Vector2f(position.x, position.y));
	drawQuad(transform, size, color);
}

void Renderer2D::drawQuad(const Vector3f& position, const Vector2f& size, Ref<Texture> texture, const Color& tintColor)
{
	Matrix transform = glm::translate(Matrix(1.0f), Vector2f(position.x, position.y));
	drawQuad(transform, size, texture, tintColor);
}

void Renderer2D::drawQuad(const Matrix& transform, const Vector2f& size, const Color& color)
{
	drawQuad(transform, size, nullptr, color);
}

void Renderer2D::drawQuad(const Matrix& transform, const Vector2f& size, Ref<Texture> texture, const Color& tintColor)
{
	if (m_lineVerticesCount > 0)
		flushLines();

	float texSlot = texture ? getTextureSlot(texture) : EMPTY_TEXTURE_SLOT;
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flushQuads();

	Vector4f normColor = tintColor.normalized();
	Vector3f v0 = transform * Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f v1 = transform * Vector3f(size.x, 0.0f, 1.0f);
	Vector3f v2 = transform * Vector3f(0.0f, size.y, 1.0f);
	Vector3f v3 = transform * Vector3f(size.x, size.y, 1.0f);

	m_quadVertices[m_quadVerticesCount + 0] = QuadVertex{ .position = v0, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 1] = QuadVertex{ .position = v1, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 2] = QuadVertex{ .position = v2, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 3] = QuadVertex{ .position = v3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };

	m_quadVerticesCount += 4;
}

void Renderer2D::drawLine(const Vector3f& start, const Vector3f& end, const Color& color)
{
	if (m_quadVerticesCount > 0)
		flushQuads();

	if (m_lineVerticesCount + 2 >= MAX_LINE_VERTICES)
		flushLines();

	Vector4f normColor = color.normalized();
	m_lineVertices[m_lineVerticesCount++] = LineVertex{ normColor, start };
	m_lineVertices[m_lineVerticesCount++] = LineVertex{ normColor, end };
}

void Renderer2D::drawLine(const Vector2f& start, const Vector2f& end, const Color& color)
{
	drawLine(Vector3f(start.x, start.y, 0.0f), Vector3f(end.x, end.y, 0.0f), color);
}

}
