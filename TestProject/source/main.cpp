#include <Orc/Orc.hpp>

#include "Menu.hpp"

orc::Engine* orc::startEngine()
{
	orc::Config config;
	config.majorVersion = 0;
	config.minorVersion = 0;
	config.patchVersion = 1;

	config.gameName = "Test Project";
	config.logPath = "logs/TestProject.log";

	config.videoSettings.title = "Test Project";
	config.videoSettings.width = 800;
	config.videoSettings.height = 600;
	config.videoSettings.vsync = true;

	config.audioSettings.maxChannels = 512;

	orc::Engine* engine = new Engine();
	if (!engine->init(config))
		return nullptr;

	ORC_LOG_INFO("Test Project v.{}.{}.{}", config.majorVersion, config.minorVersion, config.patchVersion);

	engine->getGameLayerManager().addLayer("Menu", orc::createRef<Menu>());
	engine->getGameLayerManager().setActiveLayer("Menu");

	return engine;
}
