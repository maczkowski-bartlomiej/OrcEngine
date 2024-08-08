#pragma once

#include <imgui.h>

namespace orc {

class Gui
{
public:
	bool init();
	void deinit();

	void begin();
	void end();
};

}
