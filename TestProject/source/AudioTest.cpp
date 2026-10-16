#include "AudioTest.hpp"

AudioTest::AudioTest(orc::LayerContext& context)
	: orc::GameLayer(context)
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
	m_context.window.setTitle("AudioTest");
}

void AudioTest::onDetach()
{
	ORC_LOG_INFO("Leaving from AudioTest,");
}

void AudioTest::onUpdate(const orc::FrameContext& ctx)
{
}

void AudioTest::onRender(orc::Renderer& renderer)
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.beginScene(camera);
	renderer.endScene();
}

void AudioTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);

	if (const auto* kbPressed = event.getIf<orc::KeyboardKeyPressedEvent>())
	{
		switch (kbPressed->key)
		{
			case orc::Keyboard::Key::Left: m_context.gameLayerManager.setActiveLayer("AnimationTest"); break;
			case orc::Keyboard::Key::Right: m_context.gameLayerManager.setActiveLayer("CameraTest"); break;
			default: break;
		}
	}
}

void AudioTest::onGuiRender()
{
	ImGui::Begin("Audio testing");
	{
		if (ImGui::Button("SFX 1", { 200, 50 }))
			m_context.audio.play("event:/SFX/sfx1");
		if (ImGui::Button("SFX 2", { 200, 50 }))
			m_context.audio.play("event:/SFX/sfx2");
		if (ImGui::Button("SFX 3", { 200, 50 }))
			m_context.audio.play("event:/SFX/sfx3");
		if (ImGui::Button("Music 1", { 200, 50 }))
			m_context.audio.play("event:/Music/music1");
		if (ImGui::Button("Music 2", { 200, 50 }))
			m_context.audio.play("event:/Music/music2");
		if (ImGui::Button("Music 3", { 200, 50 }))
			m_context.audio.play("event:/Music/music3");
		if (ImGui::Button("Load wrong bank", { 200, 50 }))
			m_context.audio.loadBank("wrong_bank.bank");
		if (ImGui::Button("Play not existing event", { 200, 50 }))
			m_context.audio.play("bad_event");
	}
	ImGui::End();

	ImGui::Begin("SFX bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_context.audio.getSfxBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_context.audio.getSfxBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_context.audio.getSfxBus().stop();
		float volume = m_context.audio.getSfxBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_context.audio.getSfxBus().setVolume(volume);
		ImGui::Text("Volume = %f", m_context.audio.getSfxBus().getVolume());
		ImGui::Text("Is paused = %s", m_context.audio.getSfxBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Music bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_context.audio.getMusicBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_context.audio.getMusicBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_context.audio.getMusicBus().stop();
		float volume = m_context.audio.getMusicBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_context.audio.getMusicBus().setVolume(volume);

		ImGui::Text("Volume = %f", m_context.audio.getMusicBus().getVolume());
		ImGui::Text("Is paused = %s", m_context.audio.getMusicBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Master bus control");
	{
		if (ImGui::Button("Pause", { 200, 50 }))
			m_context.audio.getMasterBus().pause();
		if (ImGui::Button("Resume", { 200, 50 }))
			m_context.audio.getMasterBus().resume();
		if (ImGui::Button("Stop", { 200, 50 }))
			m_context.audio.getMasterBus().stop();
		float volume = m_context.audio.getMasterBus().getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
			m_context.audio.getMasterBus().setVolume(volume);

		ImGui::Text("Volume = %f", m_context.audio.getMasterBus().getVolume());
		ImGui::Text("Is paused = %s", m_context.audio.getMasterBus().isPaused() ? "True" : "False");
	}
	ImGui::End();

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
