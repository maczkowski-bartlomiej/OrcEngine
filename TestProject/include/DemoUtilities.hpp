#pragma once

#include <string_view>

#include <Orc/Orc.hpp>

namespace demo {

void renderNavigationMenu(orc::GameLayerManager& gameLayerManager);

void drawGrid(orc::Renderer& renderer);

orc::Text createShowcaseLabel(const orc::Ref<orc::Font>& font, std::string_view label, const orc::Vector2f& position);

void handleLayerNavigation(orc::LayerContext& layerContext, std::string_view prevLayer, std::string_view nextLayer);

}
