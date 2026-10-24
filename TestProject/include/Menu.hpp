#pragma once

#include <Orc/Orc.hpp>

class Menu : public orc::GameLayer
{
public:
	explicit Menu(orc::LayerContext& layerContext);
	~Menu();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;
};
