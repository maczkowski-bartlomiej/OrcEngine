#include "InputsTest.hpp"

InputsTest::InputsTest()
{
	orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	orc::Ref<orc::Texture> soldierTexture = textureHolder.getResource("soldier_texture");
	orc::Ref<orc::Font> font = fontHolder.getResource("arial_font");

	m_player.setTexture(soldierTexture);
	m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	m_player.setPosition(orc::Vector2f(400.0f, 300.0f));
}

InputsTest::~InputsTest()
{
	ORC_LOG_INFO("InputsTest shutting down...");
}

void InputsTest::onAttach()
{
	window.setTitle("InputsTest");
}

void InputsTest::onDetach()
{
}

void InputsTest::onUpdate(float deltaTime)
{
	float speed = 50.0f;
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))
	{
		m_player.move(-speed * deltaTime, 0.0f);
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))
	{
		m_player.move(speed * deltaTime, 0.0f);
	}

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))
	{
		m_player.move(0.0f, -speed * deltaTime);
	}
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))
	{
		m_player.move(0.0f, speed * deltaTime);
	}

	orc::Vector2f mouse = orc::Mouse::getMousePosition();
	orc::Vector2f playerPos = m_player.getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player.setRotation(angle);


	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.draw(m_player);

	renderer.end();
}

void InputsTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("game");
		}
		else if (newEvent->key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("sprites_test");
		}
	}
	else if (event.getType() == orc::Event::Type::MouseWheelScrolled)
	{
		auto newEvent = (orc::MouseWheelScrolledEvent*)&event;
		if (newEvent->yDelta > 0)
		{
			m_player.scale(0.1f);
		}
		else if (newEvent->yDelta < 0)
		{
			m_player.scale(-0.1f);
		}
		m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	}
}

