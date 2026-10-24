#pragma once

#include <imgui.h>

namespace orc {

class Window;

class Gui
{
public:
	explicit Gui(Window& window);
	~Gui();

	void begin();
	void end();

private:
	Window& m_window;
};

}
