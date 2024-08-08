#pragma once

namespace orc {

class Keyboard
{
public:
	enum class Key
	{
		Invalid = -1,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		ExlamationMark, At, Hash, Dollar, Percentage, Caret, Apersand, Asterisk, LeftParenthesis, RightParenthesis,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
		NumpadMultiply, NumpadDivide, NumpadMinus, NumpadAdd, NumpadEnter, NumpadDecimal, NumpadEqual,
		Up, Down, Left, Right,
		LeftAlt, LeftShift, LeftSystem, LeftControl, LeftBracket, LeftBrace,
		RightAlt, RightShift, RightSystem, RightControl, RightBracket, RightBrace,
		Less, Plus, Minus, Equal, Greater,
		Colon, Grave, Tilde, Slash, Quote, Comma, Period, Backslash, Semicolon, Underscore, Apostrophe, VerticalBar, QuestionMark,
		Space, Enter, NumLock, CapsLock, ScrollLock, PauseBreak, PrintScreen,
		End, Tab, Menu, Home, Delete, Escape, Insert, PageUp, PageDown, Backspace,
	};

	struct SpecialKeys
	{
		bool alt = false;
		bool shift = false;
		bool system = false;
		bool control = false;
		bool numLock = false;
		bool capsLock = false;
	};

	static bool isKeyPressed(Key key);
};

namespace glfw {

	Keyboard::Key glfwKeyToOrcKey(int glfwKey, int mods);
	Keyboard::SpecialKeys glfwKeyModsToOrcSpecialKeys(int mods);

	int orcKeyToGlfwKey(Keyboard::Key key);
}

}
