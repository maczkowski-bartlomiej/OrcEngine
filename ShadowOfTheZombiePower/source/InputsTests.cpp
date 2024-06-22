#include "InputsTest.hpp"

InputsTest::InputsTest()
{
	orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	orc::Ref<orc::Texture> soldierTexture = textureHolder.getResource("soldier_texture");
	orc::Ref<orc::Font> font = fontHolder.getResource("arial_font");

	m_player.setTexture(soldierTexture);
	m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	m_player.setPosition(orc::Vector2f(400.0f, 300.0f));
	
	m_sprite2.setTexture(zombieTexture);
	m_sprite2.setOrigin(m_sprite2.getGlobalRect().getSize() / 2.0f);
	m_sprite2.setPosition(orc::Vector2f(400.0f, 300.0f));

	m_sprite3.setTexture(soldierTexture);
	m_sprite3.setColor(orc::Color(255, 0, 0, 255));
	m_sprite3.setOrigin(m_sprite3.getGlobalRect().getSize() / 2.0f);
	m_sprite3.setPosition(orc::Vector2f(400.0f, 300.0f));

	m_sprite3.setScale(0.5f);

	m_text1.setFont(font);
	m_text1.setString("Na dole");
	m_text1.setPosition(orc::Vector2f(310.0f, 220.0f));

	m_text2.setFont(font);
	m_text2.setString("Na gorze\nOJ NA GORZE");
	m_text2.setPosition(orc::Vector2f(250.0f, 200.0f));
	m_text2.setColor(orc::Color(255, 0, 0, 255));

	m_rectangle1 = orc::Rectangle(orc::Vector2f(400.0f, 200.0f), orc::Vector2f(175.0f, 175.0f), orc::Color(0, 0, 255));
	m_rectangle1.setOrigin(m_rectangle1.getSize() / 2.0f);
}

InputsTest::~InputsTest()
{
	ORC_LOG_INFO("InputsTest shutting down...");
}

void InputsTest::onAttach()
{
	window.setTitle("InputsTest");

//	audio.playMusic(soundBufferHolder.getResource("music"), {0, 0});
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

	orc::Vector2f mouse = orc::Mouse::getPosition();
	orc::Vector2f playerPos = m_player.getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player.setRotation(angle);


	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.draw(m_text1);
	renderer.draw(m_player);
	renderer.draw(m_sprite2);
	renderer.draw(m_rectangle1);
	renderer.draw(m_sprite3);
	renderer.draw(m_text2);

	renderer.drawLine(orc::Vector2f(0.0f, 0.0f), orc::Vector2f(100.0f, 100.0f), orc::Color(255, 0, 0));
	renderer.drawLine(orc::Vector2f(0.0f, 0.0f), orc::Vector2f(800.0f, 600.0f), orc::Color(255, 0, 0));
	renderer.end();
}

void InputsTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& newEvent = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (newEvent.key)
		{
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("game"); break;
			case orc::Keyboard::Key::Left:  gameLayerManager.setActiveGameLayer("sprites_test"); break;
			case orc::Keyboard::Key::Q:     camera.rotate(0.1f); break;
			case orc::Keyboard::Key::E:     camera.rotate(-0.1f); break;
			case orc::Keyboard::Key::I:     camera.move(0.0f, -10.0f); break;
			case orc::Keyboard::Key::K:     camera.move(0.0f, 10.0f); break;
			case orc::Keyboard::Key::J:     camera.move(-10.0f, 0.0f); break;
			case orc::Keyboard::Key::L:     camera.move(10.0f, 0.0f); break;
		}

		if (newEvent.key == orc::Keyboard::Key::A)
		{
			camera.test(x);
			x++;
		}
		else if (newEvent.key == orc::Keyboard::Key::D)
		{
			camera.test(x);
			x--;
		}

		ORC_LOG_INFO("X: {}", x);
	}
	else if (event.getType() == orc::Event::Type::MouseWheelScrolled)
	{
		auto& newEvent = orc::getEvent<orc::MouseWheelScrolledEvent>(event);
		if (newEvent.yDelta > 0)
		{
			m_player.scale(0.1f);
		}
		else if (newEvent.yDelta < 0)
		{
			m_player.scale(-0.1f);
		}
		m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	}
}

