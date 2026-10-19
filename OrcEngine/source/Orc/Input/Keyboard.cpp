#include "OrcPch.hpp"
#include "Input/Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace orc::glfw {

Keyboard::Key glfwKeyToOrcKey(int glfwKey, int /*mods*/)
{
	switch (glfwKey)
	{
		case GLFW_KEY_UNKNOWN: return Keyboard::Key::Invalid;

		case GLFW_KEY_F1:  return Keyboard::Key::F1;
		case GLFW_KEY_F2:  return Keyboard::Key::F2;
		case GLFW_KEY_F3:  return Keyboard::Key::F3;
		case GLFW_KEY_F4:  return Keyboard::Key::F4;
		case GLFW_KEY_F5:  return Keyboard::Key::F5;
		case GLFW_KEY_F6:  return Keyboard::Key::F6;
		case GLFW_KEY_F7:  return Keyboard::Key::F7;
		case GLFW_KEY_F8:  return Keyboard::Key::F8;
		case GLFW_KEY_F9:  return Keyboard::Key::F9;
		case GLFW_KEY_F10: return Keyboard::Key::F10;
		case GLFW_KEY_F11: return Keyboard::Key::F11;
		case GLFW_KEY_F12: return Keyboard::Key::F12;
		case GLFW_KEY_F13: return Keyboard::Key::F13;
		case GLFW_KEY_F14: return Keyboard::Key::F14;
		case GLFW_KEY_F15: return Keyboard::Key::F15;
		case GLFW_KEY_F16: return Keyboard::Key::F16;
		case GLFW_KEY_F17: return Keyboard::Key::F17;
		case GLFW_KEY_F18: return Keyboard::Key::F18;
		case GLFW_KEY_F19: return Keyboard::Key::F19;
		case GLFW_KEY_F20: return Keyboard::Key::F20;
		case GLFW_KEY_F21: return Keyboard::Key::F21;
		case GLFW_KEY_F22: return Keyboard::Key::F22;
		case GLFW_KEY_F23: return Keyboard::Key::F23;
		case GLFW_KEY_F24: return Keyboard::Key::F24;
		case GLFW_KEY_F25: return Keyboard::Key::F25;

		case GLFW_KEY_0: return Keyboard::Key::Zero;
		case GLFW_KEY_1: return Keyboard::Key::One;
		case GLFW_KEY_2: return Keyboard::Key::Two;
		case GLFW_KEY_3: return Keyboard::Key::Three;
		case GLFW_KEY_4: return Keyboard::Key::Four;
		case GLFW_KEY_5: return Keyboard::Key::Five;
		case GLFW_KEY_6: return Keyboard::Key::Six;
		case GLFW_KEY_7: return Keyboard::Key::Seven;
		case GLFW_KEY_8: return Keyboard::Key::Eight;
		case GLFW_KEY_9: return Keyboard::Key::Nine;

		case GLFW_KEY_A: return Keyboard::Key::A;
		case GLFW_KEY_B: return Keyboard::Key::B;
		case GLFW_KEY_C: return Keyboard::Key::C;
		case GLFW_KEY_D: return Keyboard::Key::D;
		case GLFW_KEY_E: return Keyboard::Key::E;
		case GLFW_KEY_F: return Keyboard::Key::F;
		case GLFW_KEY_G: return Keyboard::Key::G;
		case GLFW_KEY_H: return Keyboard::Key::H;
		case GLFW_KEY_I: return Keyboard::Key::I;
		case GLFW_KEY_J: return Keyboard::Key::J;
		case GLFW_KEY_K: return Keyboard::Key::K;
		case GLFW_KEY_L: return Keyboard::Key::L;
		case GLFW_KEY_M: return Keyboard::Key::M;
		case GLFW_KEY_N: return Keyboard::Key::N;
		case GLFW_KEY_O: return Keyboard::Key::O;
		case GLFW_KEY_P: return Keyboard::Key::P;
		case GLFW_KEY_Q: return Keyboard::Key::Q;
		case GLFW_KEY_R: return Keyboard::Key::R;
		case GLFW_KEY_S: return Keyboard::Key::S;
		case GLFW_KEY_T: return Keyboard::Key::T;
		case GLFW_KEY_U: return Keyboard::Key::U;
		case GLFW_KEY_V: return Keyboard::Key::V;
		case GLFW_KEY_W: return Keyboard::Key::W;
		case GLFW_KEY_X: return Keyboard::Key::X;
		case GLFW_KEY_Y: return Keyboard::Key::Y;
		case GLFW_KEY_Z: return Keyboard::Key::Z;

		case GLFW_KEY_KP_0: return Keyboard::Key::Numpad0;
		case GLFW_KEY_KP_1: return Keyboard::Key::Numpad1;
		case GLFW_KEY_KP_2: return Keyboard::Key::Numpad2;
		case GLFW_KEY_KP_3: return Keyboard::Key::Numpad3;
		case GLFW_KEY_KP_4: return Keyboard::Key::Numpad4;
		case GLFW_KEY_KP_5: return Keyboard::Key::Numpad5;
		case GLFW_KEY_KP_6: return Keyboard::Key::Numpad6;
		case GLFW_KEY_KP_7: return Keyboard::Key::Numpad7;
		case GLFW_KEY_KP_8: return Keyboard::Key::Numpad8;
		case GLFW_KEY_KP_9: return Keyboard::Key::Numpad9;

		case GLFW_KEY_KP_MULTIPLY: return Keyboard::Key::NumpadMultiply;
		case GLFW_KEY_KP_DIVIDE:   return Keyboard::Key::NumpadDivide;
		case GLFW_KEY_KP_SUBTRACT: return Keyboard::Key::NumpadMinus;
		case GLFW_KEY_KP_ADD:      return Keyboard::Key::NumpadAdd;
		case GLFW_KEY_KP_ENTER:    return Keyboard::Key::NumpadEnter;
		case GLFW_KEY_KP_DECIMAL:  return Keyboard::Key::NumpadDecimal;
		case GLFW_KEY_KP_EQUAL:    return Keyboard::Key::NumpadEqual;

		case GLFW_KEY_UP:    return Keyboard::Key::Up;
		case GLFW_KEY_DOWN:  return Keyboard::Key::Down;
		case GLFW_KEY_LEFT:  return Keyboard::Key::Left;
		case GLFW_KEY_RIGHT: return Keyboard::Key::Right;

		case GLFW_KEY_LEFT_ALT:     return Keyboard::Key::LeftAlt;
		case GLFW_KEY_LEFT_SHIFT:   return Keyboard::Key::LeftShift;
		case GLFW_KEY_LEFT_SUPER:   return Keyboard::Key::LeftSystem;
		case GLFW_KEY_LEFT_CONTROL: return Keyboard::Key::LeftControl;
		case GLFW_KEY_LEFT_BRACKET: return Keyboard::Key::LeftBracket;

		case GLFW_KEY_RIGHT_ALT:     return Keyboard::Key::RightAlt;
		case GLFW_KEY_RIGHT_SHIFT:   return Keyboard::Key::RightShift;
		case GLFW_KEY_RIGHT_SUPER:   return Keyboard::Key::RightSystem;
		case GLFW_KEY_RIGHT_CONTROL: return Keyboard::Key::RightControl;
		case GLFW_KEY_RIGHT_BRACKET: return Keyboard::Key::RightBracket;

		case GLFW_KEY_COMMA:  return Keyboard::Key::Comma;
		case GLFW_KEY_EQUAL:  return Keyboard::Key::Equal;
		case GLFW_KEY_MINUS:  return Keyboard::Key::Minus;
		case GLFW_KEY_PERIOD: return Keyboard::Key::Period;

		case GLFW_KEY_SEMICOLON:    return Keyboard::Key::Semicolon;
		case GLFW_KEY_GRAVE_ACCENT: return Keyboard::Key::Grave;
		case GLFW_KEY_SLASH:        return Keyboard::Key::Slash;
		case GLFW_KEY_APOSTROPHE:   return Keyboard::Key::Apostrophe;
		case GLFW_KEY_BACKSLASH:    return Keyboard::Key::Backslash;

		case GLFW_KEY_SPACE:        return Keyboard::Key::Space;
		case GLFW_KEY_ENTER:        return Keyboard::Key::Enter;
		case GLFW_KEY_NUM_LOCK:     return Keyboard::Key::NumLock;
		case GLFW_KEY_CAPS_LOCK:    return Keyboard::Key::CapsLock;
		case GLFW_KEY_SCROLL_LOCK:  return Keyboard::Key::ScrollLock;
		case GLFW_KEY_PAUSE:        return Keyboard::Key::PauseBreak;
		case GLFW_KEY_PRINT_SCREEN: return Keyboard::Key::PrintScreen;

		case GLFW_KEY_END:       return Keyboard::Key::End;
		case GLFW_KEY_TAB:       return Keyboard::Key::Tab;
		case GLFW_KEY_MENU:      return Keyboard::Key::Menu;
		case GLFW_KEY_HOME:      return Keyboard::Key::Home;
		case GLFW_KEY_DELETE:    return Keyboard::Key::Delete;
		case GLFW_KEY_ESCAPE:    return Keyboard::Key::Escape;
		case GLFW_KEY_INSERT:    return Keyboard::Key::Insert;
		case GLFW_KEY_PAGE_UP:   return Keyboard::Key::PageUp;
		case GLFW_KEY_PAGE_DOWN: return Keyboard::Key::PageDown;
		case GLFW_KEY_BACKSPACE: return Keyboard::Key::Backspace;

		default: return Keyboard::Key::Invalid;
	}
}

Keyboard::SpecialKeys glfwKeyModsToOrcSpecialKeys(int mods)
{
	Keyboard::SpecialKeys specialKeys;

	int mask = 1;
	while (mods)
	{
		switch (mods & mask)
		{
			case GLFW_MOD_SHIFT:	 specialKeys.shift = true;		break;
			case GLFW_MOD_CONTROL:	 specialKeys.control = true;		break;
			case GLFW_MOD_ALT:		 specialKeys.alt = true;			break;
			case GLFW_MOD_SUPER:	 specialKeys.system = true;		break;
			case GLFW_MOD_CAPS_LOCK: specialKeys.capsLock = true;	break;
			case GLFW_MOD_NUM_LOCK:	 specialKeys.numLock = true;		break;
			default: break;
		}

		mods &= ~mask;
		mask <<= 1;
	}

	return specialKeys;
}

}

