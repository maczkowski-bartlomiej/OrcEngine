#include "Game.hpp"

Game::Game()
	: m_camera(0.0f, 800.0f, 600.0f, 0.0f), m_position(0.0f)
{
	orc::Ref<orc::Texture> playerTexture = textureHolder.getResource("soldier_texture");
	m_player = orc::createRef<orc::Sprite>(playerTexture);

	orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	m_zombie = orc::createRef<orc::Sprite>(zombieTexture);

	m_zombie->setRotation(0.0f);
	m_zombie->setPosition(500, 400);
	m_zombie->setColor(orc::Color(1, 1, 1, 1));

	m_player->setRotation(0.0f);
	m_player->setPosition(0.0f, 0.0f);
	//m_player->setOrigin(m_player->getGlobalRect().getSize() / 2.0f);
	m_player->setScale(0.3f, 0.3f);
	m_player->setTexture(playerTexture);

	m_emptySprite = orc::createRef<orc::Sprite>();
	m_emptySprite->setPosition(orc::Vector2f(500, 600));

	orc::Ref<orc::Texture> gradientTexture = textureHolder.getResource("gradient_texture");

	m_rectangle1 = orc::createRef<orc::Rectangle>(orc::Vector2f{ 0, 0 }, orc::Vector2f(100, 100));
	m_rectangle1->setTexture(gradientTexture);

	m_rectangle2 = orc::createRef<orc::Rectangle>(orc::Vector2f(500, 200), orc::Vector2f(100, 100));
	m_rectangle2->setFillColor(orc::Color(255, 255, 0, 255));
	m_rectangle2->setBorderThickness(0.05f);
	m_rectangle2->setBorderColor(orc::Color(255, 255, 255));
	m_rectangle2->setRotation(45.0f);
	m_rectangle2->setOrigin(m_rectangle2->getSize() / 2.0f);

	m_rectangle3 = orc::createRef<orc::Rectangle>(orc::Vector2f(100, 100), orc::Vector2f(100, 100));
	m_rectangle3->setTexture(textureHolder.getResource("small_texture"));
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
	m_circle1->setTexture(textureHolder.getResource("gradient_texture"));

	m_circle2 = orc::createRef<orc::Circle>();
	m_circle2->setFillColor(orc::Color(0, 255, 0));
	m_circle2->setPosition(300.0f, 150.f);
	m_circle2->setRadius(30.0f);
	m_circle2->setInnerRadius(0.3f);
	m_circle2->setBorderColor(orc::Color(255, 255, 255));
	m_circle2->setBorderThickness(0.10f);

	m_circle3 = orc::createRef<orc::Circle>();
	m_circle3->setBorderThickness(0.4f);
	m_circle3->setBorderColor(orc::Color(0, 0, 255));
	m_circle3->setFillColor(orc::Color(255, 0, 0, 125));
	m_circle3->setPosition(400.0f, 400.f);
	m_circle3->setRadius(60.0f);

	m_rectangle5 = orc::createRef<orc::Rectangle>(m_circle2->getPosition(), orc::Vector2f(m_circle2->getGlobalRect().width, m_circle2->getGlobalRect().height));
	m_rectangle5->setFillColor(orc::Color(0, 0, 0, 0));
	m_rectangle5->setBorderColor(orc::Color(255, 255, 255));

	m_audioBuffer = soundBufferHolder.getResource("example_sound");

	m_font = orc::createRef<orc::Font>("assets/fonts/arial.ttf");
	m_text = orc::createRef<orc::Text>(m_font);
	//m_text->setScale(0.01f, 0.01f);
	m_text->setString("Lera jest piekna i wy wszyscy tez cnie");
	m_text->setScale(0.5f, 0.5f);
	m_fontText = orc::createRef<orc::Sprite>(m_font->getBitmap());
	//m_fontText->setScale(1.0f, 1.0f);
	//m_fontText->setColor(orc::Color(255, 0, 0, 255));
	//std::array<orc::SpriteVertex, 4> vertices;
	//vertices[0].color = m_text->m_vertices[0].color;
	//vertices[0].position = m_text->m_vertices[0].position;
	//vertices[0].textureCoord = m_text->m_vertices[0].textureCoord;
	//vertices[1].color = m_text->m_vertices[1].color;
	//vertices[1].position = m_text->m_vertices[1].position;
	//vertices[1].textureCoord = m_text->m_vertices[1].textureCoord;
	//vertices[2].color = m_text->m_vertices[2].color;
	//vertices[2].position = m_text->m_vertices[2].position;
	//vertices[2].textureCoord = m_text->m_vertices[2].textureCoord;
	//vertices[3].color = m_text->m_vertices[3].color;
	//vertices[3].position = m_text->m_vertices[3].position;
	//vertices[3].textureCoord = m_text->m_vertices[3].textureCoord;

	//vertices[0].position = { 1, 1 };
	//vertices[1].position = { 33, 1 };
	//vertices[2].position = { 1, 33 };
	//vertices[3].position = { 33, 33 };
	//vertices[0].position = vertices[0].position + orc::Vector2f{ 0.01, 0.01 };
	//vertices[1].position = vertices[1].position + orc::Vector2f{ 0.01, 0.01 };
	//vertices[2].position = vertices[2].position + orc::Vector2f{ 0.01, 0.01 };
	//vertices[3].position = vertices[3].position + orc::Vector2f{ 0.01, 0.01 };

	//m_fontText->setVertices(vertices);
	//
	//ORC_LOG_INFO("Text pos vertices: {0}, {1}, {2}, {3}", vertices[0].position.x, vertices[1].position.x, vertices[2].position.x, vertices[3].position.x);
	////ORC_LOG_INFO("Text texCoord vertices: {0}:{1}, {2}:{3}, {3}:{4}, {5}:{6}", vertices[0].textureCoord.x, vertices[0].textureCoord.y, vertices[1].textureCoord.x, vertices[1].textureCoord.y, vertices[2].textureCoord.x, vertices[2].textureCoord.y, vertices[3].textureCoord.x, vertices[3].textureCoord.y);
	////
	////vertices = m_fontText->getVertices();
	////
	////ORC_LOG_INFO("Text pos vertices: {0}, {1}, {2}, {3}", vertices[0].position.x, vertices[1].position.x, vertices[2].position.x, vertices[3].position.x);
	////ORC_LOG_INFO("Text texCoord vertices: {0}:{1}, {2}:{3}, {3}:{4}, {5}:{6}", vertices[0].textureCoord.x, vertices[0].textureCoord.y, vertices[1].textureCoord.x, vertices[1].textureCoord.y, vertices[2].textureCoord.x, vertices[2].textureCoord.y, vertices[3].textureCoord.x, vertices[3].textureCoord.y);//
//

	//m_fontText->setVertices(vertices);

	/*
	* 
		Texture mode
		Text \n\t?

		Make more examples with different shapes, sprites, circles, rectangles, lines with transformations
		Rethink of texture wrapping, clamping, repeating...
		updateVerticesPositions: 1.0 vs 0.0 z values
		Z-ordering
		After z-ordering check for particular batching
		Check for max texture units
		Shaders in Renderer - project path
		Check for loadFromFile, loadFromMemory, loadFromStream methods in case of cleanup of previous
		Add music streaming support
		Disable openAl internal logging
		Rethink of audio holder name

		Delta time fix

		Boolean init instead constructor?

		OpenAll, drawable tpp files?
		Rework event system
		Add GUI
		Add Lua script engine
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

	renderer.draw(*m_text);
	//renderer.draw(*m_fontText);

	//renderer.drawLine({ 100, 100 }, { 500, 100 }, orc::Color(255, 0, 0));
	//renderer.drawLine({ 100, 200 }, { 500, 300 }, orc::Color(255, 0, 0));

	//renderer.draw(*m_rectangle1);
	//renderer.draw(*m_rectangle2);
	//renderer.draw(*m_rectangle3);
	//renderer.draw(*m_rectangle4);
	//renderer.draw(*m_rectangle5);

	//renderer.draw(*m_sprite1);
	//renderer.draw(*m_sprite2);
	//renderer.draw(*m_sprite3);
	//renderer.draw(*m_sprite4);
	//renderer.draw(*m_emptySprite);

	//m_sprite1->setPosition(m_position);

	//renderer.draw(*m_player);
	//renderer.draw(*m_zombie);

	//renderer.draw(*m_circle1);
	//renderer.draw(*m_circle2);
	//renderer.draw(*m_circle3);

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
