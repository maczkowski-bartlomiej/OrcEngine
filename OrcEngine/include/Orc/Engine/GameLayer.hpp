#pragma once

#include "Audio/Audio.hpp"

#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

#include "Engine/ResourceHolder.hpp"

#include "Events/Event.hpp"

namespace orc {

class GameLayer
{
protected:
	Audio& audio;
	Window& window;
	Renderer& renderer;

	ShaderHolder& shaderHolder;
	TextureHolder& textureHolder;
	SoundBufferHolder& soundBufferHolder;

public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void onEvent(Event& event) {}
	virtual void onUpdate(float deltaTime) {}
};

}
