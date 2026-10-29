#include "InputTest.hpp"

#include "DemoUtilities.hpp"

InputTest::InputTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	orc::Ref<orc::Texture> soldierTexture = m_layerContext.textureResources.getResource("soldier_texture");
	m_player.setTexture(soldierTexture);
	m_player.setOrigin(m_player.getGlobalRect().getSize() / 2.0f);
	m_player.setPosition(orc::Vector2f(400.0f, 300.0f));
}

InputTest::~InputTest()
{
}

void InputTest::onAttach()
{
	ORC_LOG_INFO("Switching to InputTest.");
	m_layerContext.window.setTitle("InputTest");
}

void InputTest::onDetach()
{
}

void InputTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "CirclesTest", "Menu");

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

	ImGui::Begin("Input State Diagnostics");
	{
		ImGui::Text("--- Held Keys ---");
		ImGui::Text("W: %s | A: %s | S: %s | D: %s | Space: %s",
					m_layerContext.input.isKeyPressed(orc::Keyboard::Key::W) ? "Down" : "Up",
					m_layerContext.input.isKeyPressed(orc::Keyboard::Key::A) ? "Down" : "Up",
					m_layerContext.input.isKeyPressed(orc::Keyboard::Key::S) ? "Down" : "Up",
					m_layerContext.input.isKeyPressed(orc::Keyboard::Key::D) ? "Down" : "Up",
					m_layerContext.input.isKeyPressed(orc::Keyboard::Key::Space) ? "Down" : "Up");

		ImGui::Text("--- Key Transitions ---");
		ImGui::Text("Space JustPressed: %s | JustReleased: %s",
					m_layerContext.input.wasKeyJustPressed(orc::Keyboard::Key::Space) ? "True" : "False",
					m_layerContext.input.wasKeyJustReleased(orc::Keyboard::Key::Space) ? "True" : "False");
		ImGui::Text("Enter JustPressed: %s | JustReleased: %s",
					m_layerContext.input.wasKeyJustPressed(orc::Keyboard::Key::Enter) ? "True" : "False",
					m_layerContext.input.wasKeyJustReleased(orc::Keyboard::Key::Enter) ? "True" : "False");

		const auto& specialKeys = m_layerContext.input.getSpecialKeys();
		ImGui::Text("--- Modifiers (SpecialKeys) ---");
		ImGui::Text("Alt: %s | Shift: %s | Ctrl: %s | System: %s | CapsLock: %s | NumLock: %s",
					specialKeys.alt ? "True" : "False",
					specialKeys.shift ? "True" : "False",
					specialKeys.control ? "True" : "False",
					specialKeys.system ? "True" : "False",
					specialKeys.capsLock ? "True" : "False",
					specialKeys.numLock ? "True" : "False");

		ImGui::Text("--- Mouse Buttons ---");
		ImGui::Text("Left   - Held: %s | Pressed: %s | Released: %s",
					m_layerContext.input.isMouseButtonPressed(orc::Mouse::Button::Left) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustPressed(orc::Mouse::Button::Left) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustReleased(orc::Mouse::Button::Left) ? "True" : "False");
		ImGui::Text("Right  - Held: %s | Pressed: %s | Released: %s",
					m_layerContext.input.isMouseButtonPressed(orc::Mouse::Button::Right) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustPressed(orc::Mouse::Button::Right) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustReleased(orc::Mouse::Button::Right) ? "True" : "False");
		ImGui::Text("Scroll - Held: %s | Pressed: %s | Released: %s",
					m_layerContext.input.isMouseButtonPressed(orc::Mouse::Button::Scroll) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustPressed(orc::Mouse::Button::Scroll) ? "True" : "False",
					m_layerContext.input.wasMouseButtonJustReleased(orc::Mouse::Button::Scroll) ? "True" : "False");

		const auto& mousePos = m_layerContext.input.getMousePosition();
		const auto mouseDelta = m_layerContext.input.getMouseDelta();
		const auto& scrollDelta = m_layerContext.input.getMouseScrollDelta();

		ImGui::Text("--- Mouse Coordinates & Deltas ---");
		ImGui::Text("Position: (%.1f, %.1f)", mousePos.x, mousePos.y);
		ImGui::Text("Delta:    (%.1f, %.1f)", mouseDelta.x, mouseDelta.y);
		ImGui::Text("Scroll:   (%.1f, %.1f)", scrollDelta.x, scrollDelta.y);
	}
	ImGui::End();

	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
