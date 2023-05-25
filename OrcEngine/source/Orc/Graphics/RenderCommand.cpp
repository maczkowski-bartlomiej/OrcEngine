#include "orcPch.hpp"
#include "Graphics/RenderCommand.hpp"

#include <glad/glad.h>

namespace orc {

void RenderCommand::clear() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::setClearColor(const Vector4f& color) 
{
	glClearColor(color.x, color.y, color.z, color.p);
}

void RenderCommand::drawIndexed(Reference<VertexArray> vertexArray) 
{
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

}
