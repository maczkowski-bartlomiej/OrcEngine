#pragma once

#include "Input/Keyboard.hpp"
#include "Events/Event.hpp"

namespace orc {

struct KeyboardKeyPressedEvent : public Event
{
	KeyboardKeyPressedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: Event(Event::Type::KeyboardKeyPressed), key(key), specialKeys(specialKeys) {}

	const Keyboard::Key key;
	const Keyboard::SpecialKeys specialKeys;
};

struct KeyboardKeyReleasedEvent : public Event
{
	KeyboardKeyReleasedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: Event(Event::Type::KeyboardKeyReleased), key(key), specialKeys(specialKeys) {}

	const Keyboard::Key key;
	const Keyboard::SpecialKeys specialKeys;
};

}
