#pragma once

#include "Engine/Engine.hpp"

#ifdef ORC_PLATFORM_WINDOWS

extern orc::Config orc::getEngineConfig(); //user function callback
extern void orc::onEngineStart(Engine& engine); //user function callback

int main(int argc, char** argv);

#endif
