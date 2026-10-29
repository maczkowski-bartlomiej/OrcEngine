#pragma once

#include <vector>

#include <Orc/Orc.hpp>

class CirclesTest : public orc::GameLayer
{
public:
	explicit CirclesTest(orc::LayerContext& layerContext);
	~CirclesTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;

private:
	struct GridItem
	{
		orc::Circle shape;
		orc::Text label;
	};

	std::vector<GridItem> m_items;
	orc::Circle* m_circle11 = nullptr;
};
