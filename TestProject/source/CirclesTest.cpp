#include "CirclesTest.hpp"

CirclesTest::CirclesTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("CirclesTest init.");

	orc::Ref<orc::Texture> gradientTexture = m_layerContext.textureResources.getResource("gradient_texture");
	orc::Ref<orc::Texture> smallTexture = m_layerContext.textureResources.getResource("small_texture");
	orc::Ref<orc::Font> font = m_layerContext.fontResources.getResource("arial_font");

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
	ORC_LOG_INFO("CirclesTest deinit.");
}

void CirclesTest::onAttach()
{
	ORC_LOG_INFO("Switching to CirclesTest.");
	ORC_LOG_INFO("<- CameraTest | InputTest ->");
	m_layerContext.window.setTitle("CirclesTest");
}

void CirclesTest::onDetach()
{
	ORC_LOG_INFO("Leaving from CirclesTest.");
}

void CirclesTest::onUpdate(float deltaTime)
{
	m_circle11.rotate(45.0f * deltaTime);
}

void CirclesTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (const auto* kbPressed = event.getIf<orc::KeyboardKeyPressedEvent>())
	{
		switch (kbPressed->key)
		{
			case orc::Keyboard::Key::Left: m_layerContext.gameLayerManager.setActiveLayer("CameraTest"); break;
			case orc::Keyboard::Key::Right: m_layerContext.gameLayerManager.setActiveLayer("InputTest"); break;
			default: break;
		}
	}
}

void CirclesTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));

	m_layerContext.renderer.beginScene(m_camera);

	m_layerContext.renderer.draw(m_circle1);
	m_layerContext.renderer.draw(m_circle2);
	m_layerContext.renderer.draw(m_circle3);
	m_layerContext.renderer.draw(m_circle4);
	m_layerContext.renderer.draw(m_circle5);
	m_layerContext.renderer.draw(m_circle6);
	m_layerContext.renderer.draw(m_circle7);
	m_layerContext.renderer.draw(m_circle8);
	m_layerContext.renderer.draw(m_circle9);
	m_layerContext.renderer.draw(m_circle10);
	m_layerContext.renderer.draw(m_circle11);
	m_layerContext.renderer.draw(m_circle12);

	m_layerContext.renderer.draw(m_text1);
	m_layerContext.renderer.draw(m_text2);
	m_layerContext.renderer.draw(m_text3);
	m_layerContext.renderer.draw(m_text4);
	m_layerContext.renderer.draw(m_text5);
	m_layerContext.renderer.draw(m_text6);
	m_layerContext.renderer.draw(m_text7);
	m_layerContext.renderer.draw(m_text8);
	m_layerContext.renderer.draw(m_text9);
	m_layerContext.renderer.draw(m_text10);
	m_layerContext.renderer.draw(m_text11);
	//m_layerContext.renderer.draw(m_text12);

	drawLines();

	m_layerContext.renderer.endScene();
}

void CirclesTest::onGuiRender()
{
	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("AnimationTest");
		if (ImGui::Button("AudioTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("AudioTest");
		if (ImGui::Button("CameraTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("CameraTest");
		if (ImGui::Button("CirclesTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("CirclesTest");
		if (ImGui::Button("Menu", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("Menu");
		if (ImGui::Button("InputTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("InputTest");
		if (ImGui::Button("RectanglesTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("RectanglesTest");
		if (ImGui::Button("SpritesTest", { 200, 50 }))
			m_layerContext.gameLayerManager.setActiveLayer("SpritesTest");
	}
	ImGui::End();
}

void CirclesTest::drawLines()
{
	m_layerContext.renderer.drawLine(orc::Vector2f(200.0f, 0.0f), orc::Vector2f(200.0f, 600.0f), orc::Color(255));
	m_layerContext.renderer.drawLine(orc::Vector2f(400.0f, 0.0f), orc::Vector2f(400.0f, 600.0f), orc::Color(255));
	m_layerContext.renderer.drawLine(orc::Vector2f(600.0f, 0.0f), orc::Vector2f(600.0f, 600.0f), orc::Color(255));

	m_layerContext.renderer.drawLine(orc::Vector2f(0.0f, 150.0f), orc::Vector2f(800.0f, 150.0f), orc::Color(255));
	m_layerContext.renderer.drawLine(orc::Vector2f(0.0f, 300.0f), orc::Vector2f(800.0f, 300.0f), orc::Color(255));
	m_layerContext.renderer.drawLine(orc::Vector2f(0.0f, 450.0f), orc::Vector2f(800.0f, 450.0f), orc::Color(255));
}