#pragma once

#include "Audio/Audio.hpp"

#include "Events/Event.hpp"

#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

namespace orc {

class GameLayer
{
protected:
	Audio& audio;
	Window& window;
	Renderer& renderer;

public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void onEvent(Event& event) {}
	virtual void onUpdate(float deltaTime) {}
};

}
