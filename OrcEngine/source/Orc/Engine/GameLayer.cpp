#include "Engine/GameLayer.hpp"

#include "Events/WindowEvents.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Window.hpp"

namespace orc {

GameLayer::GameLayer(LayerContext& layerContext)
	: m_layerContext(layerContext),
	  m_camera(0.0f, 0.0f, static_cast<float>(m_layerContext.window.getWidth()), static_cast<float>(m_layerContext.window.getHeight()))
{
}

void GameLayer::onEvent(const Event& event)
{
	if (event.getType() == Event::Type::WindowResized)
	{
		const auto& resized = getEvent<WindowResizedEvent>(event);
		m_camera.setViewportSize(0.0f, 0.0f, static_cast<float>(resized.width), static_cast<float>(resized.height));
	}
}

}
