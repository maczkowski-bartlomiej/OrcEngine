#include "Game.hpp"

orc::Engine* orc::startEngine()
{
	orc::GameSettings gameSettings;
	gameSettings.majorVersion = 0;
	gameSettings.minorVersion = 0;
	gameSettings.patchVersion = 1;

	gameSettings.gameName = "Test Project";
	gameSettings.logPath = "logs/TestProject.log";

	gameSettings.videoSettings.title = "Test Project";
	gameSettings.videoSettings.width = 800;
	gameSettings.videoSettings.height = 600;
	gameSettings.videoSettings.vsync = true;

	orc::Engine* engine = new Engine(gameSettings);
	ORC_LOG_INFO("Test Project v.{}.{}.{}", gameSettings.majorVersion, gameSettings.minorVersion, gameSettings.patchVersion);

	engine->getGameLayerManager().addGameLayer("game", orc::createRef<Game>());
	engine->getGameLayerManager().setActiveGameLayer("game");

	return engine;
}
