#include "AnimationTest.hpp"

AnimationTest::AnimationTest()
{
	ORC_LOG_INFO("AnimationTest init...");
}

AnimationTest::~AnimationTest()
{
	ORC_LOG_INFO("AnimationTest deinit...");
}

void AnimationTest::onAttach()
{
	ORC_LOG_INFO("Switching to AnimationTest");
	ORC_LOG_INFO("<- SpritesTest | AudioTest ->");
	window.setTitle("AnimationTest");
}

void AnimationTest::onDetach()
{
	ORC_LOG_INFO("Leaving from AnimationTest");
}

void AnimationTest::onUpdate(float deltaTime)
{
<<<<<<< ours

}

void AnimationTest::onEvent(const orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveLayer("SpritesTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveLayer("AudioTest"); break;
		}
	}
}

void AnimationTest::onRender()
{
||||||| ancestor
=======

}

void AnimationTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveGameLayer("SpritesTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("AudioTest"); break;
		}
	}
}

void AnimationTest::onRender()
{
>>>>>>> theirs
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.end();
	
}

void AnimationTest::onGuiRender()
{
	ImGui::Begin("Navigation Menu");
	{
<<<<<<< ours
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
||||||| ancestor
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("circles_test");
		}
		else if (newEvent->key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("game");
		}
=======
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
>>>>>>> theirs
	}
	ImGui::End();
}
