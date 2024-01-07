#pragma once

#include <Orc/Orc.hpp>

class SpritesTest : public orc::GameLayer
{
public:
	SpritesTest();
	~SpritesTest();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(orc::Event& event) override;

private:
	void drawLines();

	orc::Sprite m_sprite1;
	orc::Sprite m_sprite2;
	orc::Sprite m_sprite3;
	orc::Sprite m_sprite4;
	orc::Sprite m_sprite5;
	orc::Sprite m_sprite6;
	orc::Sprite m_sprite7;
	orc::Sprite m_sprite8;
	orc::Sprite m_sprite9;
	orc::Sprite m_sprite10;
	orc::Sprite m_sprite11;
	orc::Sprite m_sprite12;
	orc::Sprite m_sprite13;

	orc::Text m_text1;
	orc::Text m_text2;
	orc::Text m_text3;
	orc::Text m_text4;
	orc::Text m_text5;
	orc::Text m_text6;
	orc::Text m_text7;
	orc::Text m_text8;
	orc::Text m_text9;
	orc::Text m_text10;
	orc::Text m_text11;
	orc::Text m_text12;
	orc::Text m_text13;
};
