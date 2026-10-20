#include "CameraTest.hpp"

CameraTest::CameraTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("CameraTest init.");

	orc::Ref<orc::Texture> zombieTexture = m_layerContext.textureResources.getResource("zombie_texture");
	m_sprite1 = orc::Sprite(zombieTexture, orc::Vector2f(400.0f, 300.0f));
	m_sprite1.setOrigin(m_sprite1.getGlobalRect().getSize() / 2.0f);

	m_sprite2 = orc::Sprite(zombieTexture, orc::Vector2f(700.0f, 500.0f));
	m_sprite2.setOrigin(m_sprite2.getGlobalRect().getSize() / 2.0f);
}

CameraTest::~CameraTest()
{
	ORC_LOG_INFO("CameraTest deinit.");
}

void CameraTest::onAttach()
{
	ORC_LOG_INFO("Switching to CameraTest.");
	ORC_LOG_INFO("<- AudioTest | CirclesTest ->");
	m_layerContext.window.setTitle("CameraTest");
}

void CameraTest::onDetach()
{
	ORC_LOG_INFO("Leaving from CameraTest.");
}

void CameraTest::onUpdate(float deltaTime)
{
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::Q)) m_camera.rotate(0.1f);
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::E)) m_camera.rotate(-0.1f);
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::W)) m_camera.move(0.0f, -10.0f);
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::S)) m_camera.move(0.0f, 10.0f);
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::A)) m_camera.move(-10.0f, 0.0f);
	if (m_layerContext.input.isKeyPressed(orc::Keyboard::Key::D)) m_camera.move(10.0f, 0.0f);
}

void CameraTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));
	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.draw(m_sprite1);
	m_layerContext.renderer.draw(m_sprite2);
	m_layerContext.renderer.endScene();
}

void CameraTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		const auto& e = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (e.key)
		{
			case orc::Keyboard::Key::Left: m_layerContext.gameLayerManager.setActiveLayer("AudioTest"); break;
			case orc::Keyboard::Key::Right: m_layerContext.gameLayerManager.setActiveLayer("CirclesTest"); break;
			default: break;
		}
	}
	else if (event.getType() == orc::Event::Type::MouseWheelScrolled)
	{
		const auto& wheelScrolled = orc::getEvent<orc::MouseWheelScrolledEvent>(event);
		if (wheelScrolled.yDelta > 0)
			m_camera.zoom(0.01f);
		else if (wheelScrolled.yDelta < 0)
			m_camera.zoom(-0.01f);
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
		ImGui::Text("Scroll - zoom in/zoom out");
	}
	ImGui::End();

	ImGui::Begin("Camera info");
	{
		orc::Vector2f position = m_camera.getPosition();
		float rotation = m_camera.getRotation();
		float zoom = m_camera.getZoom();

		ImGui::Text("x = %f", position.x);
		ImGui::Text("y = %f", position.y);
		ImGui::Text("zoom = %f", zoom);
		ImGui::Text("rotation = %f", rotation);
	}
	ImGui::End();

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
