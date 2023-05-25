#pragma once

#include "Events/Event.hpp"

namespace orc {

class GameLayer
{
public:
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate(double deltaTime) {}
	virtual void onEvent(Event& event) {}
};

}
