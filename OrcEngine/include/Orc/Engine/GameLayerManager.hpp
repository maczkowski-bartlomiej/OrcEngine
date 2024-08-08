#pragma once

#include "Engine/Core.hpp"
#include "Engine/GameLayer.hpp"

#include <string>
#include <unordered_map>

namespace orc {

class GameLayerManager
{
public:
	void setActiveGameLayer(const std::string& name);
	void addGameLayer(const std::string& name, Ref<GameLayer> gameLayer);

	Ref<GameLayer> getActiveGameLayer();

private:
	Ref<GameLayer> m_activeGameLayer;
	std::unordered_map<std::string, Ref<GameLayer>> m_gameLayers;
};

}
