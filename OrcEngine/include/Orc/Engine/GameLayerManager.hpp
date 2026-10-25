#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Core/Memory.hpp"
#include "Engine/GameLayer.hpp"
#include "Engine/Utility.hpp"

namespace orc {

class GameLayerManager
{
public:
	void setActiveLayer(std::string_view name);
	void addLayer(std::string_view name, Ref<GameLayer> gameLayer);

	void clear();

	const Ref<GameLayer>& getActiveLayer() const;

private:
	Ref<GameLayer> m_activeLayer;
	std::unordered_map<std::string, Ref<GameLayer>, StringViewHash, std::equal_to<>> m_layers;
};

}
