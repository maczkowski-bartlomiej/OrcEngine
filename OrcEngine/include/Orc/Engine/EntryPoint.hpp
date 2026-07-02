#pragma once

#include "Engine/Engine.hpp"
#include "Engine/Config.hpp"

extern orc::Config orc::getEngineConfig(); //user function callback
extern void orc::onEngineStart(Engine& engine); //user function callback

int main(int argc, char** argv);
