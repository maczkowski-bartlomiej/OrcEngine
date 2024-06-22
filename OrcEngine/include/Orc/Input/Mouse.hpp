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

	static Vector2f getPosition();
	static bool isButtonPressed(Button button);
};

namespace glfw {

	Mouse::Button glfwButtonToOrcButton(int glfwButton);
	int orcButtonToGlfwButton(Mouse::Button button);
}

}
