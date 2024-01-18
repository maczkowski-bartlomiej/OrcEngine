#pragma once

#include "Engine/ResourceHolder.hpp"
#include "Graphics/Camera.hpp"
#include "Events/Event.hpp"

namespace orc {

class Audio;
class Window;
class Renderer;
class GameLayerManager;

class GameLayer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void onEvent(Event& event) {}
	virtual void onUpdate(float deltaTime) {}

protected:
	Camera camera;

	Audio& audio;
	Window& window;
	Renderer& renderer;
	GameLayerManager& gameLayerManager;

	FontHolder& fontHolder;
	ShaderHolder& shaderHolder;
	TextureHolder& textureHolder;
	SoundBufferHolder& soundBufferHolder;
};

}
