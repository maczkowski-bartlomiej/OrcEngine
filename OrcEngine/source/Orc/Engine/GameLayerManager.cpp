#include "OrcPch.hpp"

#include "Engine/Debug.hpp"
#include "Engine/GameLayerManager.hpp"

namespace orc {

void GameLayerManager::setActiveLayer(const std::string& name)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty");

	if (const auto& find = m_layers.find(name); find != m_layers.end())
	{
		if (m_activeLayer)
			m_activeLayer->onDetach();

		m_activeLayer = find->second;
		m_activeLayer->onAttach();
	}
	else
	{
		ORC_LOG_ERROR("Requested switch to non-existing game layer '{}'", name);
	}
}

void GameLayerManager::addLayer(const std::string& name, Ref<GameLayer> gameLayer)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty");
	ORC_ASSERT(gameLayer, "Game layer is nullptr");

	m_layers[name] = gameLayer;
}

void GameLayerManager::clear()
{
	if (m_activeLayer)
	{
		m_activeLayer->onDetach();
		m_activeLayer.reset();
	}

	m_layers.clear();
}

Ref<GameLayer> GameLayerManager::getActiveLayer()
{
	return m_activeLayer;
}


}
