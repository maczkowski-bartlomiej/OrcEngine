#include "OrcPch.hpp"
#include "Input/Mouse.hpp"

#include <GLFW/glfw3.h>

namespace orc::glfw {

Mouse::Button glfwButtonToOrcButton(int glfwButton)
{
	switch (glfwButton)
	{
		case GLFW_MOUSE_BUTTON_LEFT:   return Mouse::Button::Left;
		case GLFW_MOUSE_BUTTON_RIGHT:  return Mouse::Button::Right;
		case GLFW_MOUSE_BUTTON_MIDDLE: return Mouse::Button::Scroll;
		case GLFW_MOUSE_BUTTON_4:	   return Mouse::Button::Extra1;
		case GLFW_MOUSE_BUTTON_5:	   return Mouse::Button::Extra2;
		case GLFW_MOUSE_BUTTON_6:	   return Mouse::Button::Extra3;
		case GLFW_MOUSE_BUTTON_7:	   return Mouse::Button::Extra4;
		case GLFW_MOUSE_BUTTON_8:	   return Mouse::Button::Extra5;
		default:					   return Mouse::Button::Invalid;
	}
}

}

