#pragma once

#include <Orc/Orc.hpp>

class CameraTest : public orc::GameLayer
{
public:
	explicit CameraTest(orc::LayerContext& layerContext);
	~CameraTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;

private:
	orc::Sprite m_sprite1;
	orc::Sprite m_sprite2;
};
