#include "OrcPch.hpp"

#include "Engine/Engine.hpp"
#include "Engine/DeltaTime.hpp"
#include "Engine/ResourceHolder.hpp"

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
	ORC_LOG_INFO("Loading shaders...");
	m_shaderHolder = createUniquePtr<ShaderHolder>(m_gameSettings.shadersPath);

	ORC_LOG_INFO("Loading textures...");
	m_textureHolder = createUniquePtr<TextureHolder>(m_gameSettings.texturesPath);

	ORC_LOG_INFO("Loading audio...");
	m_soundBufferHolder = createUniquePtr<SoundBufferHolder>(m_gameSettings.audioPath);

	ORC_LOG_INFO("Initializing game layer stack...");
	m_gameLayerStack = createUniquePtr<GameLayerStack>();
}

Engine::~Engine()
{
	if (m_instance == this)
	{
		m_audio->stopAllSounds();

		ORC_LOG_INFO("Deinitializing game layer stack...");
		m_gameLayerStack.reset();

		ORC_LOG_INFO("Deinitializing resource holders...");
		ORC_LOG_INFO("Unloading audio...");
		m_soundBufferHolder.reset();

		ORC_LOG_INFO("Unloading textures...");
		m_textureHolder.reset();

		ORC_LOG_INFO("Unloading shaders...");
		m_shaderHolder.reset();

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
	DeltaTime deltaTime;
	constexpr double fixedTimestep = 1.0 / 60.0;
	double accumulatedTime = 0.0;

	while (m_running)
	{
		double elapsedTime = deltaTime.elapsed();
		accumulatedTime += elapsedTime;
		deltaTime.reset();

		while (accumulatedTime >= fixedTimestep)
		{
			for (auto layer : *m_gameLayerStack)
				layer->onUpdate((float)fixedTimestep);

			accumulatedTime -= fixedTimestep;
		}

		m_audio->update();
		m_window->display();
	}
}

void Engine::pushGameLayer(GameLayer* gameLayer) 
{
	gameLayer->onAttach();

	m_gameLayerStack->pushGameLayer(gameLayer);
}

void Engine::pushGameOverlay(GameLayer* gameLayer) 
{
	m_gameLayerStack->pushGameOverlay(gameLayer);
}

Engine& Engine::get()
{
	return *m_instance;
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

ShaderHolder& Engine::getShaderHolder()
{
	return *m_shaderHolder;
}

TextureHolder& Engine::getTextureHolder()
{
	return *m_textureHolder;
}

SoundBufferHolder& Engine::getSoundBufferHolder()
{
	return *m_soundBufferHolder;
}

void Engine::onEvent(Event& event) 
{
	for (auto layer : *m_gameLayerStack | std::views::reverse)
	{
		if (event.isHandled())
			break;

		layer->onEvent(event);
	}

	if (event.getType() == Event::Type::WindowClosed)
	{
		m_running = false;
	}
}

}
