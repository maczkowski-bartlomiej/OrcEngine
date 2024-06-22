#pragma once

//absolute path doesn't work even it is added to additional include directories
#include "../dependencies/imgui/imgui.h"

namespace orc {

class Gui
{
public:
	Gui();
	~Gui();

	void begin();
	void end();

private:
	static Gui* m_instance;
};

}
