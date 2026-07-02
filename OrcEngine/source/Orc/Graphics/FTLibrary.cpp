#include "OrcPch.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
#include "Graphics/FTLibrary.hpp"
#include "Engine/Utility.hpp"

#include <freetype/freetype.h>


namespace orc {

FTLibrary::FTLibrary()
{
	ORC_LOG_INFO("Initializing FreeType Libary.");
	
	if (!ftCall(FT_Init_FreeType(&m_ft)))
	{
		ORC_LOG_FATAL("Failed to initialize FreeType Library.");
	}
}

FTLibrary::~FTLibrary()
{
	ORC_LOG_INFO("Deinitializing FreeType Libary.");

	if (!ftCall(FT_Done_FreeType(m_ft)))
	{
		ORC_LOG_ERROR("Failed to deinitialize FreeType Library.");
	}
}

FT_Library FTLibrary::getNativeLibrary() const
{
	return m_ft;
}

}
