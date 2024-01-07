#include "OrcPch.hpp"

#include "Engine/Debug.hpp"
#include "Engine/GameLayerManager.hpp"

namespace orc {

void GameLayerManager::setActiveGameLayer(const std::string& name)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty");

	if (const auto& find = m_gameLayers.find(name); find != m_gameLayers.end())
	{
		if (m_activeGameLayer)
			m_activeGameLayer->onDetach();

		m_activeGameLayer = find->second;
		m_activeGameLayer->onAttach();
	}
	else
	{
		ORC_LOG_ERROR("Requested switch to non-existing game layer '{}'", name);
	}
}

Ref<GameLayer> GameLayerManager::getActiveGameLayer()
{
	return m_activeGameLayer;
}

void GameLayerManager::addGameLayer(const std::string& name, Ref<GameLayer> gameLayer)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty");
	ORC_ASSERT(gameLayer, "Game layer is nullptr");

	m_gameLayers[name] = gameLayer;
}

}
