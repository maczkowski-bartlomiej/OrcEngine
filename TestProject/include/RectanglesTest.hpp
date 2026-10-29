#pragma once

#include <vector>

#include <Orc/Orc.hpp>

class RectanglesTest : public orc::GameLayer
{
public:
	explicit RectanglesTest(orc::LayerContext& layerContext);
	~RectanglesTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;

private:
	struct GridItem
	{
		orc::Rectangle shape;
		orc::Text label;
	};

	std::vector<GridItem> m_items;
	orc::Rectangle* m_rectangle4 = nullptr;
};
