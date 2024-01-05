#pragma once

#include <Orc/Orc.hpp>

#include <Orc/Graphics/Font.hpp>

class Game : public orc::GameLayer
{
public:
	Game();
	~Game();

	void onAttach()  override;
	void onDetach()  override;
	void onUpdate(float deltaTime)  override;
	void onEvent(orc::Event& event)  override;

private:
	orc::Camera m_camera;

	orc::Ref<orc::Sprite> m_player;
	orc::Ref<orc::Sprite> m_zombie;
	orc::Ref<orc::Sprite> m_emptySprite;
	orc::Ref<orc::Rectangle> m_rectangle1;
	orc::Ref<orc::Rectangle> m_rectangle2;
	orc::Ref<orc::Rectangle> m_rectangle3;
	orc::Ref<orc::Rectangle> m_rectangle4;
	orc::Ref<orc::Rectangle> m_rectangle5;

	orc::Ref<orc::Circle> m_circle1;
	orc::Ref<orc::Circle> m_circle2;
	orc::Ref<orc::Circle> m_circle3;

	orc::Ref<orc::Sprite> m_sprite1;
	orc::Ref<orc::Sprite> m_sprite2;
	orc::Ref<orc::Sprite> m_sprite3;
	orc::Ref<orc::Sprite> m_sprite4;

	orc::Ref<orc::SoundBuffer> m_audioBuffer;

	orc::Ref<orc::Font> m_font;
	orc::Ref<orc::Text> m_text;
	orc::Ref<orc::Sprite> m_fontText;

	orc::Vector2f m_cameraPosition;
	orc::Vector2f m_position;
};
