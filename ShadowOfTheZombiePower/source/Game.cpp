#include "Game.hpp"

#include "CirclesTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"
#include "InputsTest.hpp"

Game::Game()
{
	/*
		Animated sprites
		Add Lua script engine
		Music queing

		std::vector<FloatRect> frames;
		Sprite sprite;
										    time 
		sprite.addAnimation("idle", frames, 0.5f);
		
		sprite.playAnimation("idle", true);
		sprite.playAnimation("hit", false);


		sprite.update();
	*/
	
	auto anim = animationHolder.getResource("player_animation");
	anim;
	gameLayerManager.addGameLayer("circles_test", orc::createRef<CirclesTest>());
	gameLayerManager.addGameLayer("rectangles_test", orc::createRef<RectanglesTest>());
	gameLayerManager.addGameLayer("sprites_test", orc::createRef<SpritesTest>());
	gameLayerManager.addGameLayer("inputs_test", orc::createRef<InputsTest>());
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
			gameLayerManager.setActiveGameLayer("inputs_test");
		}
	}
}

void Game::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	//renderer.drawLine(orc::Vector2f(0.0f, 0.0f), orc::Vector2f(100.0f, 100.0f), orc::Color(255, 0, 0));
}

void Game::onGuiRender()
{
	//ImGui::ShowDemoWindow();
}
