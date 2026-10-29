#pragma once

#include <imgui.h>

namespace orc {

class Window;

class Gui
{
public:
	explicit Gui(Window& window);
	~Gui();

	Gui(const Gui&) = delete;
	Gui& operator=(const Gui&) = delete;
	Gui(Gui&&) = delete;
	Gui& operator=(Gui&&) = delete;

	void begin();
	void end();

private:
	Window& m_window;
};

}
