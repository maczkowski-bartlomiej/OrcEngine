#pragma once

#include <Orc/Orc.hpp>

class AudioTest : public orc::GameLayer
{
public:
	explicit AudioTest(orc::LayerContext& layerContext);
	~AudioTest();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;
};
