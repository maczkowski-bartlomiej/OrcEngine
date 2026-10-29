#include "DemoUtilities.hpp"

#include <imgui.h>

namespace demo {

void renderNavigationMenu(orc::GameLayerManager& gameLayerManager)
{
	ImGui::Begin("Navigation Menu");
	{
		if (ImGui::Button("AnimationTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("AnimationTest");
		}
		if (ImGui::Button("AudioTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("AudioTest");
		}
		if (ImGui::Button("CameraTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("CameraTest");
		}
		if (ImGui::Button("CirclesTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("CirclesTest");
		}
		if (ImGui::Button("Menu", {200, 50}))
		{
			gameLayerManager.setActiveLayer("Menu");
		}
		if (ImGui::Button("InputTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("InputTest");
		}
		if (ImGui::Button("RectanglesTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("RectanglesTest");
		}
		if (ImGui::Button("SpritesTest", {200, 50}))
		{
			gameLayerManager.setActiveLayer("SpritesTest");
		}
	}
	ImGui::End();
}

void drawGrid(orc::Renderer& renderer)
{
	renderer.drawLine(orc::Vector2f(200.0f, 0.0f), orc::Vector2f(200.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(400.0f, 0.0f), orc::Vector2f(400.0f, 600.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(600.0f, 0.0f), orc::Vector2f(600.0f, 600.0f), orc::Color(255));

	renderer.drawLine(orc::Vector2f(0.0f, 150.0f), orc::Vector2f(800.0f, 150.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 300.0f), orc::Vector2f(800.0f, 300.0f), orc::Color(255));
	renderer.drawLine(orc::Vector2f(0.0f, 450.0f), orc::Vector2f(800.0f, 450.0f), orc::Color(255));
}

orc::Text createShowcaseLabel(const orc::Ref<orc::Font>& font, std::string_view label, const orc::Vector2f& position)
{
	orc::Text text(font, label);
	text.setScale(0.25f, 0.25f);
	text.setOrigin(text.getGlobalRect().getSize().x / 2.0f, 0.0f);
	text.setPosition(position);
	return text;
}

void handleLayerNavigation(orc::LayerContext& layerContext, std::string_view prevLayer, std::string_view nextLayer)
{
	if (layerContext.input.wasKeyJustPressed(orc::Keyboard::Key::Left))
	{
		layerContext.gameLayerManager.setActiveLayer(prevLayer);
	}
	else if (layerContext.input.wasKeyJustPressed(orc::Keyboard::Key::Right))
	{
		layerContext.gameLayerManager.setActiveLayer(nextLayer);
	}
}

}
