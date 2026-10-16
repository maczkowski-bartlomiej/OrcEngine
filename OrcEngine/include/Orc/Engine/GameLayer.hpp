#pragma once

#include "Engine/FrameContext.hpp"
#include "Engine/LayerContext.hpp"
#include "Events/Event.hpp"
#include "Graphics/Camera.hpp"

namespace orc {

class Renderer;

class GameLayer
{
public:
	explicit GameLayer(LayerContext& context);
	virtual ~GameLayer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}

	virtual void onUpdate(const FrameContext& ctx) {}
	virtual void onEvent(const Event& event);

	virtual void onRender(Renderer& renderer) {}
	virtual void onGuiRender() {}

	Camera& getCamera();
	LayerContext& getContext();

protected:
	LayerContext& m_context;
	Camera camera;
};

}
