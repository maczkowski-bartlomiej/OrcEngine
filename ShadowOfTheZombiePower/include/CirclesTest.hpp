#pragma once

#include <Orc/Orc.hpp>

class CirclesTest : public orc::GameLayer
{
public:
	CirclesTest();
	~CirclesTest();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(orc::Event& event) override;

private:
	void drawLines();

	orc::Circle m_circle1;
	orc::Circle m_circle2;
	orc::Circle m_circle3;
	orc::Circle m_circle4;
	orc::Circle m_circle5;
	orc::Circle m_circle6;
	orc::Circle m_circle7;
	orc::Circle m_circle8;
	orc::Circle m_circle9;
	orc::Circle m_circle10;
	orc::Circle m_circle11;
	orc::Circle m_circle12;
	orc::Circle m_circle13;

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
