#include "OrcPch.hpp"
#include "Engine/Core.hpp"
#include "Engine/Clock.hpp"
#include "Engine/Engine.hpp"
#include "Events/WindowEvents.hpp"

#include <vector>
#include <string>

namespace orc {

Engine* Engine::m_instance = nullptr;

bool Engine::init(const GameSettings& gameSettings)
{
	if (m_instance)
	{
		ORC_FATAL("Engine already initialized");
		return false;
	}

	m_running = true;
	m_instance = this;

	if (!Logger::init(gameSettings.logPath))
		return false;

	ORC_LOG_INFO("Starting...");
	ORC_LOG_INFO("Orc Engine v.{}.{}.{}", version::MAJOR_VERSION, version::MINOR_VERSION, version::PATCH_VERSION);

	if (!m_ftLibary.init()) return false;
	if (!m_window.init(gameSettings.videoSettings)) return false;
	m_window.setEventCallback(std::bind(&Engine::onEvent, this, std::placeholders::_1));

	if (!m_renderer.init()) return false;
	if (!m_gui.init()) return false;

	std::vector<std::string> banks; //Temporary
	banks.push_back("assets/audio/Master.bank");
	banks.push_back("assets/audio/Master.strings.bank");
	banks.push_back("assets/audio/Music.bank");
	banks.push_back("assets/audio/SFX.bank");
	if (!m_audio.init(gameSettings.audioSettings, banks)) return false;

	m_fontHolder.loadResources(gameSettings.fontsPath);
	m_textureHolder.loadResources(gameSettings.texturesPath);
	m_animationHolder.loadResources(gameSettings.animationsPath);

	return true;
}

void Engine::deinit()
{
	if (m_instance != this) return;

	m_gameLayerManager.clear();
	m_animationHolder.clear();
	m_textureHolder.clear();
	m_fontHolder.clear();

	m_audio.deinit();
	m_gui.deinit();
	m_renderer.deinit();
	m_window.deinit();
	m_ftLibary.deinit();
	Logger::deinit();
}

void Engine::run()
{
	Clock clock;
	while (m_running)
	{
		float elapsed = clock.elapsed();
		clock.reset();

		Ref<GameLayer> gameLayer = m_gameLayerManager.getActiveLayer();
		gameLayer->onUpdate(elapsed);

		m_renderer.begin(gameLayer->getCamera());
		gameLayer->onRender();
		m_renderer.end();

		m_gui.begin();
		gameLayer->onGuiRender();
		m_gui.end();

		m_audio.update();
		m_window.display();

		//ORC_LOG_INFO("FPS: {}", 1.0f / elapsed);
	}
}

Engine& Engine::get()
{
	return *m_instance;
}

FontHolder& Engine::getFontHolder()
{
	return m_fontHolder;
}

Audio& Engine::getAudio()
{
	return m_audio;
}

Window& Engine::getWindow()
{
	return m_window;
}

Renderer& Engine::getRenderer()
{
	return m_renderer;
}

GameLayerManager& Engine::getGameLayerManager()
{
	return m_gameLayerManager;
}

FTLibrary& Engine::getFTLibary()
{
	return m_ftLibary;
}

TextureHolder& Engine::getTextureHolder()
{
	return m_textureHolder;
}

AnimationHolder& Engine::getAnimationHolder()
{
	return m_animationHolder;
}

void Engine::onEvent(const Event& event) 
{
	m_gameLayerManager.getActiveLayer()->onEvent(event);

	if (event.getType() == Event::Type::WindowClosed)
	{
		m_running = false;
	}
	else if (event.getType() == Event::Type::WindowResized)
	{
		const WindowResizedEvent& windowResizedEvent = getEvent<WindowResizedEvent>(event);
		Ref<GameLayer> gameLayer = m_gameLayerManager.getActiveLayer();
		if (gameLayer)
		{
			gameLayer->getCamera().setViewportSize(0.0f, static_cast<float>(windowResizedEvent.width), static_cast<float>(windowResizedEvent.height), 0.0f);
		}
	}
}

}
