#pragma once

#include <Orc/Orc.hpp>

class AnimationTest : public orc::GameLayer
{
public:
	explicit AnimationTest(orc::LayerContext& layerContext);
	~AnimationTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;

private:
};
