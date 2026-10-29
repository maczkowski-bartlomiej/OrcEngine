#pragma once

#include <Orc/Orc.hpp>

class InputTest : public orc::GameLayer
{
public:
	explicit InputTest(orc::LayerContext& layerContext);
	~InputTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;

	void onRender() override;
	void onGuiRender() override;

private:
	orc::Sprite m_player;
	static constexpr float SPEED = 50.0f;
};
