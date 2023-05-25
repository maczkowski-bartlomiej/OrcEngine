#include "Game.hpp"

class ShadowOfTheZombiePower : public orc::Engine
{
public:
	ShadowOfTheZombiePower(const orc::GameSettings& gameSettings)
		: orc::Engine(gameSettings)
	{
		ORC_LOG_INFO("Shadow of The Zombie Power v.{}.{}.{}", gameSettings.majorVersion, gameSettings.minorVersion, gameSettings.patchVersion);
		pushGameLayer(new Game());
	}

	~ShadowOfTheZombiePower()
	{
		ORC_LOG_INFO("Game shutting down...");
	}
};

orc::Engine* orc::startEngine()
{
	orc::GameSettings gameSettings;
	gameSettings.majorVersion = 0;
	gameSettings.minorVersion = 0;
	gameSettings.patchVersion = 1;

	gameSettings.gameName = "Shadow of the Zombie Power";
	gameSettings.loggerName = "SOFTZP";
	gameSettings.logPath = "logs/softzp.log";

	gameSettings.videoSettings.title = "Shadow of the Zombie Power";
	gameSettings.videoSettings.width = 800u;
	gameSettings.videoSettings.height = 600u;
	gameSettings.videoSettings.vsync = true;

	return new ShadowOfTheZombiePower(gameSettings);
}
