#include "OrcPch.hpp"

#include "Engine/EntryPoint.hpp"

#ifdef ORC_PLATFORM_WINDOWS

int main(int, char**)
{
	orc::Engine* engine = nullptr;
	engine = orc::startEngine();

	if (engine)
	{
		engine->run();
		engine->deinit();
		delete engine;
	}
}

#endif
