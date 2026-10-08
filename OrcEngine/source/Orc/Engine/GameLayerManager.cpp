#include "OrcPch.hpp"

#include "Engine/Debug.hpp"
#include "Engine/GameLayerManager.hpp"
#include <string>
#include "Engine/Core.hpp"
#include "Engine/GameLayer.hpp"
#include "Engine/Logger.hpp"

namespace orc {

void GameLayerManager::setActiveLayer(const std::string& name)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty.");

	if (const auto find = m_layers.find(name); find != m_layers.end())
	{
		if (find->second == m_activeLayer)
		{
			ORC_LOG_WARNING("Requested switch to already active game layer '{}.'", name);
			return;
		}

		if (m_activeLayer)
			m_activeLayer->onDetach();

		m_activeLayer = find->second;
		m_activeLayer->onAttach();
	}
	else
	{
		ORC_LOG_ERROR("Requested switch to non-existing game layer '{}.'", name);
	}
}

void GameLayerManager::addLayer(const std::string& name, Ref<GameLayer> gameLayer)
{
	ORC_ASSERT(!name.empty(), "Game layer name string is empty.");
	ORC_ASSERT(gameLayer, "Game layer is nullptr.");

	if (!m_layers.try_emplace(name, gameLayer).second)
	{
		ORC_LOG_WARNING("Game layer '{}' is already added.", name);
	}
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

Ref<GameLayer> GameLayerManager::getActiveLayer() const
{
	return m_activeLayer;
}


}
