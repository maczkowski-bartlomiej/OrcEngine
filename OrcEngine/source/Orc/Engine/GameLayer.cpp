#include "OrcPch.hpp"

#include "Engine/GameLayer.hpp"
#include "Events/WindowEvents.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Window.hpp"

namespace orc {

namespace {

constexpr float DEFAULT_VIEWPORT_WIDTH = 1280.0f;
constexpr float DEFAULT_VIEWPORT_HEIGHT = 720.0f;

float resolveViewportWidth(const Window& window)
{
	return window.getWidth() > 0 ? static_cast<float>(window.getWidth()) : DEFAULT_VIEWPORT_WIDTH;
}

float resolveViewportHeight(const Window& window)
{
	return window.getHeight() > 0 ? static_cast<float>(window.getHeight()) : DEFAULT_VIEWPORT_HEIGHT;
}

}

GameLayer::GameLayer(LayerContext& context)
	: m_context(context),
	camera(0.0f, 0.0f, resolveViewportWidth(context.window), resolveViewportHeight(context.window))
{
}

void GameLayer::onEvent(const Event& event)
{
	if (const auto* resized = event.getIf<WindowResizedEvent>())
	{
		camera.setViewportSize(0.0f, static_cast<float>(resized->width), static_cast<float>(resized->height), 0.0f);
	}
}

Camera& GameLayer::getCamera()
{
	return camera;
}

LayerContext& GameLayer::getContext()
{
	return m_context;
}

}
