#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Core/Memory.hpp"
#include "Engine/GameLayer.hpp"

namespace orc {

class GameLayerManager
{
public:
	void setActiveLayer(const std::string& name);
	void addLayer(const std::string& name, Ref<GameLayer> gameLayer);

	void clear();

	Ref<GameLayer> getActiveLayer() const;

private:
	Ref<GameLayer> m_activeLayer;
	std::unordered_map<std::string, Ref<GameLayer>> m_layers;
};

}
