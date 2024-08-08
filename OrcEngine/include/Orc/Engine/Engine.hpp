#pragma once

#include "Engine/Core.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/GameLayerManager.hpp"

#include "Audio/Audio.hpp"

#include "Graphics/Gui.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/FTLibrary.hpp"

namespace orc {

class Engine
{
public:
	bool init(const GameSettings& gameSettings);
	void deinit();

	void run();

	Audio& getAudio();
	Window& getWindow();
	Renderer& getRenderer();
	GameLayerManager& getGameLayerManager();

	FTLibrary& getFTLibary();
	FontHolder& getFontHolder();
	TextureHolder& getTextureHolder();
	AnimationHolder& getAnimationHolder();

	static Engine& get();

private:
	void onEvent(Event& event);

	bool m_running = false;

	FTLibrary m_ftLibary;

	Window m_window;
	Renderer m_renderer;
	Audio m_audio;

	Gui m_gui;
	GameLayerManager m_gameLayerManager;

	FontHolder m_fontHolder;
	TextureHolder m_textureHolder;
	AnimationHolder m_animationHolder;

	GameSettings m_gameSettings;

	static Engine* m_instance;
};

Engine* startEngine();

}
