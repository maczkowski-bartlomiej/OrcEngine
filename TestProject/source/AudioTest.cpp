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
			case orc::Keyboard::Key::Left: m_layerContext.gameLayerManager.setActiveLayer("AnimationTest"); break;
			case orc::Keyboard::Key::Right: m_layerContext.gameLayerManager.setActiveLayer("CameraTest"); break;
			default: break;
		}
	}
}

void AudioTest::onGuiRender()
{
	ImGui::Begin("Audio testing");
	{
		if (ImGui::Button("SFX 1", { 200, 50 }))
			m_layerContext.audio.play("event:/SFX/sfx1");
		if (ImGui::Button("SFX 2", { 200, 50 }))
			m_layerContext.audio.play("event:/SFX/sfx2");
		if (ImGui::Button("SFX 3", { 200, 50 }))
			m_layerContext.audio.play("event:/SFX/sfx3");
		if (ImGui::Button("Music 1", { 200, 50 }))
			m_layerContext.audio.play("event:/Music/music1");
		if (ImGui::Button("Music 2", { 200, 50 }))
			m_layerContext.audio.play("event:/Music/music2");
		if (ImGui::Button("Music 3", { 200, 50 }))
			m_layerContext.audio.play("event:/Music/music3");
		if (ImGui::Button("Load wrong bank", { 200, 50 }))
			m_layerContext.audio.loadBank("wrong_bank.bank");
		if (ImGui::Button("Play not existing event", { 200, 50 }))
			m_layerContext.audio.play("bad_event");
	}
	ImGui::End();

	ImGui::Begin("SFX bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_layerContext.audio.getSfxBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_layerContext.audio.getSfxBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_layerContext.audio.getSfxBus().stop();
		float volume = m_layerContext.audio.getSfxBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_layerContext.audio.getSfxBus().setVolume(volume);
		ImGui::Text("Volume = %f", m_layerContext.audio.getSfxBus().getVolume());
		ImGui::Text("Is paused = %s", m_layerContext.audio.getSfxBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Music bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_layerContext.audio.getMusicBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_layerContext.audio.getMusicBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_layerContext.audio.getMusicBus().stop();
		float volume = m_layerContext.audio.getMusicBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_layerContext.audio.getMusicBus().setVolume(volume);

		ImGui::Text("Volume = %f", m_layerContext.audio.getMusicBus().getVolume());
		ImGui::Text("Is paused = %s", m_layerContext.audio.getMusicBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Master bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_layerContext.audio.getMasterBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_layerContext.audio.getMasterBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_layerContext.audio.getMasterBus().stop();
		float volume = m_layerContext.audio.getMasterBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_layerContext.audio.getMasterBus().setVolume(volume);

		ImGui::Text("Volume = %f", m_layerContext.audio.getMasterBus().getVolume());
		ImGui::Text("Is paused = %s", m_layerContext.audio.getMasterBus().isPaused() ? "True" : "False");
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
