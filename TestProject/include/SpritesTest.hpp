#pragma once

#include <vector>

#include <Orc/Orc.hpp>

class SpritesTest : public orc::GameLayer
{
public:
	explicit SpritesTest(orc::LayerContext& layerContext);
	~SpritesTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;

private:
	struct GridItem
	{
		orc::Sprite shape;
		orc::Text label;
	};

	std::vector<GridItem> m_items;
	orc::Sprite* m_sprite4 = nullptr;
};
