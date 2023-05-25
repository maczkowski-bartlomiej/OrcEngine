#pragma once

#include "System/System.hpp"
#include "Graphics/Window.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/GameLayerStack.hpp"

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
	UniquePointer<Window> m_window;
	GameLayerStack m_gameLayerStack;

	static Engine* m_instance;
};

Engine* startEngine();

}
