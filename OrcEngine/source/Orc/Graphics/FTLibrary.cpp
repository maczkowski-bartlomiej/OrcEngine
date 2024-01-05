#include "OrcPch.hpp"

#include "Graphics/FTLibrary.hpp"

#include "Engine/Debug.hpp"

namespace orc {

FT_Library FTLibrary::m_ft = nullptr;

void FTLibrary::init()
{
	static bool initialized = false;
	if (initialized)
	{
		ORC_FATAL("FTLibrary already initialized!!!");
		return;
	}

	initialized = true;

	ORC_FATAL_CHECK(!FT_Init_FreeType(&m_ft), "Fatal occured while initializing FreeType Library");
}

void FTLibrary::shutdown()
{
	ORC_FATAL_CHECK(!FT_Done_FreeType(m_ft), "Fatal occured while deinitializing FreeType Library");
}

FT_Library& FTLibrary::getLibrary()
{
	return m_ft;
}

}
