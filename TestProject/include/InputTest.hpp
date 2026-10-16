#pragma once

#include <Orc/Orc.hpp>

class InputTest : public orc::GameLayer
{
public:
	explicit InputTest(orc::LayerContext& context);
	~InputTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(const orc::FrameContext& ctx) override;
	void onEvent(const orc::Event& event) override;

	void onRender(orc::Renderer& renderer) override;
	void onGuiRender() override;

private:
	orc::Sprite m_player;
	static constexpr float M_SPEED = 50.0f;
};
