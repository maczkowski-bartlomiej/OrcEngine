#pragma once

#include <Orc/Orc.hpp>

class AudioTest : public orc::GameLayer
{
public:
	AudioTest();
	~AudioTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;
};
