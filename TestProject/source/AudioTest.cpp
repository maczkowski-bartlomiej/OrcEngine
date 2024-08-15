#include "AudioTest.hpp"

AudioTest::AudioTest()
{
	ORC_LOG_INFO("AudioTest init...");
}

AudioTest::~AudioTest()
{
	ORC_LOG_INFO("AudioTest deinit...");
}

void AudioTest::onAttach()
{
	ORC_LOG_INFO("Switching to AudioTest");
	ORC_LOG_INFO("<- AnimationTest | CameraTest ->");
	window.setTitle("AudioTest");
}

void AudioTest::onDetach()
{
	ORC_LOG_INFO("Leaving from AudioTest");
}

void AudioTest::onUpdate(float deltaTime)
{
}

void AudioTest::onRender()
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();
	renderer.begin(camera);
	renderer.end();
}

void AudioTest::onEvent(const orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
<<<<<<< ours
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveLayer("AnimationTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveLayer("CameraTest"); break;
||||||| ancestor
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("game"); break;
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveGameLayer("inputs_test"); break;
=======
			case orc::Keyboard::Key::Left: gameLayerManager.setActiveGameLayer("AnimationTest"); break;
			case orc::Keyboard::Key::Right: gameLayerManager.setActiveGameLayer("CameraTest"); break;
>>>>>>> theirs
		}
	}
}

void AudioTest::onGuiRender()
{
	ImGui::Begin("Audio testing");
	{
		if (ImGui::Button("SFX 1", { 200, 50 }))
			audio.play("event:/SFX/sfx1");
		if (ImGui::Button("SFX 2", { 200, 50 }))
			audio.play("event:/SFX/sfx2");
		if (ImGui::Button("SFX 3", { 200, 50 }))
			audio.play("event:/SFX/sfx3");
		if (ImGui::Button("Music 1", { 200, 50 }))
			audio.play("event:/Music/music1");
		if (ImGui::Button("Music 2", { 200, 50 }))
			audio.play("event:/Music/music2");
		if (ImGui::Button("Music 3", { 200, 50 }))
			audio.play("event:/Music/music3");
		if (ImGui::Button("Load wrong bank", { 200, 50 }))
			audio.loadBank("wrong_bank.bank");
		if (ImGui::Button("Play not existing event", { 200, 50 }))
			audio.play("bad_event");
	}
	ImGui::End();

	ImGui::Begin("SFX bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			audio.getSfxBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			audio.getSfxBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			audio.getSfxBus().stop();
		float volume = audio.getSfxBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			audio.getSfxBus().setVolume(volume);
		ImGui::Text("Volume = %f", audio.getSfxBus().getVolume());
		ImGui::Text("Is paused = %s", audio.getSfxBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Music bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			audio.getMusicBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			audio.getMusicBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			audio.getMusicBus().stop();
		float volume = audio.getMusicBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			audio.getMusicBus().setVolume(volume);

		ImGui::Text("Volume = %f", audio.getMusicBus().getVolume());
		ImGui::Text("Is paused = %s", audio.getMusicBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Master bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			audio.getMasterBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			audio.getMasterBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			audio.getMasterBus().stop();
		float volume = audio.getMasterBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			audio.getMasterBus().setVolume(volume);

		ImGui::Text("Volume = %f", audio.getMasterBus().getVolume());
		ImGui::Text("Is paused = %s", audio.getMasterBus().isPaused() ? "True" : "False");
	}
	ImGui::End();
<<<<<<< ours

	ImGui::Begin("Navigation Menu");
	{
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
	}
	ImGui::End();
||||||| ancestor
=======

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
>>>>>>> theirs
}
