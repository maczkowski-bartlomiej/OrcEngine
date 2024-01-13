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

};
