#pragma once

namespace orc {

class Mouse
{
public:
	enum class Button
	{
		Invalid = -1,
		Left, Right, Scroll,
		Extra1, Extra2, Extra3, Extra4, Extra5
	};

	static bool isButtonPressed(Button button);
};

namespace glfw {
	Mouse::Button glfwButtonToorcButton(int glfwButton);
	int orcButtonToGlfwButton(Mouse::Button button);
}

}
