#pragma once

#include "Audio/Audio.hpp"

#include "Graphics/Gui.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/FTLibrary.hpp"

<<<<<<< ours
#include "Events/Event.hpp"
#include "Engine/Config.hpp"
#include "Engine/ResourceHolder.hpp"
#include "Engine/GameLayerManager.hpp"

||||||| ancestor
=======
#include "Events/Event.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/ResourceHolder.hpp"
#include "Engine/GameLayerManager.hpp"

>>>>>>> theirs
namespace orc {

class Engine
{
public:
	bool init(const Config& config);
	void deinit();

	void run();

	Audio& getAudio();
	Window& getWindow();
	Renderer& getRenderer();
	FTLibrary& getFTLibary();
	GameLayerManager& getGameLayerManager();

	FontHolder& getFontHolder();
	TextureHolder& getTextureHolder();
	AnimationHolder& getAnimationHolder();

	static Engine& get();

private:
	void onEvent(const Event& event);

	bool m_running = false;
	static Engine* m_instance;

	Gui m_gui;
	Audio m_audio;
	Window m_window;
	Renderer m_renderer;
	FTLibrary m_ftLibary;
	GameLayerManager m_gameLayerManager;

	FontHolder m_fontHolder;
	TextureHolder m_textureHolder;
	AnimationHolder m_animationHolder;
};

Config getEngineConfig();
void onEngineStart(Engine& engine);

}
