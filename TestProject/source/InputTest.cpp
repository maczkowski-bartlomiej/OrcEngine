#include "InputTest.hpp"

InputTest::InputTest()
{
	ORC_LOG_INFO("InputTest init...");
	orc::Ref<orc::Texture> soldierTexture = textureHolder.getResource("soldier_texture");
	m_player.setTexture(soldierTexture);
	m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	m_player.setPosition(orc::Vector2f(400.0f, 300.0f));
}

InputTest::~InputTest()
{
	ORC_LOG_INFO("InputTest deinit...");
}

void InputTest::onAttach()
{
	ORC_LOG_INFO("Switching to InputTest");
	ORC_LOG_INFO("<- CirclesTest | Menu ->");
	window.setTitle("InputTest");
}

void InputTest::onDetach()
{
	ORC_LOG_INFO("Leaving from InputTest");
}

void InputTest::onUpdate(float deltaTime)
{
	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::A))
		m_player.move(-M_SPEED * deltaTime, 0.0f);
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::D))
		m_player.move(M_SPEED * deltaTime, 0.0f);

	if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::W))
		m_player.move(0.0f, -M_SPEED * deltaTime);
	else if (orc::Keyboard::isKeyPressed(orc::Keyboard::Key::S))
		m_player.move(0.0f, M_SPEED * deltaTime);

	orc::Vector2f mouse = orc::Mouse::getPosition();
	orc::Vector2f playerPos = m_player.getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player.setRotation(angle);
}

void InputTest::onEvent(const orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveLayer("CirclesTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveLayer("Menu"); break;
		}
	}
}

void InputTest::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);
	renderer.draw(m_player);
	renderer.end();
}

void InputTest::onGuiRender()
{
	ImGui::Begin("Player Control Help");
	{
		ImGui::Text("W - move up");
		ImGui::Text("S - move down");
		ImGui::Text("A - move left");
		ImGui::Text("D - move right");
	}
	ImGui::End();

	ImGui::Begin("Player info");
	{
		orc::Vector2f position = m_player.getPosition();
		float rotation = m_player.getRotation();

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
