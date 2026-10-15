#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Text.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Shapes/Circle.hpp"
#include "Graphics/Shapes/Rectangle.hpp"

namespace orc {

class Renderer
{
public:
	Renderer();

	void clear();
	void setClearColor(const Color& color);

	void begin(const Camera& camera);
	void end();

	void draw(const Text& text);
	void draw(const Sprite& sprite);
	void draw(const Circle& circle);
	void draw(const Rectangle& rectangle);
	void draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform);

	void drawLine(const Vector2f& start, const Vector2f& end, const Color& color);

private:
	Matrix4 m_viewProjectionMatrix;
};

}
