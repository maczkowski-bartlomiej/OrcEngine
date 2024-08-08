#pragma once

#include <Orc/Orc.hpp>

class AnimationTest : public orc::GameLayer
{
public:
	AnimationTest();
	~AnimationTest();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(orc::Event& event) override;

private:
	orc::Sprite m_sprite;
};
