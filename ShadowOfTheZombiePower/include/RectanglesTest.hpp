#pragma once

#include <Orc/Orc.hpp>

class RectanglesTest : public orc::GameLayer
{
public:
	RectanglesTest();
	~RectanglesTest();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(orc::Event& event) override;

private:
	void drawLines();

	orc::Rectangle m_rectangle1;
	orc::Rectangle m_rectangle2;
	orc::Rectangle m_rectangle3;
	orc::Rectangle m_rectangle4;
	orc::Rectangle m_rectangle5;
	orc::Rectangle m_rectangle6;
	orc::Rectangle m_rectangle7;
	orc::Rectangle m_rectangle8;
	orc::Rectangle m_rectangle9;
	orc::Rectangle m_rectangle10;
	orc::Rectangle m_rectangle11;
	orc::Rectangle m_rectangle12;
	orc::Rectangle m_rectangle13;

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
