#include "OrcPch.hpp"
#include "Engine/Core.hpp"
#include "Engine/Clock.hpp"
#include "Engine/Engine.hpp"
#include "Events/WindowEvents.hpp"

#include <vector>
#include <string>
#include <functional>
#include <Audio/Audio.hpp>
#include <Engine/Config.hpp>
#include <Engine/Debug.hpp>
#include <Engine/GameLayer.hpp>
#include <Engine/GameLayerManager.hpp>
#include <Engine/Logger.hpp>
#include <Engine/ResourceHolder.hpp>
#include <Events/Event.hpp>
#include <Graphics/FTLibrary.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Window.hpp>

namespace orc {

	Engine* Engine::m_instance = nullptr;

	Engine::Engine(const Config& config)
	{
		if (m_instance)
		{
			ORC_FATAL("Engine already initialized.");
		}

		m_instance = this;

		Logger::init(config.logPath);

		ORC_LOG_INFO("Starting Orc Engine v.{}.{}.{}", version::MAJOR_VERSION, version::MINOR_VERSION, version::PATCH_VERSION);

		m_gameLayerManager = createUniquePtr<GameLayerManager>();

		m_ftLibary = createUniquePtr<FTLibrary>();

		m_window = createUniquePtr<Window>(config.videoSettings);
		m_window->setEventCallback(std::bind(&Engine::onEvent, this, std::placeholders::_1));

		m_renderer = createUniquePtr<Renderer>();

		m_gui = createUniquePtr<Gui>();

		m_audio = createUniquePtr<Audio>(config.audioSettings);

		m_fontResources.loadResources(config.fontsPath);
		m_textureResources.loadResources(config.texturesPath);
		m_animationResources.loadResources(config.animationsPath);
	}

	Engine::~Engine()
	{
		if (m_instance != this) return;

		m_gameLayerManager->clear();

		m_animationResources.clear();
		m_textureResources.clear();
		m_fontResources.clear();

		m_audio.reset();
		m_gui.reset();
		m_renderer.reset();
		m_window.reset();
		m_ftLibary.reset();
		m_gameLayerManager.reset();

		Logger::deinit();
		m_instance = nullptr;
	}

	void Engine::run()
	{
		Clock clock;

		m_running = true;
		while (m_running)
		{
			float elapsed = clock.restart();

			Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveLayer();
			if (gameLayer)
			{
				gameLayer->onUpdate(elapsed);

				m_renderer->begin(gameLayer->getCamera());
				gameLayer->onRender();
				m_renderer->end();

				m_gui->begin();
				gameLayer->onGuiRender();
				m_gui->end();
			}

			m_audio->update();
			m_window->display();
		}
	}

	Engine& Engine::get()
	{
		return *m_instance;
	}

	FontResources& Engine::getFontResources()
	{
		return m_fontResources;
	}

	Audio& Engine::getAudio()
	{
		return *m_audio;
	}

	Window& Engine::getWindow()
	{
		return *m_window;
	}

	Renderer& Engine::getRenderer()
	{
		return *m_renderer;
	}

	GameLayerManager& Engine::getGameLayerManager()
	{
		return *m_gameLayerManager;
	}

	FTLibrary& Engine::getFTLibary()
	{
		return *m_ftLibary;
	}

	TextureResources& Engine::getTextureResources()
	{
		return m_textureResources;
	}

	AnimationResources& Engine::getAnimationResources()
	{
		return m_animationResources;
	}

	void Engine::onEvent(const Event& event)
	{
		Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveLayer();
		if (gameLayer)
		{
			gameLayer->onEvent(event);
		}

		event.visit(
			[this, &event](const WindowClosedEvent&)
			{
				m_running = false;
				event.setHandled();
			},
			[this, &gameLayer, &event](const WindowResizedEvent& windowResizedEvent)
			{
				if (gameLayer)
				{
					gameLayer->getCamera().setViewportSize(0.0f, static_cast<float>(windowResizedEvent.width), static_cast<float>(windowResizedEvent.height), 0.0f);
				}
				event.setHandled();
			},
			[&event](auto&&)
			{
				event.setHandled();
			}
		);
	}

}
