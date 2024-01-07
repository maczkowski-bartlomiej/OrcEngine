#include "SpritesTest.hpp"

SpritesTest::SpritesTest()
{
	orc::Ref<orc::Texture> gradientTexture = textureHolder.getResource("gradient_texture");
	orc::Ref<orc::Texture> smallTexture = textureHolder.getResource("small_texture");
	orc::Ref<orc::Font> font = fontHolder.getResource("arial_font");

	/*m_text1 = orc::Text(font, "Blue Color"); m_text1.setPosition(100.0f, 0.0f); m_text1.setScale(0.25f, 0.25f); m_text1.setOrigin(m_text1.getGlobalRect().getSize() / 2.0f);
	m_rectangle1 = orc::Rectangle(orc::Vector2f(100.0f, 75.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 255));
	m_rectangle1.setOrigin(m_rectangle1.getSize() / 2.0f);

	m_text2 = orc::Text(font, "Textured"); m_text2.setPosition(300.0f, 0.0f); m_text2.setScale(0.25f, 0.25f); m_text2.setOrigin(m_text2.getGlobalRect().getSize() / 2.0f);
	m_rectangle2 = orc::Rectangle(orc::Vector2f(300.0f, 75.0f), orc::Vector2f(75.0f, 75.0f));
	m_rectangle2.setOrigin(m_rectangle2.getSize() / 2.0f);
	m_rectangle2.setTexture(gradientTexture);

	m_text3 = orc::Text(font, "Textured + Green Color"); m_text3.setPosition(500.0f, 0.0f); m_text3.setScale(0.25f, 0.25f); m_text3.setOrigin(m_text3.getGlobalRect().getSize() / 2.0f);
	m_rectangle3 = orc::Rectangle(orc::Vector2f(500.0f, 75.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
	m_rectangle3.setOrigin(m_rectangle3.getSize() / 2.0f);
	m_rectangle3.setTexture(gradientTexture);

	m_text4 = orc::Text(font, "Textured + Rotating"); m_text4.setPosition(700.0f, 0.0f); m_text4.setScale(0.25f, 0.25f); m_text4.setOrigin(m_text4.getGlobalRect().getSize() / 2.0f);
	m_rectangle4 = orc::Rectangle(orc::Vector2f(700.0f, 75.0f), orc::Vector2f(75.0f, 75.0f));
	m_rectangle4.setOrigin(m_rectangle4.getSize() / 2.0f);
	m_rectangle4.setTexture(gradientTexture);

	m_text5 = orc::Text(font, "Blue Opacity + Default Border"); m_text5.setPosition(100.0f, 150.0f); m_text5.setScale(0.25f, 0.25f); m_text5.setOrigin(m_text5.getGlobalRect().getSize() / 2.0f);
	m_rectangle5 = orc::Rectangle(orc::Vector2f(100.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 255, 90));
	m_rectangle5.setOrigin(m_rectangle5.getSize() / 2.0f);
	m_rectangle5.setBorderThickness(0.05f);

	m_text6 = orc::Text(font, "Black + Negative Border"); m_text6.setPosition(300.0f, 150.0f); m_text6.setScale(0.25f, 0.25f); m_text6.setOrigin(m_text6.getGlobalRect().getSize() / 2.0f);
	m_rectangle6 = orc::Rectangle(orc::Vector2f(300.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 0));
	m_rectangle6.setOrigin(m_rectangle6.getSize() / 2.0f);
	m_rectangle6.setBorderThickness(0.05f);*/

	/*m_text7 = orc::Text(font, "Textured + Red Border"); m_text7.setPosition(500.0f, 150.0f); m_text7.setScale(0.25f, 0.25f); m_text7.setOrigin(m_text7.getGlobalRect().getSize() / 2.0f);
	m_rectangle7 = orc::Rectangle(orc::Vector2f(500.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 255, 255));
	m_rectangle7.setOrigin(m_rectangle7.getSize() / 2.0f);
	m_rectangle7.setTexture(gradientTexture);
	m_rectangle7.setBorderThickness(0.05f); m_rectangle7.setBorderColor(orc::Color(255, 0, 0));

	m_text8 = orc::Text(font, "Green + Textured + Blue Border"); m_text8.setPosition(700.0f, 150.0f); m_text8.setScale(0.25f, 0.25f); m_text8.setOrigin(m_text8.getGlobalRect().getSize() / 2.0f);
	m_rectangle8 = orc::Rectangle(orc::Vector2f(700.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
	m_rectangle8.setOrigin(m_rectangle8.getSize() / 2.0f);
	m_rectangle8.setTexture(gradientTexture);
	m_rectangle8.setBorderThickness(0.2f); m_rectangle8.setBorderColor(orc::Color(0, 0, 255));

	m_text9 = orc::Text(font, "Green + Red Border Max"); m_text9.setPosition(100.0f, 300.0f); m_text9.setScale(0.25f, 0.25f); m_text9.setOrigin(m_text9.getGlobalRect().getSize() / 2.0f);
	m_rectangle9 = orc::Rectangle(orc::Vector2f(100.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
	m_rectangle9.setOrigin(m_rectangle9.getSize() / 2.0f);
	m_rectangle9.setBorderThickness(20.0f); m_rectangle9.setBorderColor(orc::Color(255, 0, 0));

	m_text10 = orc::Text(font, "Pink + Border + 45 degrees"); m_text10.setPosition(300.0f, 300.0f); m_text10.setScale(0.25f, 0.25f); m_text10.setOrigin(m_text10.getGlobalRect().getSize() / 2.0f);
	m_rectangle10 = orc::Rectangle(orc::Vector2f(300.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 0, 255));
	m_rectangle10.setOrigin(m_rectangle10.getSize() / 2.0f);
	m_rectangle10.setBorderThickness(0.05f);
	m_rectangle10.setRotation(45.0f);

	m_text11 = orc::Text(font, "Yellow + Border + 0.5 scale"); m_text11.setPosition(500.0f, 300.0f); m_text11.setScale(0.25f, 0.25f); m_text11.setOrigin(m_text11.getGlobalRect().getSize() / 2.0f);
	m_rectangle11 = orc::Rectangle(orc::Vector2f(500.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 255, 0));
	m_rectangle11.setBorderThickness(0.05f);
	m_rectangle11.setScale(orc::Vector2f(0.5f, 0.5f));
	m_rectangle11.setOrigin(m_rectangle11.getGlobalRect().getSize() / 2.0f);

	m_text12 = orc::Text(font, "Red + 0.5 Negative Scaled"); m_text12.setPosition(700.0f, 300.0f); m_text12.setScale(0.25f, 0.25f); m_text12.setOrigin(m_text12.getGlobalRect().getSize() / 2.0f);
	m_rectangle12 = orc::Rectangle(orc::Vector2f(700.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 0, 0));
	m_rectangle12.setScale(orc::Vector2f(-0.5f, -0.5f));
	m_rectangle12.setOrigin(m_rectangle12.getGlobalRect().getSize() / 2.0f);*/
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
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.draw(m_sprite1);
	renderer.draw(m_sprite2);
	renderer.draw(m_sprite3);
	renderer.draw(m_sprite4);
	renderer.draw(m_sprite5);
	renderer.draw(m_sprite6);
	renderer.draw(m_sprite7);
	renderer.draw(m_sprite8);
	renderer.draw(m_sprite9);
	renderer.draw(m_sprite10);
	renderer.draw(m_sprite11);
	renderer.draw(m_sprite12);

	renderer.draw(m_text1);
	renderer.draw(m_text2);
	renderer.draw(m_text3);
	renderer.draw(m_text4);
	renderer.draw(m_text5);
	renderer.draw(m_text6);
	renderer.draw(m_text7);
	renderer.draw(m_text8);
	renderer.draw(m_text9);
	renderer.draw(m_text10);
	renderer.draw(m_text11);
	renderer.draw(m_text12);

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
