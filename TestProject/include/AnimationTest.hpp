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
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;

	void onRender() override;
	void onGuiRender() override;

private:
};
