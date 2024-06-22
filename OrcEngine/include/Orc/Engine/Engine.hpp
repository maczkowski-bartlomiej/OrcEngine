#pragma once

#include "Engine/Core.hpp"
#include "Engine/GameSettings.hpp"
#include "Engine/GameLayerManager.hpp"

#include "Audio/Audio.hpp"

#include "Graphics/Gui.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

namespace orc {

class Engine
{
public:
	Engine(const GameSettings& gameSettings);
	virtual ~Engine();
	
	void run();

	Audio& getAudio();
	Window& getWindow();
	Renderer& getRenderer();
	GameLayerManager& getGameLayerManager();

	FontHolder& getFontHolder();
	ShaderHolder& getShaderHolder();
	TextureHolder& getTextureHolder();
	AnimationHolder& getAnimationHolder();
	SoundBufferHolder& getSoundBufferHolder();

	static Engine& get();

private:
	void onEvent(Event& event);

	bool m_running;

	UniquePtr<Gui> m_gui;
	UniquePtr<Audio> m_audio;
	UniquePtr<Window> m_window;
	UniquePtr<Renderer> m_renderer;
	UniquePtr<GameLayerManager> m_gameLayerManager;

	UniquePtr<FontHolder> m_fontHolder;
	UniquePtr<ShaderHolder> m_shaderHolder;
	UniquePtr<TextureHolder> m_textureHolder;
	UniquePtr<AnimationHolder> m_animationHolder;
	UniquePtr<SoundBufferHolder> m_soundBufferHolder;

	GameSettings m_gameSettings;

	static Engine* m_instance;
};

Engine* startEngine();

}
