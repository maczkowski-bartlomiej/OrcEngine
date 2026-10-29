#include "AudioTest.hpp"

#include "DemoUtilities.hpp"

AudioTest::AudioTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	m_buses[Buses::Master] = m_layerContext.audio.getBus("bus:/");
	m_buses[Buses::Sfx] = m_layerContext.audio.getBus("bus:/SFX");
	m_buses[Buses::Music] = m_layerContext.audio.getBus("bus:/Music");
}

AudioTest::~AudioTest()
{
}

void AudioTest::onAttach()
{
	ORC_LOG_INFO("Switching to AudioTest.");
	m_layerContext.window.setTitle("AudioTest");
}

void AudioTest::onDetach()
{
}

void AudioTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "AnimationTest", "CameraTest");
}

void AudioTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));
	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void AudioTest::onGuiRender()
{
	ImGui::Begin("Audio testing");
	{
		if (ImGui::Button("SFX 1", {200, 50}))
		{
			m_layerContext.audio.play("event:/SFX/sfx1");
		}
		if (ImGui::Button("SFX 2", {200, 50}))
		{
			m_layerContext.audio.play("event:/SFX/sfx2");
		}
		if (ImGui::Button("SFX 3", {200, 50}))
		{
			m_layerContext.audio.play("event:/SFX/sfx3");
		}
		if (ImGui::Button("Music 1", {200, 50}))
		{
			m_layerContext.audio.play("event:/Music/music1");
		}
		if (ImGui::Button("Music 2", {200, 50}))
		{
			m_layerContext.audio.play("event:/Music/music2");
		}
		if (ImGui::Button("Music 3", {200, 50}))
		{
			m_layerContext.audio.play("event:/Music/music3");
		}
		if (ImGui::Button("Play not existing event", {200, 50}))
		{
			m_layerContext.audio.play("bad_event");
		}
	}
	ImGui::End();

	ImGui::Begin("SFX bus control");
	{
		if (ImGui::Button("Pause", {200, 50}))
		{
			m_buses[Buses::Sfx]->pause();
		}
		if (ImGui::Button("Resume", {200, 50}))
		{
			m_buses[Buses::Sfx]->resume();
		}
		if (ImGui::Button("Stop", {200, 50}))
		{
			m_buses[Buses::Sfx]->stop();
		}
		float volume = m_buses[Buses::Sfx]->getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
		{
			m_buses[Buses::Sfx]->setVolume(volume);
		}
		ImGui::Text("Volume = %f", m_buses[Buses::Sfx]->getVolume());
		ImGui::Text("Is paused = %s", m_buses[Buses::Sfx]->isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Music bus control");
	{
		if (ImGui::Button("Pause", {200, 50}))
		{
			m_buses[Buses::Music]->pause();
		}
		if (ImGui::Button("Resume", {200, 50}))
		{
			m_buses[Buses::Music]->resume();
		}
		if (ImGui::Button("Stop", {200, 50}))
		{
			m_buses[Buses::Music]->stop();
		}
		float volume = m_buses[Buses::Music]->getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
		{
			m_buses[Buses::Music]->setVolume(volume);
		}

		ImGui::Text("Volume = %f", m_buses[Buses::Music]->getVolume());
		ImGui::Text("Is paused = %s", m_buses[Buses::Music]->isPaused() ? "True" : "False");
	}
	ImGui::End();

	ImGui::Begin("Master bus control");
	{
		if (ImGui::Button("Pause", {200, 50}))
		{
			m_buses[Buses::Master]->pause();
		}
		if (ImGui::Button("Resume", {200, 50}))
		{
			m_buses[Buses::Master]->resume();
		}
		if (ImGui::Button("Stop", {200, 50}))
		{
			m_buses[Buses::Master]->stop();
		}
		float volume = m_buses[Buses::Master]->getVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
		{
			m_buses[Buses::Master]->setVolume(volume);
		}

		ImGui::Text("Volume = %f", m_buses[Buses::Master]->getVolume());
		ImGui::Text("Is paused = %s", m_buses[Buses::Master]->isPaused() ? "True" : "False");
	}
	ImGui::End();

	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
