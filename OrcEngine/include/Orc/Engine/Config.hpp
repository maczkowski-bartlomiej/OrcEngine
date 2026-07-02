#pragma once

#include "Audio/Audio.hpp"
#include "Graphics/Window.hpp"

#include <string>
#include <cstdint>

namespace orc {

struct Config
{
	int32_t majorVersion = 0;
	int32_t minorVersion = 0;
	int32_t patchVersion = 1;

	std::string gameName = "OrcEngine Game";

	std::string logPath = "log.txt";

	std::string fontsPath = "assets/fonts.xml";
	std::string shadersPath = "assets/shaders.xml";
	std::string texturesPath = "assets/textures.xml";
	std::string animationsPath = "assets/animations.xml";

	Audio::AudioSettings audioSettings;
	Window::VideoSettings videoSettings;
};

}
	