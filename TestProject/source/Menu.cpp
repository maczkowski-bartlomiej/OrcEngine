#include "Menu.hpp"

#include "AnimationTest.hpp"
#include "AudioTest.hpp"
#include "CameraTest.hpp"
#include "CirclesTest.hpp"
#include "InputTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"

Menu::Menu(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("Menu init.");

	m_layerContext.gameLayerManager.addLayer("AnimationTest", orc::createRef<AnimationTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("AudioTest", orc::createRef<AudioTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("CameraTest", orc::createRef<CameraTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("CirclesTest", orc::createRef<CirclesTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("InputTest", orc::createRef<InputTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("RectanglesTest", orc::createRef<RectanglesTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("SpritesTest", orc::createRef<SpritesTest>(m_layerContext));
}

Menu::~Menu()
{
	ORC_LOG_INFO("Menu deinit.");
}

void Menu::onAttach()
{
	ORC_LOG_INFO("Switching to Menu.");
	ORC_LOG_INFO("<- InputTest | RectanglesTest ->");
	m_layerContext.window.setTitle("Menu");
}

void Menu::onDetach()
{
	ORC_LOG_INFO("Leaving from Menu.");
}

void Menu::onUpdate(float deltaTime)
{

}

void Menu::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		const auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: m_layerContext.gameLayerManager.setActiveLayer("InputTest"); break;
			case orc::Keyboard::Key::Right: m_layerContext.gameLayerManager.setActiveLayer("RectanglesTest"); break;
			default: break;
		}
	}
}

void Menu::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));
	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void Menu::onGuiRender()
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
