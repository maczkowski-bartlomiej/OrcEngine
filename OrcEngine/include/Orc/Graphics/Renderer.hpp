#pragma once

#include <array>
#include <vector>

#include "Core/Core.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Shapes/Circle.hpp"
#include "Graphics/Shapes/Rectangle.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Text.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"

namespace orc {

class Renderer
{
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void clear();
	void setClearColor(const Color& color);
	void setViewport(uint32_t width, uint32_t height);
	void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	void beginScene(const Camera& camera);
	void endScene();
	void flush();

	// High-level shape overloads
	void draw(const Sprite& sprite);
	void draw(const Rectangle& rectangle);
	void draw(const Circle& circle);
	void draw(const Text& text);

	// Lines rendered directly as oriented quads (zero state/pipeline switches)
	void drawLine(const Vector2f& start, const Vector2f& end, const Color& color, float thickness = 1.0f);
	void drawLine(const Vector3f& start, const Vector3f& end, const Color& color, float thickness = 1.0f);

	// Unbatched custom geometry pass (flushes active 2D batch before drawing)
	void draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Matrix& transform);

private:
	void startBatch();
	float getTextureSlot(const Ref<Texture>& texture);
	void submitQuad(const std::array<Vertex, 4>& vertices, const Ref<Texture>& texture = nullptr);

	static constexpr uint32_t MAX_QUADS = 2000;
	static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
	static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;

	static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
	static constexpr float EMPTY_TEXTURE_SLOT = 32.0f;

	Ref<Shader> m_quadShader;
	Ref<VertexArray> m_quadVertexArray;
	Ref<VertexBuffer> m_quadVertexBuffer;
	std::array<Vertex, MAX_QUAD_VERTICES> m_quadVertices;
	uint32_t m_quadVerticesCount = 0;

	std::vector<Ref<Texture>> m_textures;

	Matrix4 m_viewProjectionMatrix;
	bool m_isSceneActive = false;
};

}
