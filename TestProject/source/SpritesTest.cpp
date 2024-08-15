#include "SpritesTest.hpp"

SpritesTest::SpritesTest()
{
	ORC_LOG_INFO("SpritesTest init...");

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
}

SpritesTest::~SpritesTest()
{
	ORC_LOG_INFO("SpritesTest deinit...");
}

void SpritesTest::onAttach()
{
	ORC_LOG_INFO("Switching to SpritesTest");
	ORC_LOG_INFO("<- RectanglesTest | AnimationTest ->");
	window.setTitle("SpritesTest");
}

void SpritesTest::onDetach()
{
	ORC_LOG_INFO("Leaving from SpritesTest");
}

void SpritesTest::onUpdate(float deltaTime)
{
	m_sprite4.rotate(45.0f * deltaTime);
}

void SpritesTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveGameLayer("RectanglesTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("AnimationTest"); break;
		}
	}
}

void SpritesTest::onRender()
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

void SpritesTest::onGuiRender()
{
	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("AnimationTest");
		if (ImGui::Button("AudioTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("AudioTest");
		if (ImGui::Button("CameraTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("CameraTest");
		if (ImGui::Button("CirclesTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("CirclesTest");
		if (ImGui::Button("Menu", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("Menu");
		if (ImGui::Button("InputTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("InputTest");
		if (ImGui::Button("RectanglesTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("RectanglesTest");
		if (ImGui::Button("SpritesTest", { 200, 50 }))
			gameLayerManager.setActiveGameLayer("SpritesTest");
	}
	ImGui::End();
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
