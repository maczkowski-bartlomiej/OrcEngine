#pragma once

#include <Orc/Orc.hpp>

class AnimationTest : public orc::GameLayer
{
public:
	explicit AnimationTest(orc::LayerContext& context);
	~AnimationTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(const orc::FrameContext& ctx) override;
	void onEvent(const orc::Event& event) override;

	void onRender(orc::Renderer& renderer) override;
	void onGuiRender() override;

private:
};
