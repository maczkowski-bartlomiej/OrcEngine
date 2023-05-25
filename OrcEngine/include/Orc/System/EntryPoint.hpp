#pragma once

#include "Engine/Engine.hpp"

#ifdef ORC_PLATFORM_WINDOWS

extern orc::Engine* orc::startEngine();
int main(int argc, char** argv);

#endif
