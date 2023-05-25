#include "OrcPch.hpp"

#include "Input/Keyboard.hpp"

#include "Engine/Engine.hpp"

#include <GLFW/glfw3.h>

namespace orc {

bool Keyboard::isKeyPressed(Key key)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindow());
	int isPressed = glfwGetKey(window, glfw::orcKeyToGlfwKey(key));
	return isPressed == GLFW_PRESS;
}

namespace glfw {

Keyboard::Key glfwKeyToOrcKey(int glfwKey, int mods)
{
	switch (glfwKey)
	{
		case GLFW_KEY_UNKNOWN: return Keyboard::Key::Invalid;

		case GLFW_KEY_F1: return Keyboard::Key::F1;
		case GLFW_KEY_F2: return Keyboard::Key::F2;
		case GLFW_KEY_F3: return Keyboard::Key::F3;
		case GLFW_KEY_F4: return Keyboard::Key::F4;
		case GLFW_KEY_F5: return Keyboard::Key::F5;
		case GLFW_KEY_F6: return Keyboard::Key::F6;
		case GLFW_KEY_F7: return Keyboard::Key::F7;
		case GLFW_KEY_F8: return Keyboard::Key::F8;
		case GLFW_KEY_F9: return Keyboard::Key::F9;
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

		case GLFW_KEY_0: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::RightParenthesis : Keyboard::Key::Zero;
		case GLFW_KEY_1: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::ExlamationMark : Keyboard::Key::One;
		case GLFW_KEY_2: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Dollar : Keyboard::Key::Four;
		case GLFW_KEY_5: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Percentage : Keyboard::Key::Five;
		case GLFW_KEY_6: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Caret : Keyboard::Key::Six;
		case GLFW_KEY_7: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Apersand : Keyboard::Key::Seven;
		case GLFW_KEY_8: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Asterisk : Keyboard::Key::Eight;
		case GLFW_KEY_9: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::LeftParenthesis : Keyboard::Key::Nine;

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
		case GLFW_KEY_KP_DIVIDE: return Keyboard::Key::NumpadDivide;
		case GLFW_KEY_KP_SUBTRACT: return Keyboard::Key::NumpadMinus;
		case GLFW_KEY_KP_ADD: return Keyboard::Key::NumpadAdd;
		case GLFW_KEY_KP_ENTER: return Keyboard::Key::NumpadEnter;
		case GLFW_KEY_KP_DECIMAL: return Keyboard::Key::NumpadDecimal;
		case GLFW_KEY_KP_EQUAL: return Keyboard::Key::NumpadEqual;

		case GLFW_KEY_UP: return Keyboard::Key::Up;
		case GLFW_KEY_DOWN: return Keyboard::Key::Down;
		case GLFW_KEY_LEFT: return Keyboard::Key::Left;
		case GLFW_KEY_RIGHT: return Keyboard::Key::Right;

		case GLFW_KEY_LEFT_ALT: return Keyboard::Key::LeftAlt;
		case GLFW_KEY_LEFT_SHIFT: return Keyboard::Key::LeftShift;
		case GLFW_KEY_LEFT_SUPER: return Keyboard::Key::LeftSystem;
		case GLFW_KEY_LEFT_CONTROL: return Keyboard::Key::LeftControl;
		case GLFW_KEY_LEFT_BRACKET: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::LeftBrace : Keyboard::Key::LeftBracket;

		case GLFW_KEY_RIGHT_ALT: return Keyboard::Key::RightAlt;
		case GLFW_KEY_RIGHT_SHIFT: return Keyboard::Key::RightShift;
		case GLFW_KEY_RIGHT_SUPER: return Keyboard::Key::RightSystem;
		case GLFW_KEY_RIGHT_CONTROL: return Keyboard::Key::RightControl;
		case GLFW_KEY_RIGHT_BRACKET: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::RightBrace : Keyboard::Key::RightBracket;

		case GLFW_KEY_COMMA: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Less : Keyboard::Key::Comma;
		case GLFW_KEY_EQUAL: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Plus : Keyboard::Key::Equal;
		case GLFW_KEY_MINUS: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Underscore : Keyboard::Key::Minus;
		case GLFW_KEY_PERIOD: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Greater : Keyboard::Key::Period;

		case GLFW_KEY_SEMICOLON: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Colon : Keyboard::Key::Semicolon;
		case GLFW_KEY_GRAVE_ACCENT: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Tilde : Keyboard::Key::Grave;
		case GLFW_KEY_SLASH: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::QuestionMark : Keyboard::Key::Slash;
		case GLFW_KEY_APOSTROPHE: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::Quote : Keyboard::Key::Apostrophe;
		case GLFW_KEY_BACKSLASH: return mods & GLFW_MOD_SHIFT ? Keyboard::Key::VerticalBar : Keyboard::Key::Backslash;

		case GLFW_KEY_SPACE: return Keyboard::Key::Space;
		case GLFW_KEY_ENTER: return Keyboard::Key::Enter;
		case GLFW_KEY_NUM_LOCK: return Keyboard::Key::NumLock;
		case GLFW_KEY_CAPS_LOCK: return Keyboard::Key::CapsLock;
		case GLFW_KEY_SCROLL_LOCK: return Keyboard::Key::ScrollLock;
		case GLFW_KEY_PAUSE: return Keyboard::Key::PauseBreak;
		case GLFW_KEY_PRINT_SCREEN: return Keyboard::Key::PrintScreen;

		case GLFW_KEY_END: return Keyboard::Key::End;
		case GLFW_KEY_TAB: return Keyboard::Key::Tab;
		case GLFW_KEY_MENU: return Keyboard::Key::Menu;
		case GLFW_KEY_HOME: return Keyboard::Key::Home;
		case GLFW_KEY_DELETE: return Keyboard::Key::Delete;
		case GLFW_KEY_ESCAPE: return Keyboard::Key::Escape;
		case GLFW_KEY_INSERT: return Keyboard::Key::Insert;
		case GLFW_KEY_PAGE_UP: return Keyboard::Key::PageUp;
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
			case GLFW_MOD_SHIFT:		specialKeys.shift = true;		break;
			case GLFW_MOD_CONTROL:		specialKeys.control = true;		break;
			case GLFW_MOD_ALT:			specialKeys.alt = true;			break;
			case GLFW_MOD_SUPER:		specialKeys.system = true;		break;
			case GLFW_MOD_CAPS_LOCK:	specialKeys.capsLock = true;	break;
			case GLFW_MOD_NUM_LOCK:		specialKeys.numLock = true;		break;
		}

		mods &= ~mask;
		mask <<= 1;
	}

	return specialKeys;
}

int orcKeyToGlfwKey(Keyboard::Key key)
{
	switch (key)
	{
		case Keyboard::Key::F1: return GLFW_KEY_F1;
		case Keyboard::Key::F2: return GLFW_KEY_F2;
		case Keyboard::Key::F3: return GLFW_KEY_F3;
		case Keyboard::Key::F4: return GLFW_KEY_F4;
		case Keyboard::Key::F5: return GLFW_KEY_F5;
		case Keyboard::Key::F6: return GLFW_KEY_F6;
		case Keyboard::Key::F7: return GLFW_KEY_F7;
		case Keyboard::Key::F8: return GLFW_KEY_F8;
		case Keyboard::Key::F9: return GLFW_KEY_F9;
		case Keyboard::Key::F10: return GLFW_KEY_F10;
		case Keyboard::Key::F11: return GLFW_KEY_F11;
		case Keyboard::Key::F12: return GLFW_KEY_F12;
		case Keyboard::Key::F13: return GLFW_KEY_F13;
		case Keyboard::Key::F14: return GLFW_KEY_F14;
		case Keyboard::Key::F15: return GLFW_KEY_F15;
		case Keyboard::Key::F16: return GLFW_KEY_F16;
		case Keyboard::Key::F17: return GLFW_KEY_F17;
		case Keyboard::Key::F18: return GLFW_KEY_F18;
		case Keyboard::Key::F19: return GLFW_KEY_F19;
		case Keyboard::Key::F20: return GLFW_KEY_F20;
		case Keyboard::Key::F21: return GLFW_KEY_F21;
		case Keyboard::Key::F22: return GLFW_KEY_F22;
		case Keyboard::Key::F23: return GLFW_KEY_F23;
		case Keyboard::Key::F24: return GLFW_KEY_F24;
		case Keyboard::Key::F25: return GLFW_KEY_F25;

		case Keyboard::Key::RightParenthesis:
		case Keyboard::Key::Zero:  return GLFW_KEY_0;
		case Keyboard::Key::ExlamationMark:
		case Keyboard::Key::One:   return GLFW_KEY_1;
		case Keyboard::Key::Dollar:
		case Keyboard::Key::Four:  return GLFW_KEY_2;
		case Keyboard::Key::Percentage:
		case Keyboard::Key::Five:  return GLFW_KEY_5;
		case Keyboard::Key::Caret:
		case Keyboard::Key::Six:   return GLFW_KEY_6;
		case Keyboard::Key::Apersand:
		case Keyboard::Key::Seven: return GLFW_KEY_7;
		case Keyboard::Key::Asterisk:
		case Keyboard::Key::Eight: return GLFW_KEY_8;
		case Keyboard::Key::LeftParenthesis:
		case Keyboard::Key::Nine:  return GLFW_KEY_9;

		case Keyboard::Key::A: return GLFW_KEY_A;
		case Keyboard::Key::B: return GLFW_KEY_B;
		case Keyboard::Key::C: return GLFW_KEY_C;
		case Keyboard::Key::D: return GLFW_KEY_D;
		case Keyboard::Key::E: return GLFW_KEY_E;
		case Keyboard::Key::F: return GLFW_KEY_F;
		case Keyboard::Key::G: return GLFW_KEY_G;
		case Keyboard::Key::H: return GLFW_KEY_H;
		case Keyboard::Key::I: return GLFW_KEY_I;
		case Keyboard::Key::J: return GLFW_KEY_J;
		case Keyboard::Key::K: return GLFW_KEY_K;
		case Keyboard::Key::L: return GLFW_KEY_L;
		case Keyboard::Key::M: return GLFW_KEY_M;
		case Keyboard::Key::N: return GLFW_KEY_N;
		case Keyboard::Key::O: return GLFW_KEY_O;
		case Keyboard::Key::P: return GLFW_KEY_P;
		case Keyboard::Key::Q: return GLFW_KEY_Q;
		case Keyboard::Key::R: return GLFW_KEY_R;
		case Keyboard::Key::S: return GLFW_KEY_S;
		case Keyboard::Key::T: return GLFW_KEY_T;
		case Keyboard::Key::U: return GLFW_KEY_U;
		case Keyboard::Key::V: return GLFW_KEY_V;
		case Keyboard::Key::W: return GLFW_KEY_W;
		case Keyboard::Key::X: return GLFW_KEY_X;
		case Keyboard::Key::Y: return GLFW_KEY_Y;
		case Keyboard::Key::Z: return GLFW_KEY_Z;

		case Keyboard::Key::Numpad0: return GLFW_KEY_KP_0;
		case Keyboard::Key::Numpad1: return GLFW_KEY_KP_1;
		case Keyboard::Key::Numpad2: return GLFW_KEY_KP_2;
		case Keyboard::Key::Numpad3: return GLFW_KEY_KP_3;
		case Keyboard::Key::Numpad4: return GLFW_KEY_KP_4;
		case Keyboard::Key::Numpad5: return GLFW_KEY_KP_5;
		case Keyboard::Key::Numpad6: return GLFW_KEY_KP_6;
		case Keyboard::Key::Numpad7: return GLFW_KEY_KP_7;
		case Keyboard::Key::Numpad8: return GLFW_KEY_KP_8;
		case Keyboard::Key::Numpad9: return GLFW_KEY_KP_9;

		case Keyboard::Key::NumpadMultiply: return GLFW_KEY_KP_MULTIPLY;
		case Keyboard::Key::NumpadDivide: return GLFW_KEY_KP_DIVIDE;
		case Keyboard::Key::NumpadMinus: return GLFW_KEY_KP_SUBTRACT;
		case Keyboard::Key::NumpadAdd: return GLFW_KEY_KP_ADD;
		case Keyboard::Key::NumpadEnter: return GLFW_KEY_KP_ENTER;
		case Keyboard::Key::NumpadDecimal: return GLFW_KEY_KP_DECIMAL;
		case Keyboard::Key::NumpadEqual: return GLFW_KEY_KP_EQUAL;

		case Keyboard::Key::Up: return GLFW_KEY_UP;
		case Keyboard::Key::Down: return GLFW_KEY_DOWN;
		case Keyboard::Key::Left: return GLFW_KEY_LEFT;
		case Keyboard::Key::Right: return GLFW_KEY_RIGHT;

		case Keyboard::Key::LeftAlt: return GLFW_KEY_LEFT_ALT;
		case Keyboard::Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
		case Keyboard::Key::LeftSystem: return GLFW_KEY_LEFT_SUPER;
		case Keyboard::Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
		case Keyboard::Key::LeftBrace:
		case Keyboard::Key::LeftBracket: return GLFW_KEY_LEFT_BRACKET;

		case Keyboard::Key::RightAlt: return GLFW_KEY_RIGHT_ALT;
		case Keyboard::Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;
		case Keyboard::Key::RightSystem: return GLFW_KEY_RIGHT_SUPER;
		case Keyboard::Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
		case Keyboard::Key::RightBrace:
		case Keyboard::Key::RightBracket: return GLFW_KEY_RIGHT_BRACKET;

		case Keyboard::Key::Less:
		case Keyboard::Key::Comma: return GLFW_KEY_COMMA;
		case Keyboard::Key::Plus:
		case Keyboard::Key::Equal: return GLFW_KEY_EQUAL;
		case Keyboard::Key::Underscore:
		case Keyboard::Key::Minus: return GLFW_KEY_MINUS;
		case Keyboard::Key::Greater:
		case Keyboard::Key::Period: return GLFW_KEY_PERIOD;
		case Keyboard::Key::Colon:
		case Keyboard::Key::Semicolon: return GLFW_KEY_SEMICOLON;
		case Keyboard::Key::Tilde:
		case Keyboard::Key::Grave: return GLFW_KEY_GRAVE_ACCENT;
		case Keyboard::Key::QuestionMark:
		case Keyboard::Key::Slash: return GLFW_KEY_SLASH;
		case Keyboard::Key::Quote:
		case Keyboard::Key::Apostrophe: return GLFW_KEY_APOSTROPHE;
		case Keyboard::Key::VerticalBar:
		case Keyboard::Key::Backslash: return GLFW_KEY_BACKSLASH;


		case Keyboard::Key::Space: return GLFW_KEY_SPACE;
		case Keyboard::Key::Enter: return GLFW_KEY_ENTER;
		case Keyboard::Key::NumLock: return GLFW_KEY_NUM_LOCK;
		case Keyboard::Key::CapsLock: return GLFW_KEY_CAPS_LOCK;
		case Keyboard::Key::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
		case Keyboard::Key::PauseBreak: return GLFW_KEY_PAUSE;
		case Keyboard::Key::PrintScreen: return GLFW_KEY_PRINT_SCREEN;

		case Keyboard::Key::End: return GLFW_KEY_END;
		case Keyboard::Key::Tab: return GLFW_KEY_TAB;
		case Keyboard::Key::Menu: return GLFW_KEY_MENU;
		case Keyboard::Key::Home: return GLFW_KEY_HOME;
		case Keyboard::Key::Delete: return GLFW_KEY_DELETE;
		case Keyboard::Key::Escape: return GLFW_KEY_ESCAPE;
		case Keyboard::Key::Insert: return GLFW_KEY_INSERT;
		case Keyboard::Key::PageUp: return GLFW_KEY_PAGE_UP;
		case Keyboard::Key::PageDown: return GLFW_KEY_PAGE_DOWN;
		case Keyboard::Key::Backspace: return GLFW_KEY_BACKSPACE;

		default: return GLFW_KEY_UNKNOWN;
	}
}

}
}
