#include "Game.hpp"

orc::Engine* orc::startEngine()
{
	orc::GameSettings gameSettings;
	gameSettings.majorVersion = 0;
	gameSettings.minorVersion = 0;
	gameSettings.patchVersion = 1;

	gameSettings.gameName = "Shadow of the Zombie Power";
	gameSettings.logPath = "logs/softzp.log";

	gameSettings.videoSettings.title = "Shadow of the Zombie Power";
	gameSettings.videoSettings.width = 800;
	gameSettings.videoSettings.height = 600;
	gameSettings.videoSettings.vsync = true;

	orc::Engine* engine = new Engine(gameSettings);
	ORC_LOG_INFO("Shadow of The Zombie Power v.{}.{}.{}", gameSettings.majorVersion, gameSettings.minorVersion, gameSettings.patchVersion);
	engine->pushGameLayer(new Game());
	return engine;
}
