#pragma once

#include "Audio/Audio.hpp"
#include "Core.hpp"
#include "Engine/Config.hpp"
#include "Engine/GameLayerManager.hpp"
#include "Engine/ResourceHolder.hpp"
#include "Events/Event.hpp"
#include "Graphics/FTLibrary.hpp"
#include "Graphics/Gui.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Window.hpp"

namespace orc {

class Engine
{
public:
	Engine(const Config& config);
	~Engine();

	void run();

	Audio& getAudio();
	Window& getWindow();
	Renderer& getRenderer();
	FTLibrary& getFTLibary();
	GameLayerManager& getGameLayerManager();

	FontResources& getFontResources();
	TextureResources& getTextureResources();
	AnimationResources& getAnimationResources();

	static Engine& get();

private:
	void onEvent(const Event& event);

	bool m_running = false;

	UniquePtr<Gui> m_gui;
	UniquePtr<Audio> m_audio;
	UniquePtr<Window> m_window;
	UniquePtr<Renderer> m_renderer;
	UniquePtr<FTLibrary> m_ftLibary;
	UniquePtr<GameLayerManager> m_gameLayerManager;

	FontResources m_fontResources;
	TextureResources m_textureResources;
	AnimationResources m_animationResources;

	static Engine* m_instance;
};

Config getEngineConfig();
void onEngineStart(Engine& engine);

}
