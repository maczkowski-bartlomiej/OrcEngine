#pragma once

#include "Engine/Core.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/GameLayerStack.hpp"

#include "Graphics/Window.hpp"

namespace orc {

class Engine
{
public:
	Engine(const GameSettings& gameSettings);
	virtual ~Engine();
	
	void run();

	void pushGameLayer(GameLayer* gameLayer);
	void pushGameOverlay(GameLayer* gameLayer);

	Window& getWindow();

	static Engine& get();

private:
	void onEvent(Event& event);

	bool m_running;

	GameSettings m_gameSettings;
	UniquePtr<Window> m_window;
	GameLayerStack m_gameLayerStack;

	static Engine* m_instance;
};

Engine* startEngine();

}
