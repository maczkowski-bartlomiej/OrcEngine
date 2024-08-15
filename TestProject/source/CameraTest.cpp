#include "CameraTest.hpp"

CameraTest::CameraTest()
{
	ORC_LOG_INFO("CameraTest init...");
	
	orc::Ref<orc::Texture> zombieTexture = textureHolder.getResource("zombie_texture");
	m_sprite = orc::Sprite(zombieTexture, orc::Vector2f(400.0f, 300.0f));
	m_sprite.setOrigin(m_sprite.getGlobalRect().getSize() / 2.0f);
}

CameraTest::~CameraTest()
{
	ORC_LOG_INFO("CameraTest deinit...");
}

void CameraTest::onAttach()
{
	ORC_LOG_INFO("Switching to CameraTest");
	ORC_LOG_INFO("<- AudioTest | CirclesTest ->");
	window.setTitle("CameraTest");
}

void CameraTest::onDetach()
{
	ORC_LOG_INFO("Leaving from CameraTest");
}

void CameraTest::onUpdate(float deltaTime)
{
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::Q))     camera.rotate(0.1f);
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::E))     camera.rotate(-0.1f);
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))     camera.move(0.0f, -10.0f);
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))     camera.move(0.0f, 10.0f);
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))     camera.move(-10.0f, 0.0f);
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))     camera.move(10.0f, 0.0f);
}

void CameraTest::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();
	renderer.begin(camera);
	renderer.draw(m_sprite);
	renderer.end();
}

void CameraTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveLayer("AudioTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveLayer("CirclesTest"); break;
		}
	}
	else if (event.getType() == orc::Event::Type::MouseWheelScrolled)
	{
		auto& wheelScrolled = orc::getEvent<orc::MouseWheelScrolledEvent>(event);
		if (wheelScrolled.yDelta > 0)
			m_sprite.scale(0.1f);
		else if (wheelScrolled.yDelta < 0)
			m_sprite.scale(-0.1f);

		m_sprite.setOrigin(m_sprite.getGlobalRect().getSize() / 2.0f);
	}
}

void CameraTest::onGuiRender()
{
	ImGui::Begin("Camera control help");
	{
		ImGui::Text("Q - rotate counterclockwise");
		ImGui::Text("Q - rotate clockwise");
		ImGui::Text("W - move up");
		ImGui::Text("S - move down");
		ImGui::Text("A - move left");
		ImGui::Text("D - move right");
	}
	ImGui::End();

	ImGui::Begin("Camera info");
	{
		orc::Vector2f position = camera.getPosition();
		float rotation = camera.getRotation();

		ImGui::Text("x = %f", position.x);
		ImGui::Text("y = %f", position.y);
		ImGui::Text("rotation - %f", rotation);
	}
	ImGui::End();

	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("AnimationTest");
		if (ImGui::Button("AudioTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("AudioTest");
		if (ImGui::Button("CameraTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("CameraTest");
		if (ImGui::Button("CirclesTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("CirclesTest");
		if (ImGui::Button("Menu", { 200, 50 }))
			gameLayerManager.setActiveLayer("Menu");
		if (ImGui::Button("InputTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("InputTest");
		if (ImGui::Button("RectanglesTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("RectanglesTest");
		if (ImGui::Button("SpritesTest", { 200, 50 }))
			gameLayerManager.setActiveLayer("SpritesTest");
	}
	ImGui::End();
}
