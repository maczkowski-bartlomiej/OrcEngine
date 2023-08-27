#include "Game.hpp"

Game::Game()
	: m_camera(0.0f, 800.0f, 600.0f, 0.0f), m_position(0.0f), m_textureHolder("assets/textures.xml")
{
	orc::Reference<orc::Texture> playerTexture = m_textureHolder.getResource("soldier_texture");
	m_player = orc::createReference<orc::Sprite>(playerTexture);

	orc::Reference<orc::Texture> zombieTexture = m_textureHolder.getResource("zombie_texture");
	m_zombie = orc::createReference<orc::Sprite>(zombieTexture);

	m_zombie->setRotation(0.0f);
	m_zombie->setPosition(500, 400);
	m_zombie->setColor(orc::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	m_player->setRotation(0.0f);
	m_player->setPosition(200, 200);
	m_player->setColor(orc::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	m_player->setOrigin(m_player->getSize() / 2.0f);
	//m_player->setTextureRect(orc::IntRect(playerTexture->getWidth(), playerTexture->getHeight(), playerTexture->getWidth(), playerTexture->getHeight()));}

	m_rectangle = orc::createReference<orc::Rectangle>(orc::Vector2f{ 0, 0 }, orc::Vector2f{ 100, 100 }, orc::Vector4f(0.0, 0.0, 1.0, 1.0), false);
	m_rectangle->setScale(0.5f, 0.5f);
	/*
		FillColor, BorderColor (draw lines first i guess)
		It will not work with scaling etc? check that
	*/
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
	float speed = 50.0f;
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))
	{
		m_position.x -= speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))
	{
		m_position.x += speed * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))
	{
		m_position.y -= speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))
	{
		m_position.y += speed * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Left))
	{
		m_cameraPosition.x -= speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Right))
	{
		m_cameraPosition.x += speed * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Up))
	{
		m_cameraPosition.y -= speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Down))
	{
		m_cameraPosition.y += speed * deltaTime;
	}

	m_player->move(m_position);
	m_position = orc::Vector2f();

	m_camera.setPosition(m_cameraPosition);

	orc::Vector2f mouse = orc::Mouse::getMousePosition();
	orc::Vector2f playerPos = m_player->getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player->setRotation(angle);

	//orc::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	orc::Renderer::setClearColor({ 1.f, 1.f, 1.f, 1.f });
	orc::Renderer::clear();

	orc::Renderer::begin(m_camera);

	orc::Renderer::drawLine({ 100, 100 }, { 500, 100 }, { 1.0f, 0.0f, 0.0f, 0.50f });
	orc::Renderer::drawLine({ 100, 200 }, { 500, 300 }, { 1.0f, 0.0f, 0.0f, 1.00f });

	orc::Renderer::draw(m_rectangle);

	//orc::Renderer::draw(m_player);
	//orc::Renderer::draw(m_zombie);

	orc::Renderer::end();
}

void Game::onEvent(orc::Event& event) 
{
	if (event.getType() == orc::Event::Type::MouseButtonPressed)
	{
		auto newEvent = (orc::MouseButtonPressedEvent*)&event;
		if (newEvent->button == orc::Mouse::Button::Left)
		{
		}
	}
}
