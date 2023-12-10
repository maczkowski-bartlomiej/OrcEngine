#pragma once

#include "Engine/Core.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/GameLayerStack.hpp"

#include "Audio/Audio.hpp"

#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

namespace orc {

class Engine
{
public:
	Engine(const GameSettings& gameSettings);
	virtual ~Engine();
	
	void run();

	void pushGameLayer(GameLayer* gameLayer);
	void pushGameOverlay(GameLayer* gameLayer);

	Audio& getAudio();
	Window& getWindow();
	Renderer& getRenderer();

	static Engine& get();

private:
	void onEvent(Event& event);

	bool m_running;
	
	UniquePtr<Audio> m_audio;
	UniquePtr<Window> m_window;
	UniquePtr<Renderer> m_renderer;
	UniquePtr<GameLayerStack> m_gameLayerStack;

	GameSettings m_gameSettings;

	static Engine* m_instance;
};

Engine* startEngine();

}
