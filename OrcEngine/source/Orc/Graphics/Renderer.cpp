#include "OrcPch.hpp"

#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

void Renderer::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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


void Renderer::beginScene(const Camera& camera)
{
	m_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene()
{

}

void Renderer::submit(Reference<Sprite> sprite)
{
	
}

void Renderer::submit(Reference<Sprite> sprite, Reference<Shader> shader)
{
	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_sceneData->viewProjectionMatrix);
	shader->uploadUniformMatrix4("u_transform", sprite->getMatrix());

	sprite->getTexture()->bind();

	glDrawPixels(sprite->getTexture()->getWidth(), sprite->getTexture()->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, sprite->getTexture()->
}

void Renderer::submit(Reference<VertexArray> vertexArray, Reference<Shader> shader, const Matrix& transform)
{
	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_sceneData->viewProjectionMatrix);
	shader->uploadUniformMatrix4("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());

	Renderer::drawIndexed(vertexArray);
}

}
