#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"

namespace orc {

class Renderer
{
public:
	static void init();

	static void clear();
	static void setClearColor(const Vector4f& color);

	static void drawIndexed(Reference<VertexArray> vertexArray);

	static void beginScene(const Camera& camera);
	static void endScene();

	static void submit(Reference<Sprite> sprite);
	static void submit(Reference<Sprite> sprite, Reference<Shader> shader);

	static void submit(Reference<VertexArray> vertexArray, Reference<Shader> shader, const Matrix& transform = Matrix(1.0f));

private:
	struct SceneData
	{
		Matrix viewProjectionMatrix;
	};

	static SceneData* m_sceneData;
};

}
