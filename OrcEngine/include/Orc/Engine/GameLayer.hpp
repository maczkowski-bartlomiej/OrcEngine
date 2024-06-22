#pragma once

#include "Events/Event.hpp"
#include "Graphics/Camera.hpp"
#include "Engine/ResourceHolder.hpp"

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

	virtual void onUpdate(float dt) {}
	virtual void onEvent(Event& event) {}

	virtual void onRender() {}
	virtual void onGuiRender() {}

	Camera& getCamera();

protected:
	Camera camera;

	Audio& audio;
	Window& window;
	Renderer& renderer;
	GameLayerManager& gameLayerManager;

	FontHolder& fontHolder;
	ShaderHolder& shaderHolder;
	TextureHolder& textureHolder;
	AnimationHolder& animationHolder;
	SoundBufferHolder& soundBufferHolder;
};

}
