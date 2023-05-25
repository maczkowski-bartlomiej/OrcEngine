#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

#include "Math/Vector4.hpp"
#include "Engine/Core.hpp"


namespace orc {

class Renderer
{
public:
	static void init();

	static void beginScene(const Camera& camera);
	static void endScene();

	static void submit(Reference<VertexArray> vertexArray, Reference<Shader> shader, const glm::mat4& transform = glm::mat4(1.0));

private:
	struct SceneData
	{
		glm::mat4 viewProjectionMatrix;
	};

	static SceneData* m_sceneData;
};

}
