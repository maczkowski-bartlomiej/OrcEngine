#include "AnimationTest.hpp"

AnimationTest::AnimationTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("AnimationTest init.");
}

AnimationTest::~AnimationTest()
{
	ORC_LOG_INFO("AnimationTest deinit.");
}

void AnimationTest::onAttach()
{
	ORC_LOG_INFO("Switching to AnimationTest");
	ORC_LOG_INFO("<- SpritesTest | AudioTest ->");
	m_layerContext.window.setTitle("AnimationTest");
}

void AnimationTest::onDetach()
{
	ORC_LOG_INFO("Leaving from AnimationTest.");
}

void AnimationTest::onUpdate(float deltaTime)
{

}

void AnimationTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (const auto* kbPressed = event.getIf<orc::KeyboardKeyPressedEvent>())
	{
		switch (kbPressed->key)
		{
			case orc::Keyboard::Key::Left: m_layerContext.gameLayerManager.setActiveLayer("SpritesTest"); break;
			case orc::Keyboard::Key::Right: m_layerContext.gameLayerManager.setActiveLayer("AudioTest"); break;
			default: break;
		}
	}
}

void AnimationTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));

	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void AnimationTest::onGuiRender()
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
