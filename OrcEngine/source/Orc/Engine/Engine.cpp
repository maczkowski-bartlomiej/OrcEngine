#include "OrcPch.hpp"

#include "Engine/Clock.hpp"
#include "Engine/Engine.hpp"
#include "Engine/ResourceHolder.hpp"

#include "Events/WindowEvents.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/FTLibrary.hpp"

namespace orc {

Engine* Engine::m_instance = nullptr;

Engine::Engine(const GameSettings& gameSettings)
	: m_running(true), m_gameSettings(gameSettings)
{
	if (m_instance)
	{
		ORC_FATAL("Engine instance already exist!!!");
		return;
	}

	m_instance = this;

	Logger::init(gameSettings.logPath);

	ORC_LOG_INFO("Orc Engine v.{}.{}.{}", version::MAJOR_VERSION, version::MINOR_VERSION, version::PATCH_VERSION);

	ORC_LOG_INFO("Initializing FreeType Library...");
	FTLibrary::init();

	ORC_LOG_INFO("Initializing window...");
	m_window = createUniquePtr<Window>(m_gameSettings.videoSettings);
	m_window->setEventCallback(std::bind(&Engine::onEvent, this, std::placeholders::_1));

	ORC_LOG_INFO("Initializing renderer...");
	m_renderer = createUniquePtr<Renderer>();

	ORC_LOG_INFO("Initializing audio...");
	m_audio = createUniquePtr<Audio>();

	ORC_LOG_INFO("Initializing resource holders...");

	ORC_LOG_INFO("Loading fonts...");
	m_fontHolder = createUniquePtr<FontHolder>(m_gameSettings.fontsPath);

	ORC_LOG_INFO("Loading shaders...");
	m_shaderHolder = createUniquePtr<ShaderHolder>(m_gameSettings.shadersPath);

	ORC_LOG_INFO("Loading textures...");
	m_textureHolder = createUniquePtr<TextureHolder>(m_gameSettings.texturesPath);

	ORC_LOG_INFO("Loading animations...");
	m_animationHolder = createUniquePtr<AnimationHolder>(m_gameSettings.animationsPath);

	ORC_LOG_INFO("Loading audio...");
	m_soundBufferHolder = createUniquePtr<SoundBufferHolder>(m_gameSettings.audioPath);

	ORC_LOG_INFO("Initializing gui...");
	m_gui = createUniquePtr<Gui>();

	ORC_LOG_INFO("Initializing game layer manager...");
	m_gameLayerManager = createUniquePtr<GameLayerManager>();
}

Engine::~Engine()
{
	if (m_instance == this)
	{
		m_audio->stopAllSounds();

		ORC_LOG_INFO("Deinitializing game layer manager...");
		m_gameLayerManager.reset();

		ORC_LOG_INFO("Deinitializing gui...");
		m_gui.reset();

		ORC_LOG_INFO("Deinitializing resource holders...");
		ORC_LOG_INFO("Unloading audio...");
		m_soundBufferHolder.reset();

		ORC_LOG_INFO("Unloading textures...");
		m_textureHolder.reset();

		ORC_LOG_INFO("Unloading shaders...");
		m_shaderHolder.reset();

		ORC_LOG_INFO("Unloading fonts...");
		m_fontHolder.reset();

		ORC_LOG_INFO("Unloading animiations...");
		m_animationHolder.reset();

		ORC_LOG_INFO("Deinitializing audio...");
		m_audio.reset();

		ORC_LOG_INFO("Deinitializing renderer...");
		m_renderer.reset();

		ORC_LOG_INFO("Deinitializing window...");
		m_window.reset();

		ORC_LOG_INFO("Deinitializing FreeType Library...");
		FTLibrary::shutdown();

		ORC_LOG_INFO("Engine shutting down...");
		Logger::shutdown();
	}
}

void Engine::run()
{
	Clock clock;

	while (m_running)
	{
		float elapsed = clock.elapsed();
		clock.reset();

		Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveGameLayer();
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

FontHolder& Engine::getFontHolder()
{
	return *m_fontHolder;
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

ShaderHolder& Engine::getShaderHolder()
{
	return *m_shaderHolder;
}

TextureHolder& Engine::getTextureHolder()
{
	return *m_textureHolder;
}

AnimationHolder& Engine::getAnimationHolder()
{
	return *m_animationHolder;
}

SoundBufferHolder& Engine::getSoundBufferHolder()
{
	return *m_soundBufferHolder;
}

void Engine::onEvent(Event& event) 
{
	m_gameLayerManager->getActiveGameLayer()->onEvent(event);

	if (event.getType() == Event::Type::WindowClosed)
	{
		m_running = false;
	}
	else if (event.getType() == Event::Type::WindowResized)
	{
		const WindowResizedEvent& windowResizedEvent = getEvent<WindowResizedEvent>(event);
		Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveGameLayer();
		if (gameLayer)
		{
			gameLayer->getCamera().setViewportSize(0.0f, static_cast<float>(windowResizedEvent.width), static_cast<float>(windowResizedEvent.height), 0.0f);
		}
	}
}

}
