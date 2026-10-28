#pragma once

#include "Events/Event.hpp"
#include "Input/Keyboard.hpp"

namespace orc {

struct KeyboardKeyPressedEvent : public Event
{
	explicit KeyboardKeyPressedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys())
		: Event(Type::KeyboardKeyPressed), key(key), specialKeys(specialKeys)
	{
	}

	Keyboard::Key key = Keyboard::Key::Invalid;
	Keyboard::SpecialKeys specialKeys{};
};

struct KeyboardKeyReleasedEvent : public Event
{
	explicit KeyboardKeyReleasedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys())
		: Event(Type::KeyboardKeyReleased), key(key), specialKeys(specialKeys)
	{
	}

	Keyboard::Key key = Keyboard::Key::Invalid;
	Keyboard::SpecialKeys specialKeys{};
};

}
