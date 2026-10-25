#include "Engine/GameLayerManager.hpp"

#include <string>

#include "Core/Memory.hpp"
#include "Engine/Debug.hpp"
#include "Engine/GameLayer.hpp"
#include "Engine/Logger.hpp"

namespace orc {

void GameLayerManager::setActiveLayer(std::string_view name)
{
	ORC_FATAL_CHECK(!name.empty(), "Game layer name string is empty.");

	const auto find = m_layers.find(name);
	ORC_FATAL_CHECK(find != m_layers.end(), "Requested switch to non-existing game layer '{}'.", name);

	if (find->second == m_activeLayer)
	{
		ORC_LOG_WARNING("Requested switch to already active game layer '{}'.", name);
		return;
	}

	if (m_activeLayer)
	{
		m_activeLayer->onDetach();
	}

	m_activeLayer = find->second;
	m_activeLayer->syncCameraWithWindow();
	m_activeLayer->onAttach();
}

void GameLayerManager::addLayer(std::string_view name, Ref<GameLayer> gameLayer)
{
	ORC_FATAL_CHECK(!name.empty(), "Game layer name string is empty.");
	ORC_FATAL_CHECK(gameLayer != nullptr, "Game layer is nullptr.");
	ORC_FATAL_CHECK(m_layers.try_emplace(std::string(name), std::move(gameLayer)).second, "Game layer '{}' is already added.", name);
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

const Ref<GameLayer>& GameLayerManager::getActiveLayer() const
{
	return m_activeLayer;
}

}
