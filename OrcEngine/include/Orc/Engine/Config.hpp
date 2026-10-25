#pragma once

#include <cstdint>
#include <string>

#include "Audio/Audio.hpp"
#include "Core/Types.hpp"
#include "Graphics/Window.hpp"

namespace orc {

struct Config
{
	int32_t majorVersion = 1;
	int32_t minorVersion = 0;
	int32_t patchVersion = 0;

	std::string gameName = "OrcEngine Game";

	FilePath logPath = "log.txt";

	FilePath fontsPath = "assets/fonts.xml";
	FilePath shadersPath = "assets/shaders.xml";
	FilePath texturesPath = "assets/textures.xml";
	FilePath animationsPath = "assets/animations.xml";
	FilePath scriptingConfigPath = "OrcEngine.Scripting.runtimeconfig.json";

	Audio::AudioSettings audioSettings;
	Window::VideoSettings videoSettings;
};

}