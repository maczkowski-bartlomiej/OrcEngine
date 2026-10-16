#include "Menu.hpp"

#include "AnimationTest.hpp"
#include "AudioTest.hpp"
#include "CameraTest.hpp"
#include "CirclesTest.hpp"
#include "InputTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"

Menu::Menu(orc::LayerContext& context)
	: orc::GameLayer(context)
{
	ORC_LOG_INFO("Menu init.");

	m_context.gameLayerManager.addLayer("AnimationTest", orc::createRef<AnimationTest>(m_context));
	m_context.gameLayerManager.addLayer("AudioTest", orc::createRef<AudioTest>(m_context));
	m_context.gameLayerManager.addLayer("CameraTest", orc::createRef<CameraTest>(m_context));
	m_context.gameLayerManager.addLayer("CirclesTest", orc::createRef<CirclesTest>(m_context));
	m_context.gameLayerManager.addLayer("InputTest", orc::createRef<InputTest>(m_context));
	m_context.gameLayerManager.addLayer("RectanglesTest", orc::createRef<RectanglesTest>(m_context));
	m_context.gameLayerManager.addLayer("SpritesTest", orc::createRef<SpritesTest>(m_context));
}

Menu::~Menu()
{
	ORC_LOG_INFO("Menu deinit.");
}

void Menu::onAttach()
{
	ORC_LOG_INFO("Switching to Menu.");
	ORC_LOG_INFO("<- InputTest | RectanglesTest ->");
	m_context.window.setTitle("Menu");
}

void Menu::onDetach()
{
	ORC_LOG_INFO("Leaving from Menu.");
}

void Menu::onUpdate(const orc::FrameContext& ctx)
{

}

void Menu::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (const auto* kbPressed = event.getIf<orc::KeyboardKeyPressedEvent>())
	{
		switch (kbPressed->key)
		{
			case orc::Keyboard::Key::Left: m_context.gameLayerManager.setActiveLayer("InputTest"); break;
			case orc::Keyboard::Key::Right: m_context.gameLayerManager.setActiveLayer("RectanglesTest"); break;
			default: break;
		}
	}
}

void Menu::onRender(orc::Renderer& renderer)
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.beginScene(camera);
	renderer.endScene();
}

void Menu::onGuiRender()
{
	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("AnimationTest");
		if (ImGui::Button("AudioTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("AudioTest");
		if (ImGui::Button("CameraTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("CameraTest");
		if (ImGui::Button("CirclesTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("CirclesTest");
		if (ImGui::Button("Menu", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("Menu");
		if (ImGui::Button("InputTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("InputTest");
		if (ImGui::Button("RectanglesTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("RectanglesTest");
		if (ImGui::Button("SpritesTest", { 200, 50 }))
			m_context.gameLayerManager.setActiveLayer("SpritesTest");
	}
	ImGui::End();
}
