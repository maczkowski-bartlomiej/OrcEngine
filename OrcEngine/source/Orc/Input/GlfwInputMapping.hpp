#pragma once

#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"

namespace orc::glfw {

Keyboard::Key glfwKeyToOrcKey(int glfwKey, int mods);
Keyboard::SpecialKeys glfwKeyModsToOrcSpecialKeys(int mods);
Mouse::Button glfwButtonToOrcButton(int glfwButton);

}
