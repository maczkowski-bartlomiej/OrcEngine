#include "Game.hpp"

#include "CirclesTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"
#include "InputsTest.hpp"
#include "AnimationTest.hpp"
#include "AudioTest.hpp"

Game::Game()
{
	gameLayerManager.addGameLayer("circles_test", orc::createRef<CirclesTest>());
	gameLayerManager.addGameLayer("rectangles_test", orc::createRef<RectanglesTest>());
	gameLayerManager.addGameLayer("sprites_test", orc::createRef<SpritesTest>());
	gameLayerManager.addGameLayer("inputs_test", orc::createRef<InputsTest>());
	gameLayerManager.addGameLayer("audio_test", orc::createRef<AudioTest>());
}

Game::~Game()
{
	ORC_LOG_INFO("Game shutting down...");
}

void Game::onAttach()
{
	window.setTitle("Game");
}

void Game::onDetach()
{
}

void Game::onUpdate(float deltaTime)
{

}

void Game::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		if (newEvent.key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("rectangles_test");
		}
		else if (newEvent.key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("audio_test");
		}
	}
}

void Game::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();
}

void Game::onGuiRender()
{
	//ImGui::ShowDemoWindow();
}
