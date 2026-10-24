#pragma once

#include "Engine/Config.hpp"
#include "Engine/Engine.hpp"

namespace orc {
Config getEngineConfig();			// user function callback
void onEngineStart(Engine& engine); // user function callback
}

int main(int argc, char** argv);
