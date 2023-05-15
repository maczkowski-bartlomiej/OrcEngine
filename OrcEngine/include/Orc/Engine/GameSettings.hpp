#pragma once

#include "System/System.hpp"
#include "Graphics/Window.hpp"

#include <string>

namespace orc {

struct GameSettings
{
	int32 majorVersion = 0u;
	int32 minorVersion = 0u;
	int32 patchVersion = 1u;

	std::string gameName = "orcEngine Game";

	std::string logPath = "log.txt";
	std::string loggerName = "GAME";

	Window::VideoSettings videoSettings;
};

}
