#pragma once

#include <Orc/Orc.hpp>

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

	orc::Reference<orc::Sprite> m_player;
	orc::Reference<orc::Sprite> m_zombie;
	orc::Reference<orc::Rectangle> m_rectangle;
	orc::Vector2f m_cameraPosition;
	orc::Vector2f m_position;
	orc::TextureHolder m_textureHolder;
};
