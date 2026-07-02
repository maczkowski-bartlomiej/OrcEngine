#pragma once

#define FT_CONFIG_OPTION_ERROR_STRINGS 
#include <freetype/freetype.h>

namespace orc {

class FTLibrary
{
public:
	FTLibrary();
	~FTLibrary();

	FT_Library getNativeLibrary() const;

private:
	FT_Library m_ft;
};

}
