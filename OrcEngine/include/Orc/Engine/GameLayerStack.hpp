#pragma once

#include "GameLayer.hpp"

#include <vector>

namespace orc {

class GameLayerStack
{
public:
	GameLayerStack();
	~GameLayerStack();

	GameLayerStack(GameLayerStack&&) = delete;
	GameLayerStack(const GameLayerStack&) = delete;
	GameLayerStack operator=(GameLayerStack&&) = delete;
	GameLayerStack operator=(const GameLayerStack&) = delete;

	void pushGameLayer(GameLayer* gameLayer);
	void pushGameOverlay(GameLayer* gameLayer);

	void popGameLayer(GameLayer* gameLayer);
	void popGameOverlay(GameLayer* gameLayer);

	std::vector<GameLayer*>::iterator end();
	std::vector<GameLayer*>::iterator begin();

	std::vector<GameLayer*>::const_iterator end() const;
	std::vector<GameLayer*>::const_iterator begin() const;

private:
	size_t m_gameLayerInsertIndex;
	std::vector<GameLayer*> m_gameLayers;
};

}
