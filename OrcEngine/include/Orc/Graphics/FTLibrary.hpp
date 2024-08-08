#pragma once

struct FT_LibraryRec_;
typedef FT_LibraryRec_* FT_Library;

namespace orc {

class FTLibrary
{
public:
	bool init();
	void deinit();

	FT_Library getNativeLibrary();

private:
	FT_Library m_ft;
};

}
