#include "OrcPch.hpp"

#include "Engine/GameLayerStack.hpp"

namespace orc {

GameLayerStack::GameLayerStack() 
	:m_gameLayerInsertIndex(0u)
{
}

GameLayerStack::~GameLayerStack() 
{
	for (GameLayer* gameLayer : m_gameLayers)
	{
		gameLayer->onDetach();
		delete gameLayer;
	}
}

void GameLayerStack::pushGameLayer(GameLayer* gameLayer) 
{
	m_gameLayers.emplace(m_gameLayers.begin() + m_gameLayerInsertIndex, gameLayer);
	m_gameLayerInsertIndex++;
}

void GameLayerStack::pushGameOverlay(GameLayer* gameLayer) 
{
	m_gameLayers.push_back(gameLayer);
}

void GameLayerStack::popGameLayer(GameLayer* gameLayer) 
{
	auto find = std::find(m_gameLayers.begin(), m_gameLayers.end(), gameLayer);
	if (find != m_gameLayers.end())
	{
		gameLayer->onDetach();

		m_gameLayers.erase(find);
		m_gameLayerInsertIndex--;
	}
}

void GameLayerStack::popGameOverlay(GameLayer* gameLayer) 
{
	auto find = std::find(m_gameLayers.begin(), m_gameLayers.end(), gameLayer);
	if (find != m_gameLayers.end())
	{
		gameLayer->onDetach();

		m_gameLayers.erase(find);
	}
}

std::vector<GameLayer*>::const_iterator GameLayerStack::end() const 
{
	return m_gameLayers.cbegin();
}

std::vector<GameLayer*>::const_iterator GameLayerStack::begin() const 
{
	return m_gameLayers.cend();
}

std::vector<GameLayer*>::iterator GameLayerStack::end() 
{
	return m_gameLayers.end();
}

std::vector<GameLayer*>::iterator GameLayerStack::begin() 
{
	return m_gameLayers.begin();
}

}
