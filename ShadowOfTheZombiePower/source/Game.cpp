#include "Game.hpp"

#include "CirclesTest.hpp"
#include "RectanglesTest.hpp"

Game::Game()
{
	//orc::Ref<orc::Texture> playerTexture = textureHolder.getResource("soldier_texture");
	//m_player = orc::createRef<orc::Sprite>(playerTexture);

	//orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	//m_zombie = orc::createRef<orc::Sprite>(zombieTexture);

	//m_zombie->setRotation(0.0f);
	//m_zombie->setPosition(500, 400);
	//m_zombie->setColor(orc::Color(1, 1, 1, 1));

	//m_player->setRotation(0.0f);
	//m_player->setPosition(0.0f, 0.0f);
	////m_player->setOrigin(m_player->getGlobalRect().getSize() / 2.0f);
	//m_player->setScale(0.3f, 0.3f);
	//m_player->setTexture(playerTexture);

	//m_emptySprite = orc::createRef<orc::Sprite>();
	//m_emptySprite->setPosition(orc::Vector2f(500, 600));

	//m_sprite1 = orc::createRef<orc::Sprite>(playerTexture);
	//m_sprite1->setOrigin(m_sprite1->getTexture()->getSize() / 2.0f);
	//m_sprite2 = orc::createRef<orc::Sprite>(playerTexture);
	//m_sprite3 = orc::createRef<orc::Sprite>(playerTexture);
	//m_sprite4 = orc::createRef<orc::Sprite>(playerTexture);

	//m_audioBuffer = soundBufferHolder.getResource("example_sound");

	/*
		Texture loading modes
		Text special characters handling \n \t

		Rework texture wrapping, clamping, repeating...

		updateVerticesPositions: 1.0 vs 0.0 z values

		Z-ordering
		After z-ordering check for particular type batching

		Shaders in Renderer - use project path, move to Engine folder
		Check all loadFromFile, loadFromMemory, loadFromStream methods in case of invalid cleanup
		Add music streaming support

		Disable openAl internal logging
		Rethink of AudioHolder name
		Delta time fix

		Boolean init of all systems instead constructor? - Rethink it

		Move implementation of OpenAll.hpp, Texturable.hpp to tpp files

		Rework event system to avoid dynamic casting
		
		Add GUI
		Add Lua script engine
	*/

	gameLayerManager.addGameLayer("circles_test", orc::createRef<CirclesTest>());
	gameLayerManager.addGameLayer("rectangles_test", orc::createRef<RectanglesTest>());
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
	//float speed = 50.0f;
	//if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))
	//{
	//	m_position.x -= speed * deltaTime;
	//}
	//else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))
	//{
	//	m_position.x += speed * deltaTime;
	//}

	//if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))
	//{
	//	m_position.y -= speed * deltaTime;
	//}
	//else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))
	//{
	//	m_position.y += speed * deltaTime;
	//}

	//if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Left))
	//{
	//	m_cameraPosition.x += speed * deltaTime;
	//}
	//else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Right))
	//{
	//	m_cameraPosition.x -= speed * deltaTime;
	//}

	//if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Up))
	//{
	//	m_cameraPosition.y += speed * deltaTime;
	//}
	//else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Down))
	//{
	//	m_cameraPosition.y -= speed * deltaTime;
	//}

	//m_player->setPosition(m_position);

	//orc::Vector2f mouse = orc::Mouse::getMousePosition();
	//orc::Vector2f playerPos = m_player->getPosition();
	//orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	//float angle = glm::degrees(glm::atan(delta.y, delta.x));
	//m_sprite1->setRotation(angle);

	//renderer.end();

	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();
	renderer.begin(camera);
	renderer.end();
}

void Game::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			//audio.playSound(m_audioBuffer, { 0, 0 });
			gameLayerManager.setActiveGameLayer("rectangles_test");
		}
	}
}
