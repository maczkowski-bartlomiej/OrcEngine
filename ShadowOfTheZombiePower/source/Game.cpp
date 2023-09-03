#include "Game.hpp"

Game::Game()
	: m_camera(0.0f, 800.0f, 600.0f, 0.0f), m_position(0.0f), m_textureHolder("assets/textures.xml")
{
	orc::Ref<orc::Texture> playerTexture = m_textureHolder.getResource("soldier_texture");
	m_player = orc::createRef<orc::Sprite>(playerTexture);

	orc::Ref<orc::Texture> zombieTexture = m_textureHolder.getResource("zombie_texture");
	m_zombie = orc::createRef<orc::Sprite>(zombieTexture);

	m_zombie->setRotation(0.0f);
	//m_zombie->setPosition(500, 400);
	//m_zombie->setColor(orc::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	m_player->setRotation(0.0f);
	m_player->setPosition(400.0f, 300.0f);
	m_player->setOrigin(m_player->getGlobalRect().getSize() / 2.0f);
	m_player->setTextureRect(orc::IntRect(0, 0, playerTexture->getWidth(), playerTexture->getHeight()));
	m_player->setScale(1.0f, 1.0f);
	m_player->setTexture(playerTexture);
	m_player->setColor(orc::Color(255, 255, 255, 255));

	orc::Ref<orc::Texture> gradientTexture = m_textureHolder.getResource("gradient_texture");
	m_rectangle = orc::createRef<orc::Rectangle>(orc::Vector2f{ 0, 0 }, orc::Vector2f{ 100, 100 }, orc::Color());
	m_rectangle->setTexture(gradientTexture);

	//m_rectangle->setScale(0.5f, 0.5f);
	/*
		FillColor, BorderColor, thincess maybe (lines thicness then??) (draw lines first i guess)
		texture repeat, clamp?
		circles,
		polygons,
		text
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
		m_cameraPosition.x += speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Right))
	{
		m_cameraPosition.x -= speed * deltaTime;
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Up))
	{
		m_cameraPosition.y += speed * deltaTime;
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Down))
	{
		m_cameraPosition.y -= speed * deltaTime;
	}

	//m_player->move(m_position);
	m_position = orc::Vector2f();

	m_camera.setPosition(m_cameraPosition);

	orc::Vector2f mouse = orc::Mouse::getMousePosition();
	orc::Vector2f playerPos = m_player->getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player->setRotation(angle);

	orc::Renderer::setClearColor(orc::Color(25, 25, 25, 255));
	orc::Renderer::clear();

	orc::Renderer::begin(m_camera);

	orc::Renderer::drawLine({ 100, 100 }, { 500, 100 }, orc::Color(255, 0, 0));
	orc::Renderer::drawLine({ 100, 200 }, { 500, 300 }, orc::Color(255, 0, 0));

	orc::Renderer::draw(m_rectangle);

	orc::Renderer::draw(m_player);
	orc::Renderer::draw(m_zombie);

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
