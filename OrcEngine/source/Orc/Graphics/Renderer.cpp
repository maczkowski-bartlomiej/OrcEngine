#include "OrcPch.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Renderer2D.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Debug.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::Renderer()
{
	ORC_LOG_INFO("Initializing base renderer.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
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
	Engine::get().getRenderer2D().beginScene(camera);
}

void Renderer::end()
{
	Engine::get().getRenderer2D().endScene();
}

void Renderer::draw(const Text& text)
{
	Engine::get().getRenderer2D().draw(text);
}

void Renderer::draw(const Sprite& sprite)
{
	Engine::get().getRenderer2D().draw(sprite);
}

void Renderer::draw(const Circle& circle)
{
	Engine::get().getRenderer2D().draw(circle);
}

void Renderer::draw(const Rectangle& rectangle)
{
	Engine::get().getRenderer2D().draw(rectangle);
}

void Renderer::draw(Ref<VertexArray> vertexArray, Ref<Shader> shader, const Matrix& transform)
{
	Engine::get().getRenderer2D().flush();

	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_viewProjectionMatrix);
	shader->uploadUniformMatrix3("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawLine(const Vector2f& start, const Vector2f& end, const Color& color)
{
	Engine::get().getRenderer2D().drawLine(start, end, color);
}

}
