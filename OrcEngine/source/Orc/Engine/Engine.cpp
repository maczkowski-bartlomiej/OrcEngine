#include "orcPch.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Version.hpp"
#include "Engine/ResourceHolder.hpp"

#include "Engine/DeltaTime.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/RenderCommand.hpp"
#include "Graphics/BufferLayout.hpp"

#include "Events/KeyboardEvents.hpp"

#include <glad/glad.h>

namespace orc
{

Engine* Engine::m_instance = nullptr;

Engine::Engine(const GameSettings& gameSettings)
	: m_running(true), m_gameSettings(gameSettings)
{
	ORC_CORE_ASSERT(!m_instance, "Engine instance already running")
	m_instance = this;

	Logger::init(std::string(gameSettings.logPath), std::string(gameSettings.loggerName));

	ORC_CORE_LOG_INFO("orc Engine v.{}.{}.{}", version::MAJOR_VERSION, version::MINOR_VERSION, version::PATCH_VERSION);

	m_window = std::make_unique<Window>(m_gameSettings.videoSettings);
	m_window->setEventCallback(std::bind(&Engine::onEvent, this, std::placeholders::_1));
}

Engine::~Engine()
{
	ORC_CORE_LOG_INFO("Engine shutting down...");

	Logger::shutdown();
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
			for (auto layer : m_gameLayerStack)
				layer->onUpdate(fixedTimestep);

			accumulatedTime -= fixedTimestep;
		}

		m_window->display();
	}
}

void Engine::pushGameLayer(GameLayer* gameLayer) 
{
	gameLayer->onAttach();

	m_gameLayerStack.pushGameLayer(gameLayer);
}

void Engine::pushGameOverlay(GameLayer* gameLayer) 
{
	m_gameLayerStack.pushGameOverlay(gameLayer);
}

Window& Engine::getWindow()
{
	return *m_window;
}

Engine& Engine::get()
{
	return *m_instance;
}

void Engine::onEvent(Event& event) 
{
	for (auto layer : m_gameLayerStack | std::views::reverse)
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
