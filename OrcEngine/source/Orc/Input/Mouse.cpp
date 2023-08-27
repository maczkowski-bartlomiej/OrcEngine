#include "OrcPch.hpp"

#include "Input/Mouse.hpp"

#include "Engine/Engine.hpp"

#include <GLFW/glfw3.h>

namespace orc {

Vector2f Mouse::getMousePosition()
{
	Vector2d mousePosition;
	GLFWwindow* window = static_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindow());
	glfwGetCursorPos(window, &mousePosition.x, &mousePosition.y);

	return (Vector2f)mousePosition;
}

bool Mouse::isButtonPressed(Button button)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindow());
	int isPressed = glfwGetMouseButton(window, glfw::orcButtonToGlfwButton(button));

	return isPressed == GLFW_PRESS;
}

namespace glfw {

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

int orcButtonToGlfwButton(Mouse::Button button)
{
	switch (button)
	{
		case Mouse::Button::Left:    return GLFW_MOUSE_BUTTON_LEFT;
		case Mouse::Button::Right:   return GLFW_MOUSE_BUTTON_RIGHT;
		case Mouse::Button::Scroll:  return GLFW_MOUSE_BUTTON_MIDDLE;
		case Mouse::Button::Extra1:  return GLFW_MOUSE_BUTTON_4;
		case Mouse::Button::Extra2:  return GLFW_MOUSE_BUTTON_5;
		case Mouse::Button::Extra3:  return GLFW_MOUSE_BUTTON_6;
		case Mouse::Button::Extra4:  return GLFW_MOUSE_BUTTON_7;
		case Mouse::Button::Extra5:  return GLFW_MOUSE_BUTTON_8;
		default: return GLFW_KEY_UNKNOWN;
	}
}

}
}
