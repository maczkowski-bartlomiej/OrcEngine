#include "OrcPch.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/RenderCommand.hpp"

#include <glad/glad.h>

namespace orc {

Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

void Renderer::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::beginScene(const Camera& camera)
{
	m_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene()
{

}

void Renderer::submit(Reference<VertexArray> vertexArray, Reference<Shader> shader, const glm::mat4& transform)
{
	shader->bind();
	shader->uploadUniformMatrix4("u_viewProjectionMatrix", m_sceneData->viewProjectionMatrix);
	shader->uploadUniformMatrix4("u_transform", transform);

	glBindVertexArray(vertexArray->getRendererID());

	RenderCommand::drawIndexed(vertexArray);
}

}
