#pragma once

#define FT_CONFIG_OPTION_ERROR_STRINGS 
#include <freetype/freetype.h>

namespace orc
{

class FTLibrary
{
public:
	static void init();
	static void shutdown();

	static FT_Library& getLibrary();

private:
	static FT_Library m_ft;
};

}
