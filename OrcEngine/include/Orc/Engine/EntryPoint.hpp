#pragma once

#include "Engine/Engine.hpp"
#include "Engine/Config.hpp"

namespace orc {
	Config getEngineConfig(); //user function callback
	void onEngineStart(Engine& engine); //user function callback
}

int main(int argc, char** argv);
