#include "orcPch.hpp"
#include "System/EntryPoint.hpp"

#ifdef ORC_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	orc::Engine* engine = nullptr;
	try
	{
		engine = orc::startEngine();
		engine->run();
	}
	catch (std::exception exception)
	{
		if (strlen(exception.what()) > 0)
		{
			MessageBoxA(NULL, (LPCSTR)exception.what(), (LPCSTR)"Crash", MB_DEFBUTTON2);
		}
		else
		{
			MessageBoxA(NULL, (LPCSTR)"Crash occurred, more information in log", (LPCSTR)"Crash", MB_DEFBUTTON2);
		}
	}

	delete engine;
}

#endif
