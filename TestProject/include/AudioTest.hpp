#pragma once

#include <Orc/Orc.hpp>

class AudioTest : public orc::GameLayer
{
public:
	explicit AudioTest(orc::LayerContext& context);
	~AudioTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(const orc::FrameContext& ctx) override;
	void onEvent(const orc::Event& event) override;

	void onRender(orc::Renderer& renderer) override;
	void onGuiRender() override;
};
