#pragma once

#include "Events/Event.hpp"
#include "Input/Keyboard.hpp"

namespace orc {

struct KeyboardKeyPressedEvent final : public EventT<Event::Type::KeyboardKeyPressed>
{
	KeyboardKeyPressedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: key(key), specialKeys(specialKeys) {}

	const Keyboard::Key key;
	const Keyboard::SpecialKeys specialKeys;
};

struct KeyboardKeyReleasedEvent final : public EventT<Event::Type::KeyboardKeyReleased>
{
	KeyboardKeyReleasedEvent(Keyboard::Key key, Keyboard::SpecialKeys specialKeys = Keyboard::SpecialKeys()) 
		: key(key), specialKeys(specialKeys) {}

	const Keyboard::Key key;
	const Keyboard::SpecialKeys specialKeys;
};

}
