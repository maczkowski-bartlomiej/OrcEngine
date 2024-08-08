#include "CirclesTest.hpp"

CirclesTest::CirclesTest()
{
	orc::Ref<orc::Texture> gradientTexture = textureHolder.getResource("gradient_texture");
	orc::Ref<orc::Texture> smallTexture = textureHolder.getResource("small_texture");
	orc::Ref<orc::Font> font = fontHolder.getResource("arial_font");

	m_text1 = orc::Text(font, "Blue Color"); m_text1.setPosition(100.0f, 0.0f); m_text1.setScale(0.25f, 0.25f); m_text1.setOrigin(m_text1.getGlobalRect().getSize() / 2.0f);
	m_circle1 = orc::Circle(orc::Vector2f(100.0f, 75.0f), 35.0f, orc::Color(0, 0, 255));
	m_circle1.setOrigin(m_circle1.getGlobalRect().getSize() / 2.0f);

	m_text2 = orc::Text(font, "Textured"); m_text2.setPosition(300.0f, 0.0f); m_text2.setScale(0.25f, 0.25f); m_text2.setOrigin(m_text2.getGlobalRect().getSize() / 2.0f);
	m_circle2 = orc::Circle(orc::Vector2f(300.0f, 75.0f), 35.0f);
	m_circle2.setOrigin(m_circle2.getGlobalRect().getSize() / 2.0f);
	m_circle2.setTexture(gradientTexture);

	m_text3 = orc::Text(font, "Textured + Green Color"); m_text3.setPosition(500.0f, 0.0f); m_text3.setScale(0.25f, 0.25f); m_text3.setOrigin(m_text3.getGlobalRect().getSize() / 2.0f);
	m_circle3 = orc::Circle(orc::Vector2f(500.0f, 75.0f), 35.0f, orc::Color(0, 255, 0));
	m_circle3.setOrigin(m_circle3.getGlobalRect().getSize() / 2.0f);
	m_circle3.setTexture(gradientTexture);

	m_text4 = orc::Text(font, "White + Negative Radius"); m_text4.setPosition(700.0f, 0.0f); m_text4.setScale(0.25f, 0.25f); m_text4.setOrigin(m_text4.getGlobalRect().getSize() / 2.0f);
	m_circle4 = orc::Circle(orc::Vector2f(700.0f, 75.0f), -35.0f, orc::Color(255, 255, 255));
	m_circle4.setOrigin(m_circle4.getGlobalRect().getSize() / 2.0f);

	m_text5 = orc::Text(font, "Textured + Inner Radius"); m_text5.setPosition(100.0f, 150.0f); m_text5.setScale(0.25f, 0.25f); m_text5.setOrigin(m_text5.getGlobalRect().getSize() / 2.0f);
	m_circle5 = orc::Circle(orc::Vector2f(100.0f, 225.0f), 35.0f, orc::Color(255, 255, 255, 255));
	m_circle5.setOrigin(m_circle5.getGlobalRect().getSize() / 2.0f);
	m_circle5.setInnerRadius(0.3f);
	m_circle5.setTexture(gradientTexture);

	m_text6 = orc::Text(font, "Red + Inner Radius Max"); m_text6.setPosition(300.0f, 150.0f); m_text6.setScale(0.25f, 0.25f); m_text6.setOrigin(m_text6.getGlobalRect().getSize() / 2.0f);
	m_circle6 = orc::Circle(orc::Vector2f(300.0f, 225.0f), 35.0f, orc::Color(255, 0, 0));
	m_circle6.setOrigin(m_circle6.getGlobalRect().getSize() / 2.0f);
	m_circle6.setInnerRadius(2.0f);

	m_text7 = orc::Text(font, "Red + Negative Inner Radius"); m_text7.setPosition(500.0f, 150.0f); m_text7.setScale(0.25f, 0.25f); m_text7.setOrigin(m_text7.getGlobalRect().getSize() / 2.0f);
	m_circle7 = orc::Circle(orc::Vector2f(500.0f, 225.0f), 35.0f, orc::Color(255, 0, 0));
	m_circle7.setOrigin(m_circle7.getGlobalRect().getSize() / 2.0f);
	m_circle7.setInnerRadius(-0.2f);

	m_text8 = orc::Text(font, "Green + Textured + Red Border"); m_text8.setPosition(700.0f, 150.0f); m_text8.setScale(0.25f, 0.25f); m_text8.setOrigin(m_text8.getGlobalRect().getSize() / 2.0f);
	m_circle8 = orc::Circle(orc::Vector2f(700.0f, 225.0f), 35.0f, orc::Color(0, 255, 0));
	m_circle8.setOrigin(m_circle8.getGlobalRect().getSize() / 2.0f);
	m_circle8.setTexture(gradientTexture);
	m_circle8.setBorderThickness(0.2f); m_circle8.setBorderColor(orc::Color(255, 0, 0));

	m_text9 = orc::Text(font, "Green + Max Red border"); m_text9.setPosition(100.0f, 300.0f); m_text9.setScale(0.25f, 0.25f); m_text9.setOrigin(m_text9.getGlobalRect().getSize() / 2.0f);
	m_circle9 = orc::Circle(orc::Vector2f(100.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
	m_circle9.setOrigin(m_circle9.getGlobalRect().getSize() / 2.0f);
	m_circle9.setBorderThickness(20.0f); m_circle9.setBorderColor(orc::Color(255, 0, 0));

	m_text10 = orc::Text(font, "Green + Negative Red border"); m_text10.setPosition(300.0f, 300.0f); m_text10.setScale(0.25f, 0.25f); m_text10.setOrigin(m_text10.getGlobalRect().getSize() / 2.0f);
	m_circle10 = orc::Circle(orc::Vector2f(300.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
	m_circle10.setOrigin(m_circle10.getGlobalRect().getSize() / 2.0f);
	m_circle10.setBorderThickness(-0.2f); m_circle10.setBorderColor(orc::Color(255, 0, 0));

	m_text11 = orc::Text(font, "Textured + Border + Rotating"); m_text11.setPosition(500.0f, 300.0f); m_text11.setScale(0.25f, 0.25f); m_text11.setOrigin(m_text11.getGlobalRect().getSize() / 2.0f);
	m_circle11 = orc::Circle(orc::Vector2f(500.0f, 375.0f), 35.0f, orc::Color(255, 255, 255));
	m_circle11.setBorderThickness(0.05f);
	m_circle11.setTexture(gradientTexture);
	m_circle11.setOrigin(m_circle11.getGlobalRect().getSize() / 2.0f);

	/*
		Textured + scaled + texture rect
		Textured + texture wrapping
	*/

}

CirclesTest::~CirclesTest()
{
	ORC_LOG_INFO("CirclesTest shutting down...");
}

void CirclesTest::onAttach()
{
	window.setTitle("CirclesTest");
}

void CirclesTest::onDetach()
{
}

void CirclesTest::onUpdate(float deltaTime)
{
	m_circle11.rotate(45.0f * deltaTime);

	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.draw(m_circle1);
	renderer.draw(m_circle2);
	renderer.draw(m_circle3);
	renderer.draw(m_circle4);
	renderer.draw(m_circle5);
	renderer.draw(m_circle6);
	renderer.draw(m_circle7);
	renderer.draw(m_circle8);
	renderer.draw(m_circle9);
	renderer.draw(m_circle10);
	renderer.draw(m_circle11);
	renderer.draw(m_circle12);

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
	//renderer.draw(m_text12);

	drawLines();

	renderer.end();
}

void CirclesTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("sprites_test");
		}
		else if (newEvent->key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("rectangles_test");
		}
	}
}

void CirclesTest::drawLines()
{
	renderer.drawLine(orc::Vector2f(200.0f, 0.0f), orc::Vector2f(200.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(400.0f, 0.0f), orc::Vector2f(400.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(600.0f, 0.0f), orc::Vector2f(600.0f, 600.0f), orc::Color(255));

	renderer.drawLine(orc::Vector2f(0.0f, 150.0f), orc::Vector2f(800.0f, 150.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 300.0f), orc::Vector2f(800.0f, 300.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 450.0f), orc::Vector2f(800.0f, 450.0f), orc::Color(255));
}
