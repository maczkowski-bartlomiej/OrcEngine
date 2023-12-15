#include "Game.hpp"

Game::Game()
	: m_camera(0.0f, 800.0f, 600.0f, 0.0f), m_position(0.0f), m_textureHolder("assets/textures.xml"), m_soundBufferHolder("assets/audio.xml")
{
	orc::Audio audio;
	audio.playSound(m_soundBufferHolder.getResource("example_sound"), { 0, 0 });
	orc::Ref<orc::Texture> playerTexture = m_textureHolder.getResource("soldier_texture");
	m_player = orc::createRef<orc::Sprite>(playerTexture);

	orc::Ref<orc::Texture> zombieTexture = m_textureHolder.getResource("zombie_texture");
	m_zombie = orc::createRef<orc::Sprite>(zombieTexture);

	m_zombie->setRotation(0.0f);
	m_zombie->setPosition(500, 400);
	m_zombie->setColor(orc::Color(1, 1, 1, 1));

	m_player->setRotation(0.0f);
	m_player->setPosition(0.0f, 0.0f);
	m_player->setOrigin(m_player->getGlobalRect().getSize() / 2.0f);
	m_player->setScale(0.3f, 0.3f);
	m_player->setTexture(playerTexture);

	orc::Ref<orc::Texture> gradientTexture = m_textureHolder.getResource("gradient_texture");

	m_rectangle1 = orc::createRef<orc::Rectangle>(orc::Vector2f{ 0, 0 }, orc::Vector2f(100, 100));
	m_rectangle1->setTexture(gradientTexture);

	m_rectangle2 = orc::createRef<orc::Rectangle>(orc::Vector2f(500, 200), orc::Vector2f(100, 100));
	m_rectangle2->setFillColor(orc::Color(255, 0, 0, 255));
	m_rectangle2->setBorderColor(orc::Color(255, 255, 255));
	m_rectangle2->setRotation(45.0f);
	m_rectangle2->setOrigin(m_rectangle2->getSize() / 2.0f);

	m_rectangle3 = orc::createRef<orc::Rectangle>(orc::Vector2f(100, 100), orc::Vector2f(100, 100));
	m_rectangle3->setThickness(0.5f);
	m_rectangle3->setTexture(m_textureHolder.getResource("small_texture"));
	m_rectangle3->setTextureRect(orc::FloatRect(0, 0, 100, 100));
	m_rectangle3->getTexture()->setTextureWrapping(false);

	m_rectangle4 = orc::createRef<orc::Rectangle>(orc::Vector2f(300, 300), orc::Vector2f(100, 100));
	m_rectangle4->setTexture(gradientTexture);
	m_rectangle4->setFillColor(orc::Color(255, 0, 0));
	m_rectangle4->setBorderColor(orc::Color(255, 255, 255));

	m_sprite1 = orc::createRef<orc::Sprite>(playerTexture);
	m_sprite1->setOrigin(m_sprite1->getTexture()->getSize() / 2.0f);
	m_sprite2 = orc::createRef<orc::Sprite>(playerTexture);
	m_sprite3 = orc::createRef<orc::Sprite>(playerTexture);
	m_sprite4 = orc::createRef<orc::Sprite>(playerTexture);

	m_sprite1->setPosition(orc::Vector2f(0, 0));
	m_sprite2->setPosition(orc::Vector2f(100, 100));
	m_sprite3->setPosition(orc::Vector2f(200, 200));
	m_sprite4->setPosition(orc::Vector2f(300, 300));

	m_sprite1->setScale(0.7f, 0.7f);
	m_sprite2->setScale(0.3f, 0.3f);
	m_sprite3->setScale(0.3f, 0.3f);
	m_sprite4->setScale(0.3f, 0.3f);

	m_circle1 = orc::createRef<orc::Circle>();
	m_circle1->setPosition(200.0f, 450.f);
	m_circle1->setRadius(60.0f);
	m_circle1->setTexture(m_textureHolder.getResource("gradient_texture"));

	m_circle2 = orc::createRef<orc::Circle>();
	m_circle2->setFillColor(orc::Color(0, 255, 0));
	m_circle2->setPosition(300.0f, 150.f);
	m_circle2->setRadius(30.0f);
	m_circle2->setThickness(0.5f);
	m_circle2->setBorderColor(orc::Color(255, 255, 255));
	m_circle2->setBorderThickness(0.10f);

	m_circle3 = orc::createRef<orc::Circle>();
	m_circle3->setBorderThickness(0.2f);
	m_circle3->setBorderColor(orc::Color(0, 0, 255));
	m_circle3->setFillColor(orc::Color(255, 0, 0, 125));
	m_circle3->setPosition(400.0f, 400.f);
	m_circle3->setRadius(60.0f);

	m_rectangle5 = orc::createRef<orc::Rectangle>(m_circle2->getPosition(), orc::Vector2f(m_circle2->getGlobalRect().width, m_circle2->getGlobalRect().height));
	m_rectangle5->setFillColor(orc::Color(0, 0, 0, 0));
	m_rectangle5->setBorderColor(orc::Color(255, 255, 255));

	m_audioBuffer = m_soundBufferHolder.getResource("example_sound");
	/*
		Disable openAl internal logging
		Add music streaming support

		OpenAll, drawable tpp files?
	
		Empty sprite texture handling??? 
		Renderer check, rethink of invalid texture index
		Rethink of wrapping, clamping repeating...
		Normalize values from float to px at setting circles, shaders values etc...

		Make more examples with different shapes, sprites, circles, rectangles, lines with transformions

		New stuff:
		- Rework event system
		- add text rendering
		- add GUI
	*/
}

Game::~Game()
{
	ORC_LOG_INFO("Game shutting down...");
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

	m_player->setPosition(m_position);

	m_camera.setPosition(m_cameraPosition);

	orc::Vector2f mouse = orc::Mouse::getMousePosition();
	orc::Vector2f playerPos = m_player->getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_sprite1->setRotation(angle);

	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(m_camera);

	renderer.drawLine({ 100, 100 }, { 500, 100 }, orc::Color(255, 0, 0));
	renderer.drawLine({ 100, 200 }, { 500, 300 }, orc::Color(255, 0, 0));

	renderer.draw(m_rectangle1);
	renderer.draw(m_rectangle2);
	renderer.draw(m_rectangle3);
	renderer.draw(m_rectangle4);
	renderer.draw(m_rectangle5);

	renderer.draw(m_sprite1);
	renderer.draw(m_sprite2);
	renderer.draw(m_sprite3);
	renderer.draw(m_sprite4);

	m_sprite1->setPosition(m_position);

	renderer.draw(m_player);
	renderer.draw(m_zombie);

	renderer.draw(m_circle1);
	renderer.draw(m_circle2);
	renderer.draw(m_circle3);

	renderer.end();
}

void Game::onEvent(orc::Event& event) 
{
	if (event.getType() == orc::Event::Type::MouseButtonPressed)
	{
		auto newEvent = (orc::MouseButtonPressedEvent*)&event;
		if (newEvent->button == orc::Mouse::Button::Left)
		{
			audio.playSound(m_audioBuffer, { 0, 0 });
		}
	}
}
