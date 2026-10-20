#include "AudioTest.hpp"

AudioTest::AudioTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	ORC_LOG_INFO("AudioTest init.");
}

AudioTest::~AudioTest()
{
	ORC_LOG_INFO("AudioTest deinit.");
}

void AudioTest::onAttach()
{
	ORC_LOG_INFO("Switching to AudioTest,");
	ORC_LOG_INFO("<- AnimationTest | CameraTest ->");
	m_layerContext.window.setTitle("AudioTest");
}

void AudioTest::onDetach()
{
	ORC_LOG_INFO("Leaving from AudioTest,");
}

void AudioTest::onUpdate(float deltaTime)
{
}

void AudioTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));
	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void AudioTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		const auto& kbPressed = orc::getEvent<orc::KeyboardKeyPressedEvent>(event);
		switch (kbPressed.key)
		{
			case orc::Keyboard::Key::Left:
				m_layerContext.gameLayerManager.setActiveLayer("AnimationTest");
				break;
			case orc::Keyboard::Key::Right:
				m_layerContext.gameLayerManager.setActiveLayer("CameraTest");
				break;
			default:
				break;
		}
	}
}

void AudioTest::onGuiRender()
{
	ImGui::Begin("Audio testing");
	{
		if (ImGui::Button("SFX 1 (OneShot)", {200, 50}))
			m_layerContext.audio.playOneShot("event:/SFX/sfx1");
		if (ImGui::Button("SFX 2 (OneShot)", {200, 50}))
			m_layerContext.audio.playOneShot("event:/SFX/sfx2");
		if (ImGui::Button("SFX 3 (OneShot)", {200, 50}))
			m_layerContext.audio.playOneShot("event:/SFX/sfx3");
		if (ImGui::Button("Music 1 (Handle)", {200, 50}))
			m_musicHandle = m_layerContext.audio.play("event:/Music/music1");
		if (ImGui::Button("Music 2 (Handle)", {200, 50}))
			m_musicHandle = m_layerContext.audio.play("event:/Music/music2");
		if (ImGui::Button("Music 3 (Handle)", {200, 50}))
			m_musicHandle = m_layerContext.audio.play("event:/Music/music3");
	}
	ImGui::End();

	ImGui::Begin("Active Music Track (SoundHandle)");
	{
		bool isPlaying = m_musicHandle.isValid();
		ImGui::Text("Track Status: %s", isPlaying ? "Active" : "Inactive / Stopped");

		if (isPlaying)
		{
			if (ImGui::Button("Pause Track", {150, 40}))
				m_musicHandle.pause();
			ImGui::SameLine();
			if (ImGui::Button("Resume Track", {150, 40}))
				m_musicHandle.resume();
			if (ImGui::Button("Stop Track", {150, 40}))
				m_musicHandle.stop();

			static float trackVolume = 1.0f;
			if (ImGui::SliderFloat("Track Volume", &trackVolume, 0.0f, 1.0f))
				m_musicHandle.setVolume(trackVolume);

			static float trackPitch = 1.0f;
			if (ImGui::SliderFloat("Track Pitch", &trackPitch, 0.5f, 2.0f))
				m_musicHandle.setPitch(trackPitch);
		}
		else
		{
			ImGui::TextDisabled("Start a music track above to control it via SoundHandle.");
		}
	}
	ImGui::End();

	ImGui::Begin("SFX bus control");
	{
		auto sfxBus = m_layerContext.audio.getBus(orc::Bus::SFX);
		if (ImGui::Button("Pause", {200, 50}))
			sfxBus.pause();
		if (ImGui::Button("Resume", {200, 50}))
			sfxBus.resume();
		if (ImGui::Button("Stop", {200, 50}))
			sfxBus.stop();
		float volume = sfxBus.getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			sfxBus.setVolume(volume);
		ImGui::Text("Volume = %f", sfxBus.getVolume());
		ImGui::Text("Is paused = %s", sfxBus.isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Music bus control");
	{
		auto musicBus = m_layerContext.audio.getBus(orc::Bus::Music);
		if (ImGui::Button("Pause", {200, 50}))
			musicBus.pause();
		if (ImGui::Button("Resume", {200, 50}))
			musicBus.resume();
		if (ImGui::Button("Stop", {200, 50}))
			musicBus.stop();
		float volume = musicBus.getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			musicBus.setVolume(volume);

		ImGui::Text("Volume = %f", musicBus.getVolume());
		ImGui::Text("Is paused = %s", musicBus.isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Master bus control");
	{
		auto masterBus = m_layerContext.audio.getBus(orc::Bus::Master);
		if (ImGui::Button("Pause", {200, 50}))
			masterBus.pause();
		if (ImGui::Button("Resume", {200, 50}))
			masterBus.resume();
		if (ImGui::Button("Stop", {200, 50}))
			masterBus.stop();
		float volume = masterBus.getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			masterBus.setVolume(volume);

		ImGui::Text("Volume = %f", masterBus.getVolume());
		ImGui::Text("Is paused = %s", masterBus.isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("AnimationTest");
		if (ImGui::Button("AudioTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("AudioTest");
		if (ImGui::Button("CameraTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("CameraTest");
		if (ImGui::Button("CirclesTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("CirclesTest");
		if (ImGui::Button("Menu", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("Menu");
		if (ImGui::Button("InputTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("InputTest");
		if (ImGui::Button("RectanglesTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("RectanglesTest");
		if (ImGui::Button("SpritesTest", {200, 50}))
			m_layerContext.gameLayerManager.setActiveLayer("SpritesTest");
	}
	ImGui::End();
}
