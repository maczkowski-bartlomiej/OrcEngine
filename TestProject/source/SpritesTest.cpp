#include "SpritesTest.hpp"

SpritesTest::SpritesTest()
{
	orc::Ref<orc::Texture> gradientTexture = textureHolder.getResource("gradient_texture");
	orc::Ref<orc::Texture> smallTexture = textureHolder.getResource("small_texture");
	orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	orc::Ref<orc::Texture> soldierTexture = textureHolder.getResource("soldier_texture");
	orc::Ref<orc::Font> font = fontHolder.getResource("arial_font");

	m_text1 = orc::Text(font, "Soldier Texture"); m_text1.setPosition(100.0f, 0.0f); m_text1.setScale(0.25f, 0.25f); m_text1.setOrigin(m_text1.getGlobalRect().getSize() / 2.0f);
	m_sprite1 = orc::Sprite(soldierTexture, orc::Vector2f(100.0f, 75.0f));
	m_sprite1.setScale(0.5f);
	m_sprite1.setOrigin(m_sprite1.getGlobalRect().getSize() / 2.0f);

	m_text2 = orc::Text(font, "Zombie Texture"); m_text2.setPosition(300.0f, 0.0f); m_text2.setScale(0.25f, 0.25f); m_text2.setOrigin(m_text2.getGlobalRect().getSize() / 2.0f);
	m_sprite2 = orc::Sprite(zombieTexture, orc::Vector2f(300.0f, 75.0f));
	m_sprite2.setScale(0.5f);
	m_sprite2.setOrigin(m_sprite2.getGlobalRect().getSize() / 2.0f);

	m_text3 = orc::Text(font, "Soldier + Rotated 45"); m_text3.setPosition(500.0f, 0.0f); m_text3.setScale(0.25f, 0.25f); m_text3.setOrigin(m_text3.getGlobalRect().getSize() / 2.0f);
	m_sprite3 = orc::Sprite(soldierTexture, orc::Vector2f(500.0f, 75.0f));
	m_sprite3.setScale(0.5f);
	m_sprite3.setOrigin(m_sprite3.getGlobalRect().getSize() / 2.0f);
	m_sprite3.setRotation(45.0f);

	m_text4 = orc::Text(font, "Soldier + Rotating"); m_text4.setPosition(700.0f, 0.0f); m_text4.setScale(0.25f, 0.25f); m_text4.setOrigin(m_text4.getGlobalRect().getSize() / 2.0f);
	m_sprite4 = orc::Sprite(soldierTexture, orc::Vector2f(700.0f, 75.0f));
	m_sprite4.setScale(0.5f);
	m_sprite4.setOrigin(m_sprite4.getGlobalRect().getSize() / 2.0f);

	m_text5 = orc::Text(font, "Soldier + Red color"); m_text5.setPosition(100.0f, 150.0f); m_text5.setScale(0.25f, 0.25f); m_text5.setOrigin(m_text5.getGlobalRect().getSize() / 2.0f);
	m_sprite5 = orc::Sprite(soldierTexture, orc::Vector2f(100.0f, 225.0f));
	m_sprite5.setScale(0.5f);
	m_sprite5.setOrigin(m_sprite5.getGlobalRect().getSize() / 2.0f);
	m_sprite5.setColor(orc::Color(255, 0, 0));

	m_text6 = orc::Text(font, "Zombie + negative 0.5 scaled"); m_text6.setPosition(300.0f, 150.0f); m_text6.setScale(0.25f, 0.25f); m_text6.setOrigin(m_text6.getGlobalRect().getSize() / 2.0f);
	m_sprite6 = orc::Sprite(zombieTexture, orc::Vector2f(300.0f, 225.0f));
	m_sprite6.setScale(-0.5f);
	m_sprite6.setOrigin(m_sprite6.getGlobalRect().getSize() / 2.0f);

	//m_text8 = orc::Text(font, "Green + Textured + Red Border"); m_text8.setPosition(700.0f, 150.0f); m_text8.setScale(0.25f, 0.25f); m_text8.setOrigin(m_text8.getGlobalRect().getSize() / 2.0f);
	//m_circle8 = orc::Circle(orc::Vector2f(700.0f, 225.0f), 35.0f, orc::Color(0, 255, 0));
	//m_circle8.setOrigin(m_circle8.getGlobalRect().getSize() / 2.0f);
	//m_circle8.setTexture(gradientTexture);
	//m_circle8.setBorderThickness(0.2f); m_circle8.setBorderColor(orc::Color(255, 0, 0));

	//m_text9 = orc::Text(font, "Green + Max Red border"); m_text9.setPosition(100.0f, 300.0f); m_text9.setScale(0.25f, 0.25f); m_text9.setOrigin(m_text9.getGlobalRect().getSize() / 2.0f);
	//m_circle9 = orc::Circle(orc::Vector2f(100.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
	//m_circle9.setOrigin(m_circle9.getGlobalRect().getSize() / 2.0f);
	//m_circle9.setBorderThickness(20.0f); m_circle9.setBorderColor(orc::Color(255, 0, 0));

	//m_text10 = orc::Text(font, "Green + Negative Red border"); m_text10.setPosition(300.0f, 300.0f); m_text10.setScale(0.25f, 0.25f); m_text10.setOrigin(m_text10.getGlobalRect().getSize() / 2.0f);
	//m_circle10 = orc::Circle(orc::Vector2f(300.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
	//m_circle10.setOrigin(m_circle10.getGlobalRect().getSize() / 2.0f);
	//m_circle10.setBorderThickness(-0.2f); m_circle10.setBorderColor(orc::Color(255, 0, 0));
	
	/*
		Textured + scaled + texture rect
		Textured + texture wrapping
	*/

}

SpritesTest::~SpritesTest()
{
	ORC_LOG_INFO("SpritesTest shutting down...");
}

void SpritesTest::onAttach()
{
	window.setTitle("SpritesTest");
}

void SpritesTest::onDetach()
{
}

void SpritesTest::onUpdate(float deltaTime)
{
	m_sprite4.rotate(45.0f * deltaTime);

	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.draw(m_sprite1);
	renderer.draw(m_sprite2);
	renderer.draw(m_sprite3);
	renderer.draw(m_sprite4);
	renderer.draw(m_sprite5);
	renderer.draw(m_sprite6);
	//renderer.draw(m_sprite7);
	//renderer.draw(m_sprite8);
	//renderer.draw(m_sprite9);
	//renderer.draw(m_sprite10);
	//renderer.draw(m_sprite11);
	//renderer.draw(m_sprite12);

	renderer.draw(m_text1);
	renderer.draw(m_text2);
	renderer.draw(m_text3);
	renderer.draw(m_text4);
	renderer.draw(m_text5);
	renderer.draw(m_text6);
	//renderer.draw(m_text7);
	//renderer.draw(m_text8);
	//renderer.draw(m_text9);
	//renderer.draw(m_text10);
	//renderer.draw(m_text11);
	//renderer.draw(m_text12);

	drawLines();

	renderer.end();
}

void SpritesTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("inputs_test");
		}
		else if (newEvent->key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("circles_test");
		}
	}
}

void SpritesTest::drawLines()
{
	renderer.drawLine(orc::Vector2f(200.0f, 0.0f), orc::Vector2f(200.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(400.0f, 0.0f), orc::Vector2f(400.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(600.0f, 0.0f), orc::Vector2f(600.0f, 600.0f), orc::Color(255));

	renderer.drawLine(orc::Vector2f(0.0f, 150.0f), orc::Vector2f(800.0f, 150.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 300.0f), orc::Vector2f(800.0f, 300.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 450.0f), orc::Vector2f(800.0f, 450.0f), orc::Color(255));
}
