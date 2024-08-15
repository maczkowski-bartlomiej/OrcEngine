#include <Orc/Orc.hpp>

#include "Menu.hpp"

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

	gameSettings.audioSettings.maxChannels = 512;

	orc::Engine* engine = new Engine();
	if (!engine->init(gameSettings))
		return nullptr;

	ORC_LOG_INFO("Test Project v.{}.{}.{}", gameSettings.majorVersion, gameSettings.minorVersion, gameSettings.patchVersion);

	engine->getGameLayerManager().addLayer("Menu", orc::createRef<Menu>());
	engine->getGameLayerManager().setActiveLayer("Menu");

	return engine;
}
