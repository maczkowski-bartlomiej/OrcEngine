#include "OrcPch.hpp"
#include "Graphics/FTLibrary.hpp"
#include "Engine/Debug.hpp"

#define FT_CONFIG_OPTION_ERROR_STRINGS 
#include <freetype/freetype.h>

namespace orc {

bool FTLibrary::init()
{
	ORC_LOG_INFO("Initializing FreeType Libary...");
	if (FT_Init_FreeType(&m_ft) != 0)
	{
		ORC_FATAL("Failed to initialize FreeType Library");
		return false;
	}

	return true;
}

void FTLibrary::deinit()
{
	ORC_LOG_INFO("Deinitializing FreeType Libary...");
	if (FT_Done_FreeType(m_ft) != 0)
	{
		ORC_FATAL("Failed to deinitialize FreeType Library");
	}
}

FT_Library FTLibrary::getNativeLibrary()
{
	return m_ft;
}

}
