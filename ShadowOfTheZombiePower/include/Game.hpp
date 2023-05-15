#pragma once

#include <Orc/Orc.hpp>

class Game : public orc::GameLayer
{
public:
	Game();
	~Game();

	void onAttach()  override;
	void onDetach()  override;
	void onUpdate(double deltaTime)  override;
	void onEvent(orc::Event& event)  override;

private:
	orc::Camera m_camera;
	orc::Reference<orc::Shader> m_shader;
	orc::Reference<orc::Shader> m_blueShader;
	orc::Reference<orc::VertexArray> m_vertexArray;
	orc::Reference<orc::VertexArray> m_squareVA;

	glm::vec3 m_position;
};
