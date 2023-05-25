#pragma once

#include <Orc/Orc.hpp>

class Game : public orc::GameLayer
{
public:
	Game();
	~Game();

	void onAttach()  override;
	void onDetach()  override;
	void onUpdate(float deltaTime)  override;
	void onEvent(orc::Event& event)  override;

private:
	orc::Camera m_camera;

	orc::Reference<orc::Texture> m_texture;

	orc::Reference<orc::Shader> m_textureShader;
	orc::Reference<orc::Shader> m_flatColorShader;
	orc::Reference<orc::VertexArray> m_vertexArray;
	orc::Reference<orc::VertexArray> m_squareVA;

	glm::vec3 m_position;
	orc::Vector2f m_cameraPosition;

	orc::ShaderHolder m_shaderHolder;
	orc::TextureHolder m_textureHolder;

	int test = 300;
};
