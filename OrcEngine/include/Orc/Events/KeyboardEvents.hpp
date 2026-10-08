#pragma once

#include "Input/Keyboard.hpp"

namespace orc {

struct KeyboardKeyPressedEvent
{
	Keyboard::Key key = Keyboard::Key::Invalid;
	Keyboard::SpecialKeys specialKeys{};

	KeyboardKeyPressedEvent() = default;
	KeyboardKeyPressedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: key(key), specialKeys(specialKeys) {}
};

struct KeyboardKeyReleasedEvent
{
	Keyboard::Key key = Keyboard::Key::Invalid;
	Keyboard::SpecialKeys specialKeys{};

	KeyboardKeyReleasedEvent() = default;
	KeyboardKeyReleasedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: key(key), specialKeys(specialKeys) {}
};

}

