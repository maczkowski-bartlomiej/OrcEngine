#include "Game.hpp"

#include <glm/gtc/matrix_transform.hpp>

Game::Game()
	: m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_position(0.0f), m_shaderHolder("assets/shaders.xml"), m_textureHolder("assets/textures.xml")
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

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
	};

	orc::Reference<orc::VertexBuffer> squareVB = orc::createReference<orc::VertexBuffer>(squareVertices, (orc::uint32)sizeof(squareVertices));
	squareVB->setLayout({
		{ orc::BufferLayout::ShaderDataType::Float3, "a_position" },
		{ orc::BufferLayout::ShaderDataType::Float2, "a_textureCoords" }
	});

	m_squareVA->addVertexBuffer(squareVB);

	orc::uint32 squareIndices[6] = { 0u, 1u, 2u, 2u, 3u, 0u };
	orc::Reference<orc::IndexBuffer> squareIB = orc::createReference<orc::IndexBuffer>(squareIndices, (orc::uint32)sizeof(squareIndices) / (orc::uint32)sizeof(orc::uint32));
	m_squareVA->setIndexBuffer(squareIB);

	m_texture = m_textureHolder.getResource("shrek_texture");

	m_textureShader = m_shaderHolder.getResource("texture_shader");
	m_textureShader->bind();
	m_textureShader->uploadUniformInt("u_texture", 0);
	m_flatColorShader = m_shaderHolder.getResource("flatColor_shader");

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

void Game::onUpdate(float deltaTime) 
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

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Left))
	{
		m_cameraPosition.x -= 1.0f * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Right))
	{
		m_cameraPosition.x += 1.0f * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Up))
	{
		m_cameraPosition.y += 1.0f * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Down))
	{
		m_cameraPosition.y -= 1.0f * deltaTime;
	}

	m_camera.setPosition(m_cameraPosition);

	orc::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	orc::Renderer::clear();

	orc::Renderer::beginScene(m_camera);

	orc::Matrix scale = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

	orc::Vector4f redColor = { 0.8f, 0.1f, 0.1f, 1.0f };
	orc::Vector4f blueColor = { 0.1f, 0.1f, 0.8f, 1.0f };
	
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

			if (x % 2 == y % 2)
			{
				m_flatColorShader->uploadUniformFloat4("u_color", redColor);
			}
			else
			{
				m_flatColorShader->uploadUniformFloat4("u_color", blueColor);
			}

			orc::Renderer::submit(m_squareVA, m_flatColorShader, transform);
		}
	}

	m_texture->bind();
	orc::Renderer::submit(m_squareVA, m_textureShader);

	orc::Renderer::endScene();
}

void Game::onEvent(orc::Event& event) 
{
	if (event.getType() == orc::Event::Type::MouseButtonPressed)
	{
		auto newEvent = (orc::MouseButtonPressedEvent*)&event;
		if (newEvent->button == orc::Mouse::Button::Left)
		{
			test--;
			ORC_LOG_INFO("Value: {}", test);
		}
	}
}
