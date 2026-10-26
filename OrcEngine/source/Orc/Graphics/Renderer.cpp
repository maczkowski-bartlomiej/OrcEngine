#include "Graphics/Renderer.hpp"

#include <cmath>
#include <numeric>
#include <span>

#include <glad/glad.h>

#include "Engine/Debug.hpp"

namespace orc {

Renderer::Renderer()
{
	ORC_LOG_INFO("Initializing renderer.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup composite quad shader
	m_quadShader = createRef<Shader>("shaders/composite2d.vert.glsl", "shaders/composite2d.frag.glsl");
	m_quadShader->bind();

	std::array<int32_t, MAX_TEXTURE_SLOTS> samplers;
	std::iota(samplers.begin(), samplers.end(), 0);
	m_quadShader->uploadUniformIntArray("u_textures", samplers);

	m_quadVertexBuffer = createRef<VertexBuffer>(static_cast<uint32_t>(sizeof(Vertex) * MAX_QUAD_VERTICES));
	m_quadVertexBuffer->setLayout(BufferLayout{
		{BufferLayout::ShaderDataType::Float3, "a_position"},
		{BufferLayout::ShaderDataType::Float4, "a_color"},
		{BufferLayout::ShaderDataType::Float4, "a_secondaryColor"},
		{BufferLayout::ShaderDataType::Float2, "a_textureCoord"},
		{BufferLayout::ShaderDataType::Float2, "a_localPosition"},
		{BufferLayout::ShaderDataType::Float, "a_textureIndex"},
		{BufferLayout::ShaderDataType::Float, "a_shapeType"},
		{BufferLayout::ShaderDataType::Float, "a_borderThickness"},
		{BufferLayout::ShaderDataType::Float, "a_innerRadius"},
		{BufferLayout::ShaderDataType::Float, "a_radius"}});

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

	Ref<IndexBuffer> quadIndexBuffer = createRef<IndexBuffer>(std::span<const uint32_t>{quadIndices});

	m_quadVertexArray = createRef<VertexArray>();
	m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);
	m_quadVertexArray->setIndexBuffer(quadIndexBuffer);

	startBatch();

	GLint maxTextureUnits = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	ORC_LOG_INFO("Renderer:\n\tGLSL Version: {}\n\tBatch Capacity: {} quads ({} vertices, {} indices)\n\tBatch Texture Slots: {}\n\tHardware Texture Units: {}\n\tMax Texture Size: {}x{}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)), MAX_QUADS, MAX_QUAD_VERTICES, MAX_QUAD_INDICES, MAX_TEXTURE_SLOTS, maxTextureUnits, maxTextureSize, maxTextureSize);
}

Renderer::~Renderer()
{
	ORC_LOG_INFO("Deinitializing renderer.");
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

void Renderer::setViewport(uint32_t width, uint32_t height)
{
	setViewport(0, 0, width, height);
}

void Renderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
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
	{
		return;
	}

	ORC_FATAL_CHECK(m_isSceneActive, "Cannot flush renderer outside of active Scene Scope!");

	for (uint32_t s = 0; s < m_textures.size(); ++s)
	{
		m_textures[s]->bind(s);
	}

	m_quadShader->bind();
	m_quadShader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	m_quadVertexBuffer->setData(std::span{m_quadVertices.data(), m_quadVerticesCount});

	glBindVertexArray(m_quadVertexArray->getRendererID());
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((m_quadVerticesCount / 4) * 6), GL_UNSIGNED_INT, nullptr);

	startBatch();
}

float Renderer::getTextureSlot(const Ref<Texture>& texture)
{
	if (!texture)
	{
		return EMPTY_TEXTURE_SLOT;
	}

	for (size_t i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i]->getRendererID() == texture->getRendererID())
		{
			return static_cast<float>(i);
		}
	}

	if (m_textures.size() >= MAX_TEXTURE_SLOTS)
	{
		flush();
	}

	float slot = static_cast<float>(m_textures.size());
	m_textures.push_back(texture);
	return slot;
}

void Renderer::submitQuad(const std::array<Vertex, 4>& vertices, const Ref<Texture>& texture)
{
	ORC_FATAL_CHECK(m_isSceneActive, "Cannot submit geometry outside of active Scene Scope!");

	if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
	{
		flush();
	}

	float textureSlot = getTextureSlot(texture);

	for (size_t i = 0; i < 4; ++i)
	{
		m_quadVertices[m_quadVerticesCount + i] = vertices[i];
		m_quadVertices[m_quadVerticesCount + i].textureIndex = textureSlot;
	}
	m_quadVerticesCount += 4;
}

void Renderer::draw(const Sprite& sprite)
{
	ORC_FATAL_CHECK(sprite.getTexture() != nullptr, "Cannot draw Sprite without a valid Texture!");

	submitQuad(sprite.getVertices(), sprite.getTexture());
}

void Renderer::draw(const Rectangle& rectangle)
{
	submitQuad(rectangle.getVertices(), rectangle.getTexture());
}

void Renderer::draw(const Circle& circle)
{
	submitQuad(circle.getVertices(), circle.getTexture());
}

void Renderer::draw(const Text& text)
{
	ORC_FATAL_CHECK(m_isSceneActive, "Cannot submit geometry outside of active Scene Scope!");
	ORC_FATAL_CHECK(text.getFont() != nullptr && text.getFont()->getBitmap() != nullptr, "Cannot draw Text without a valid Font!");

	const Ref<Texture>& fontBitmap = text.getFont()->getBitmap();
	const auto& glyphVertices = text.getVertices();
	size_t count = glyphVertices.size();

	for (size_t i = 0; i < count; i += 4)
	{
		if (m_quadVerticesCount + 4 >= MAX_QUAD_VERTICES)
		{
			flush();
		}

		float texSlot = getTextureSlot(fontBitmap);
		size_t verticesToCopy = std::min<size_t>(4, count - i);

		for (size_t j = 0; j < verticesToCopy; ++j)
		{
			m_quadVertices[m_quadVerticesCount + j] = glyphVertices[i + j];
			m_quadVertices[m_quadVerticesCount + j].textureIndex = texSlot;
		}
		m_quadVerticesCount += static_cast<uint32_t>(verticesToCopy);
	}
}

void Renderer::drawLine(const Vector3f& start, const Vector3f& end, const Color& color, float thickness)
{
	Vector2f dir = Vector2f(end.x - start.x, end.y - start.y);
	float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (len < 0.0001f)
	{
		return;
	}

	// Compute perpendicular normal for thickness
	Vector2f normal = Vector2f(-dir.y / len, dir.x / len) * (thickness * 0.5f);
	Vector3f n3 = Vector3f(normal.x, normal.y, 0.0f);

	Vector4f normColor = color.normalized();
	std::array<Vertex, 4> vertices{
		Vertex{.position = start + n3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f},
		Vertex{.position = end + n3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 0.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f},
		Vertex{.position = start - n3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(0.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f},
		Vertex{.position = end - n3, .color = normColor, .secondaryColor = Vector4f(1.0f), .textureCoord = Vector2f(1.0f, 1.0f), .localPosition = Vector2f(0.0f), .textureIndex = EMPTY_TEXTURE_SLOT, .shapeType = 0.0f}};

	submitQuad(vertices, nullptr);
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color, float thickness)
{
	drawLine(Vector3f(start.x, start.y, 0.0f), Vector3f(end.x, end.y, 0.0f), color, thickness);
}

void Renderer::draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Matrix& transform)
{
	ORC_FATAL_CHECK(m_isSceneActive, "Cannot draw geometry outside of active Scene Scope!");

	// Direct internal flush of any active batch before executing unbatched mesh draw
	flush();

	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	shader->uploadUniformMatrix3("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}

}
