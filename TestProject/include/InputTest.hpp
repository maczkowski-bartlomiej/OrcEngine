#pragma once

#include <Orc/Orc.hpp>

class InputTest : public orc::GameLayer
{
public:
	InputTest();
	~InputTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;

private:
	orc::Sprite m_player;
	static constexpr float M_SPEED = 50.0f;
};
