#pragma once

#include "Engine/ResourceHolder.hpp"

namespace orc {

class Window;
class Renderer;
class Audio;
class GameLayerManager;
class Input;

struct LayerContext
{
	Window& window;
	Renderer& renderer;
	Audio& audio;
	GameLayerManager& gameLayerManager;
	FontResources& fontResources;
	TextureResources& textureResources;
	AnimationResources& animationResources;
	Input& input;
};

}
