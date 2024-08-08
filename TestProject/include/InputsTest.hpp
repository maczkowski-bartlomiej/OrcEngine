#pragma once

#include <Orc/Orc.hpp>

class InputsTest : public orc::GameLayer
{
public:
	InputsTest();
	~InputsTest();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(orc::Event& event) override;

	orc::Sprite m_player;
	orc::Sprite m_sprite2;
	orc::Sprite m_sprite3;

	orc::Text m_text1;
	orc::Text m_text2;

	orc::Rectangle m_rectangle1;
	//orc::Text m_text3;
	//orc::Text m_text4;
	float x = 0.0f;
};
