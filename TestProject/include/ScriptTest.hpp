#pragma once

#include <Orc/Orc.hpp>

class ScriptTest : public orc::GameLayer
{
public:
	explicit ScriptTest(orc::LayerContext& layerContext);
	~ScriptTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;
};
