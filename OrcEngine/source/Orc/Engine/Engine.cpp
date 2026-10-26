#include "Engine/Engine.hpp"

#include <filesystem>
#include <functional>
#include <string_view>
#include <thread>

#include <Audio/Audio.hpp>
#include <Engine/Config.hpp>
#include <Engine/Debug.hpp>
#include <Engine/GameLayer.hpp>
#include <Engine/GameLayerManager.hpp>
#include <Engine/Logger.hpp>
#include <Engine/ResourceHolder.hpp>
#include <Events/Event.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Window.hpp>

#include "Engine/Clock.hpp"
#include "Core/Core.hpp"

namespace orc {

Engine::Engine(const Config& config)
{
	Logger::init(config.logPath);

	ORC_LOG_INFO("Starting Orc Engine v.{}.{}.{}", version::MAJOR_VERSION, version::MINOR_VERSION, version::PATCH_VERSION);

#if defined(ORC_DEBUG)
	constexpr std::string_view BUILD_CONFIG = "Debug";
#else
	constexpr std::string_view BUILD_CONFIG = "Release";
#endif

#if defined(ORC_PLATFORM_WINDOWS)
	constexpr std::string_view PLATFORM_NAME = "Windows (x64)";
#else
	constexpr std::string_view PLATFORM_NAME = "Linux (x64)";
#endif

	ORC_LOG_INFO("Platform:\n\tOS: {}\n\tBuild Configuration: {}\n\tHardware Threads: {}\n\tWorking Directory: {}", PLATFORM_NAME, BUILD_CONFIG, std::thread::hardware_concurrency(), std::filesystem::current_path().string());

	m_scripting = createUniquePtr<Scripting>();

	m_gameLayerManager = createUniquePtr<GameLayerManager>();

	m_window = createUniquePtr<Window>(config.videoSettings);
	m_window->setEventCallback(std::bind(&Engine::onEvent, this, std::placeholders::_1));

	m_renderer = createUniquePtr<Renderer>();
	m_renderer->setViewport(m_window->getFramebufferWidth(), m_window->getFramebufferHeight());

	m_gui = createUniquePtr<Gui>(*m_window);

	m_audio = createUniquePtr<Audio>(config.audioSettings);

	m_input = createUniquePtr<Input>();

	m_fontResources.loadResources(config.fontsPath);
	m_textureResources.loadResources(config.texturesPath);
	m_animationResources.loadResources(config.animationsPath);

	m_layerContext = createUniquePtr<LayerContext>(LayerContext{
		*m_window,
		*m_renderer,
		*m_audio,
		*m_gameLayerManager,
		m_fontResources,
		m_textureResources,
		m_animationResources,
		*m_input});
}

Engine::~Engine()
{
	ORC_LOG_INFO("Shutting down Orc Engine.");

	m_gameLayerManager->clear();

	m_layerContext.reset();
	m_input.reset();

	m_animationResources.clear();
	m_textureResources.clear();
	m_fontResources.clear();

	m_audio.reset();
	m_gui.reset();
	m_renderer.reset();
	m_window.reset();
	m_gameLayerManager.reset();

	m_scripting.reset();

	Logger::deinit();
}

void Engine::run()
{
	Clock clock;

	m_running = true;
	while (m_running)
	{
		float elapsed = clock.restart();

		m_renderer->clear();

		Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveLayer();
		if (gameLayer)
		{
			gameLayer->onUpdate(elapsed);

			gameLayer->onRender();

			m_gui->begin();
			gameLayer->onGuiRender();
			m_gui->end();
		}

		m_audio->update();
		m_window->display();
	}
}

LayerContext& Engine::getLayerContext()
{
	return *m_layerContext;
}

const LayerContext& Engine::getLayerContext() const
{
	return *m_layerContext;
}

void Engine::onEvent(const Event& event)
{
	m_input->onEvent(event);

	if (event.getType() == Event::Type::WindowResized)
	{
		m_renderer->setViewport(m_window->getFramebufferWidth(), m_window->getFramebufferHeight());
	}

	Ref<GameLayer> gameLayer = m_gameLayerManager->getActiveLayer();
	if (gameLayer)
	{
		gameLayer->onEvent(event);
	}

	if (event.getType() == Event::Type::WindowClosed)
	{
		m_running = false;
	}
}

}
