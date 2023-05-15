#include "Game.hpp"

#include <glm/gtc/matrix_transform.hpp>

Game::Game()
	: m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_position(0.0f)
{
	m_vertexArray = orc::createReference<orc::VertexArray>();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
		
	orc::Reference<orc::VertexBuffer> vertexBuffer = orc::createReference<orc::VertexBuffer>(vertices, 84u);

	vertexBuffer->setLayout({
			{ orc::BufferLayout::ShaderDataType::Float3, "a_position" },
			{ orc::BufferLayout::ShaderDataType::Float4, "a_color" }
		});

	m_vertexArray->addVertexBuffer(vertexBuffer);

	orc::uint32 indices[3] = { 0u, 1u, 2u };
	orc::Reference<orc::IndexBuffer> indexBuffer = orc::createReference<orc::IndexBuffer>(indices, 3u);

	m_vertexArray->setIndexBuffer(indexBuffer);

	m_squareVA = orc::createReference<orc::VertexArray>();

	float squareVertices[3 * 4] = {
		-0.75f, -0.75f, 0.0f,
		 0.75f, -0.75f, 0.0f,
		 0.75f, 0.75f, 0.0f,
		-0.75f, 0.75f, 0.0f,
	};

	orc::Reference<orc::VertexBuffer> squareVB = orc::createReference<orc::VertexBuffer>(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ orc::BufferLayout::ShaderDataType::Float3, "a_position" }
	});
	m_squareVA->addVertexBuffer(squareVB);

	orc::uint32 squareIndices[6] = { 0u, 1u, 2u, 2u, 3u, 0u };
	orc::Reference<orc::IndexBuffer> squareIB = orc::createReference<orc::IndexBuffer>(squareIndices, sizeof(squareIndices) / sizeof(orc::uint32));
	m_squareVA->setIndexBuffer(squareIB);

	std::string vertexSource = R"(
			#version 460 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			
			uniform mat4 u_viewProjectionMatrix;
			uniform mat4 u_transform;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
			}
		)";

	std::string fragmentSource = R"(
			#version 460

			layout(location = 0) out vec4 color;
			
			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

	m_shader = orc::createReference<orc::Shader>(vertexSource, fragmentSource);

	std::string blueShaderVertexSource = R"(
			#version 460 core

			layout(location = 0) in vec3 a_position;
			
			uniform mat4 u_viewProjectionMatrix;
			uniform mat4 u_transform;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
			}
		)";

	std::string blueShaderFragmentSource = R"(
			#version 460

			layout(location = 0) out vec4 color;
			
			in vec3 v_position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.0, 1.0);
			}
		)";

	m_blueShader = orc::createReference<orc::Shader>(blueShaderVertexSource, blueShaderFragmentSource);
}

Game::~Game()
{

}

void Game::onAttach() 
{
}

void Game::onDetach() 
{
}

void Game::onUpdate(double deltaTime) 
{
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))
	{
		m_position.x -= 1.0f * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))
	{
		m_position.x += 1.0f * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))
	{
		m_position.y += 1.0f * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))
	{
		m_position.y -= 1.0f * deltaTime;
	}

	orc::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	orc::RenderCommand::clear();

	orc::Renderer::beginScene(m_camera);

	glm::mat4 transform = glm::translate(glm::mat4(1.0), m_position);
	orc::Renderer::submit(m_squareVA, m_blueShader);
	orc::Renderer::submit(m_vertexArray, m_shader, transform);

	orc::Renderer::endScene();
}

void Game::onEvent(orc::Event& event) 
{

}
