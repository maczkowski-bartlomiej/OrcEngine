#pragma once

#include "Engine/LayerContext.hpp"
#include "Events/Event.hpp"
#include "Graphics/Camera.hpp"

namespace orc {

class GameLayer
{
public:
	explicit GameLayer(LayerContext& layerContext);
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void onUpdate(float deltaTime) {}
	virtual void onEvent(const Event& event);

	virtual void onRender() {}
	virtual void onGuiRender() {}

	void syncCameraWithWindow();

protected:
	LayerContext& m_layerContext;
	Camera m_camera;
};

}
