#include <Orc/Orc.hpp>

#include "Menu.hpp"

orc::Config orc::getEngineConfig()
{
	orc::Config config;

	config.logPath = "logs/TestProject.log";

	config.videoSettings.title = "Test Project";
	config.videoSettings.width = 800;
	config.videoSettings.height = 600;
	config.videoSettings.vsync = true;

	config.audioSettings.maxChannels = 512;

	return config;
}

void orc::onEngineStart(orc::Engine& engine)
{
	ORC_LOG_INFO("Test Project v.{}.{}.{}", 0, 0, 1);

	engine.getGameLayerManager().addLayer("Menu", orc::createRef<Menu>());
	engine.getGameLayerManager().setActiveLayer("Menu");
}
