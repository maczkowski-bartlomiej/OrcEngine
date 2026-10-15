#include "OrcPch.hpp"
#include "Graphics/Renderer.hpp"
#include "Engine/Debug.hpp"

#include <glad/glad.h>
#include <cmath>
#include <span>

namespace orc {

Renderer::Renderer()
{
	ORC_LOG_INFO("Initializing unified renderer.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup composite quad shader
	m_quadShader = createRef<Shader>("shaders/composite2d.vert.glsl", "shaders/composite2d.frag.glsl");
	m_quadShader->bind();

	int32_t samplers[MAX_TEXTURE_SLOTS] = {};
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = static_cast<int32_t>(i);

	m_quadShader->uploadUniformIntArray("u_textures", samplers, MAX_TEXTURE_SLOTS);

	m_quadVertexBuffer = createRef<VertexBuffer>(nullptr, static_cast<int32_t>(sizeof(Vertex) * MAX_QUAD_VERTICES));
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

	startBatch();
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

void Renderer::beginScene(const Camera& camera)
{
	m_viewProjectionMatrix = camera.getViewProjectionMatrix();
	m_isSceneActive = true;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	startBatch();
}

void Renderer::endScene()
{
	flush();
	m_isSceneActive = false;
}

void Renderer::startBatch()
{
	m_quadVerticesCount = 0;
	m_textures.clear();
}

void Renderer::flush()
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

	startBatch();
}

float Renderer::getTextureSlot(Ref<Texture> texture)
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
		flush();
	}

	float slot = static_cast<float>(m_textures.size());
	m_textures.push_back(texture);
	return slot;
}

void Renderer::submitQuad(const std::array<Vertex, 4>& vertices, float textureSlot)
{
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flush();

	std::copy_n(vertices.data(), 4, &m_quadVertices[m_quadVerticesCount]);
	for (size_t i = 0; i < 4; ++i)
		m_quadVertices[m_quadVerticesCount + i].textureIndex = textureSlot;
	m_quadVerticesCount += 4;
}

void Renderer::draw(const Sprite& sprite)
{
	if (!sprite.getTexture())
		return;

	float texSlot = getTextureSlot(sprite.getTexture());
	submitQuad(sprite.getVertices(), texSlot);
}

void Renderer::draw(const Rectangle& rectangle)
{
	float texSlot = rectangle.getTexture() ? getTextureSlot(rectangle.getTexture()) : EMPTY_TEXTURE_SLOT;
	submitQuad(rectangle.getVertices(), texSlot);
}

void Renderer::draw(const Circle& circle)
{
	float texSlot = circle.getTexture() ? getTextureSlot(circle.getTexture()) : EMPTY_TEXTURE_SLOT;
	submitQuad(circle.getVertices(), texSlot);
}

void Renderer::draw(const Text& text)
{
	if (!text.getFont() || !text.getFont()->getBitmap())
		return;

	Ref<Texture> fontBitmap = text.getFont()->getBitmap();
	float texSlot = getTextureSlot(fontBitmap);
	const auto& glyphVertices = text.getVertices();
	size_t count = glyphVertices.size();

	for (size_t i = 0; i < count; i += 4)
	{
		if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		{
			flush();
			texSlot = getTextureSlot(fontBitmap);
		}

		size_t verticesToCopy = std::min<size_t>(4, count - i);
		std::copy_n(&glyphVertices[i], verticesToCopy, &m_quadVertices[m_quadVerticesCount]);
		for (size_t j = 0; j < verticesToCopy; ++j)
			m_quadVertices[m_quadVerticesCount + j].textureIndex = texSlot;
		m_quadVerticesCount += static_cast<uint32_t>(verticesToCopy);
	}
}

void Renderer::drawQuad(const Vector3f& position, const Vector2f& size, const Color& color)
{
	Matrix transform = glm::translate(Matrix(1.0f), Vector2f(position.x, position.y));
	drawQuad(transform, size, color);
}

void Renderer::drawQuad(const Vector3f& position, const Vector2f& size, Ref<Texture> texture, const Color& tintColor)
{
	Matrix transform = glm::translate(Matrix(1.0f), Vector2f(position.x, position.y));
	drawQuad(transform, size, texture, tintColor);
}

void Renderer::drawQuad(const Matrix& transform, const Vector2f& size, const Color& color)
{
	drawQuad(transform, size, nullptr, color);
}

void Renderer::drawQuad(const Matrix& transform, const Vector2f& size, Ref<Texture> texture, const Color& tintColor)
{
	float texSlot = texture ? getTextureSlot(texture) : EMPTY_TEXTURE_SLOT;
	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flush();

	Vector4f normColor = tintColor.normalized();
	Vector3f v0 = transform * Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f v1 = transform * Vector3f(size.x, 0.0f, 1.0f);
	Vector3f v2 = transform * Vector3f(0.0f, size.y, 1.0f);
	Vector3f v3 = transform * Vector3f(size.x, size.y, 1.0f);

	m_quadVertices[m_quadVerticesCount + 0] = Vertex{ .position = v0, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 1] = Vertex{ .position = v1, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 2] = Vertex{ .position = v2, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 3] = Vertex{ .position = v3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = texSlot, .shapeType = 0.0f };

	m_quadVerticesCount += 4;
}

void Renderer::drawLine(const Vector3f& start, const Vector3f& end, const Color& color, float thickness)
{
	Vector2f dir = Vector2f(end.x - start.x, end.y - start.y);
	float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (len < 0.0001f)
		return;

	// Compute perpendicular normal for thickness
	Vector2f normal = Vector2f(-dir.y / len, dir.x / len) * (thickness * 0.5f);
	Vector3f n3 = Vector3f(normal.x, normal.y, 0.0f);

	Vector3f v0 = start + n3;
	Vector3f v1 = end + n3;
	Vector3f v2 = start - n3;
	Vector3f v3 = end - n3;

	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		flush();

	Vector4f normColor = color.normalized();
	m_quadVertices[m_quadVerticesCount + 0] = Vertex{ .position = v0, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 1] = Vertex{ .position = v1, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 2] = Vertex{ .position = v2, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f };
	m_quadVertices[m_quadVerticesCount + 3] = Vertex{ .position = v3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f };

	m_quadVerticesCount += 4;
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color, float thickness)
{
	drawLine(Vector3f(start.x, start.y, 0.0f), Vector3f(end.x, end.y, 0.0f), color, thickness);
}

void Renderer::draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform)
{
	// Direct internal flush of any active batch before executing unbatched mesh draw
	flush();

	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	shader->uploadUniformMatrix3("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

}
