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

		return true;
	}

	void Engine::deinit()
	{
		if (m_instance != this) return;

		m_gameLayerManager->clear();

		m_animationResources.clear();
		m_textureResources.clear();
		m_fontResources.clear();

		m_audio.release();
		m_renderer.release();
		m_gui.release();
		m_window.release();

		m_ftLibary.release();


		Logger::deinit();
	}

	void Engine::run()
	{
		/*MonoDomain* domain;

		mono_config_parse(NULL);
		//mono_set_dirs("lib", "etc");
		mono_set_assemblies_path("lib");
		domain = mono_jit_init("GameDomain");
		MonoAssembly* assembly = mono_domain_assembly_open(domain, "GameScripts.dll");
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* playerClass = mono_class_from_name(image, "Game", "Player");

		MonoObject* playerObject = mono_object_new(domain, playerClass);
		mono_runtime_object_init(playerObject);*/

		//MonoMethod* updateMethod =
		//	mono_class_get_method_from_name(
		//		playerClass,
		//		"Update",
		//		0
		//	);

		//mono_runtime_invoke(
		//	updateMethod,
		//	playerObject,
		//	nullptr,
		//	nullptr
		//);

		Clock clock;

		m_running = true;
		while (m_running)
		{
			float elapsed = clock.elapsed();
			clock.reset();

			Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveLayer();
			gameLayer->onUpdate(elapsed);

			m_renderer->begin(gameLayer->getCamera());
			gameLayer->onRender();
			m_renderer->end();

			m_gui->begin();
			gameLayer->onGuiRender();
			m_gui->end();

			m_audio->update();
			m_window->display();

			//ORC_LOG_INFO("FPS: {}", 1.0f / elapsed);
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
		gameLayer->onEvent(event);

		Event::Type eventType = event.getType();
		switch (eventType)
		{
			case Event::Type::WindowClosed:
			{
				m_running = false;
				event.setHandled();
				break;
			}
			
			case Event::Type::WindowResized:
			{
				auto& windowResizedEvent = getEvent<WindowResizedEvent>(event);
				if (gameLayer)
				{
					gameLayer->getCamera().setViewportSize(0.0f, static_cast<float>(windowResizedEvent.width), static_cast<float>(windowResizedEvent.height), 0.0f);
				}

				event.setHandled();
				break;
			}

			case Event::Type::KeyboardKeyPressed:
			case Event::Type::KeyboardKeyReleased:
			case Event::Type::MouseButtonPressed:
			case Event::Type::MouseButtonReleased:
			case Event::Type::MouseMoved:
			case Event::Type::MouseWheelScrolled:
			case Event::Type::Invalid:
			default:
			{
				event.setHandled();
				break;
			}
		}
	}

}
