#pragma once

#include "Audio/Audio.hpp"
#include "Engine/Config.hpp"
#include "Engine/GameLayerManager.hpp"
#include "Engine/LayerContext.hpp"
#include "Engine/ResourceHolder.hpp"
#include "Events/Event.hpp"
#include "Graphics/Gui.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Window.hpp"
#include "Input/Input.hpp"
#include "Script/Scripting.hpp"

namespace orc {

class Engine
{
public:
	explicit Engine(const Config& config);
	~Engine();

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;

	void run();

	LayerContext& getLayerContext();
	const LayerContext& getLayerContext() const;

private:
	void onEvent(const Event& event);

	bool m_running = false;

	UniquePtr<Gui> m_gui;
	UniquePtr<Audio> m_audio;
	UniquePtr<Window> m_window;
	UniquePtr<Renderer> m_renderer;
	UniquePtr<GameLayerManager> m_gameLayerManager;
	UniquePtr<Input> m_input;
	UniquePtr<LayerContext> m_layerContext;
	UniquePtr<Scripting> m_scripting;

	FontResources m_fontResources;
	TextureResources m_textureResources;
	AnimationResources m_animationResources;
};

}
