#pragma once

namespace orc {

class Mouse
{
public:
	// clang-format off
	enum class Button
	{
		Invalid = -1,
		Left, Right, Scroll,
		Extra1, Extra2, Extra3, Extra4, Extra5
	};
	// clang-format on
};

namespace glfw {
Mouse::Button glfwButtonToOrcButton(int glfwButton);
}

}
