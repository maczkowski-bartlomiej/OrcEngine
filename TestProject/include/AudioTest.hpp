#pragma once

#include <Orc/Orc.hpp>

#include <map>

class AudioTest : public orc::GameLayer
{
public:
	explicit AudioTest(orc::LayerContext& layerContext);
	~AudioTest() override;

	void onAttach() override;
	void onDetach() override;

	void onUpdate(float deltaTime) override;
	void onEvent(const orc::Event& event) override;

	void onRender() override;
	void onGuiRender() override;

private:
	enum class Buses
	{
		Master,
		Music,
		Sfx
	};

	std::map<Buses, orc::UniquePtr<orc::Bus>> m_buses;
};
