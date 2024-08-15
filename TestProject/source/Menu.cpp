#include "Menu.hpp"

#include "AnimationTest.hpp"
#include "AudioTest.hpp"
#include "CameraTest.hpp"
#include "CirclesTest.hpp"
#include "InputTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"

Menu::Menu()
{
	ORC_LOG_INFO("Menu init...");

	gameLayerManager.addGameLayer("AnimationTest", orc::createRef<AnimationTest>());
	gameLayerManager.addGameLayer("AudioTest", orc::createRef<AudioTest>());
	gameLayerManager.addGameLayer("CameraTest", orc::createRef<CameraTest>());
	gameLayerManager.addGameLayer("CirclesTest", orc::createRef<CirclesTest>());
	gameLayerManager.addGameLayer("InputTest", orc::createRef<InputTest>());
	gameLayerManager.addGameLayer("RectanglesTest", orc::createRef<RectanglesTest>());
	gameLayerManager.addGameLayer("SpritesTest", orc::createRef<SpritesTest>());
}

Menu::~Menu()
{
	ORC_LOG_INFO("Menu deinit...");
}

void Menu::onAttach()
{
	ORC_LOG_INFO("Switching to Menu");
	ORC_LOG_INFO("<- InputTest | RectanglesTest ->");
	window.setTitle("Menu");
}

void Menu::onDetach()
{
	ORC_LOG_INFO("Leaving from Menu");
}

void Menu::onUpdate(float deltaTime)
{

}

void Menu::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveGameLayer("InputTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("RectanglesTest"); break;
		}
	}
}

void Menu::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();
	renderer.begin(camera);
	renderer.end();
}

void Menu::onGuiRender()
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
