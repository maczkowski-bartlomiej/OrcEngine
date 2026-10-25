#include "InputTest.hpp"

InputTest::InputTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("InputTest init.");
	orc::Ref<orc::Texture> soldierTexture = m_layerContext.textureResources.getResource("soldier_texture");
	m_player.setTexture(soldierTexture);
	m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	m_player.setPosition(orc::Vector2f(400.0f, 300.0f));
}

InputTest::~InputTest()
{
	ORC_LOG_INFO("InputTest deinit.");
}

void InputTest::onAttach()
{
	ORC_LOG_INFO("Switching to InputTest.");
	ORC_LOG_INFO("<- CirclesTest | Menu ->");
	m_layerContext.window.setTitle("InputTest");
}

void InputTest::onDetach()
{
	ORC_LOG_INFO("Leaving from InputTest.");
}

void InputTest::onUpdate(float deltaTime)
{
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::A))
	{
		m_player.move(-SPEED * deltaTime, 0.0f);
	}
	else if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::D))
	{
		m_player.move(SPEED * deltaTime, 0.0f);
	}

	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::W))
	{
		m_player.move(0.0f, -SPEED * deltaTime);
	}
	else if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::S))
	{
		m_player.move(0.0f, SPEED * deltaTime);
	}

	orc::Vector2f mouse = m_layerContext.input.getMousePosition();
	orc::Vector2f playerPos = m_player.getPosition();
	orc::Vector2f delta(mouse.x - playerPos.x, mouse.y - playerPos.y);
	float angle = glm::degrees(glm::atan(delta.y, delta.x));
	m_player.setRotation(angle);
}

void InputTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		const auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left:
				m_layerContext.gameLayerManager.setActiveLayer("CirclesTest");
				break;
			case orc::Keyboard::Key::Right:
				m_layerContext.gameLayerManager.setActiveLayer("Menu");
				break;
			default:
				break;
		}
	}
}

void InputTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));

	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.draw(m_player);
	m_layerContext.renderer.endScene();
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
		if (ImGui::Button("AnimationTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("AnimationTest");
		}
		if (ImGui::Button("AudioTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("AudioTest");
		}
		if (ImGui::Button("CameraTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("CameraTest");
		}
		if (ImGui::Button("CirclesTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("CirclesTest");
		}
		if (ImGui::Button("Menu", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("Menu");
		}
		if (ImGui::Button("InputTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("InputTest");
		}
		if (ImGui::Button("RectanglesTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("RectanglesTest");
		}
		if (ImGui::Button("SpritesTest", {200, 50}))
		{
			m_layerContext.gameLayerManager.setActiveLayer("SpritesTest");
		}
	}
	ImGui::End();
}
