#pragma once

#include <Orc/Orc.hpp>

class CameraTest : public orc::GameLayer
{
public:
	explicit CameraTest(orc::LayerContext& context);
	~CameraTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(const orc::FrameContext& ctx) override;
	void onEvent(const orc::Event& event) override;

	void onRender(orc::Renderer& renderer) override;
	void onGuiRender() override;

private:
	orc::Sprite m_sprite1;
	orc::Sprite m_sprite2;
};
