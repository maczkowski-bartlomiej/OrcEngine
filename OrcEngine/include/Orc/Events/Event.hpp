#pragma once

#include "Events/WindowEvents.hpp"
#include "Events/KeyboardEvents.hpp"
#include "Events/MouseEvents.hpp"

#include <variant>
#include <type_traits>
#include <functional>
#include <ostream>

namespace orc {

// C++20 Overload helper for std::visit pattern matching
template<typename... Ts>
struct Overload : Ts...
{
	using Ts::operator()...;
};

template<typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

class Event
{
public:
	enum class Type
	{
		Invalid = -1,
		WindowResized, WindowClosed,
		KeyboardKeyPressed, KeyboardKeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseWheelScrolled
	};

	using EventVariant = std::variant<
		std::monostate,
		WindowResizedEvent,
		WindowClosedEvent,
		KeyboardKeyPressedEvent,
		KeyboardKeyReleasedEvent,
		MouseButtonPressedEvent,
		MouseButtonReleasedEvent,
		MouseMovedEvent,
		MouseWheelScrolledEvent
	>;

	Event() = default;

	template<typename T, typename = std::enable_if_t<
		!std::is_same_v<std::decay_t<T>, Event> &&
		std::is_constructible_v<EventVariant, T&&>
	>>
	Event(T&& val)
		: m_data(std::forward<T>(val))
	{
	}

	void setHandled(bool handled = true) const noexcept
	{
		m_handled = handled;
	}

	bool isHandled() const noexcept
	{
		return m_handled;
	}

	template<typename T>
	[[nodiscard]] bool is() const noexcept
	{
		return std::holds_alternative<T>(m_data);
	}

	template<typename T>
	[[nodiscard]] const T* getIf() const noexcept
	{
		return std::get_if<T>(&m_data);
	}

	template<typename T>
	[[nodiscard]] T* getIf() noexcept
	{
		return std::get_if<T>(&m_data);
	}

	template<typename T>
	[[nodiscard]] const T& as() const
	{
		return std::get<T>(m_data);
	}

	template<typename T>
	[[nodiscard]] T& as()
	{
		return std::get<T>(m_data);
	}

	template<typename... Visitors>
	decltype(auto) visit(Visitors&&... visitors) const
	{
		return std::visit(Overload{std::forward<Visitors>(visitors)...}, m_data);
	}

	template<typename... Visitors>
	decltype(auto) visit(Visitors&&... visitors)
	{
		return std::visit(Overload{std::forward<Visitors>(visitors)...}, m_data);
	}

	[[nodiscard]] const EventVariant& raw() const noexcept
	{
		return m_data;
	}

	[[nodiscard]] EventVariant& raw() noexcept
	{
		return m_data;
	}

	[[nodiscard]] Type getType() const
	{
		return std::visit(Overload{
			[](std::monostate) { return Type::Invalid; },
			[](const WindowResizedEvent&) { return Type::WindowResized; },
			[](const WindowClosedEvent&) { return Type::WindowClosed; },
			[](const KeyboardKeyPressedEvent&) { return Type::KeyboardKeyPressed; },
			[](const KeyboardKeyReleasedEvent&) { return Type::KeyboardKeyReleased; },
			[](const MouseButtonPressedEvent&) { return Type::MouseButtonPressed; },
			[](const MouseButtonReleasedEvent&) { return Type::MouseButtonReleased; },
			[](const MouseMovedEvent&) { return Type::MouseMoved; },
			[](const MouseWheelScrolledEvent&) { return Type::MouseWheelScrolled; }
		}, m_data);
	}

	[[nodiscard]] Type getEventType() const
	{
		return getType();
	}

private:
	EventVariant m_data;
	mutable bool m_handled = false;

	friend std::ostream& operator<<(std::ostream& os, const Event& event);
};

template<typename DerivedEvent>
[[nodiscard]] const DerivedEvent& getEvent(const Event& event)
{
	return event.as<DerivedEvent>();
}

class EventDispatcher
{
public:
	explicit EventDispatcher(const Event& event)
		: m_event(event) {}

	template<typename T, typename Func>
	bool dispatch(Func&& func)
	{
		if (const auto* ev = m_event.getIf<T>())
		{
			if constexpr (std::is_invocable_r_v<bool, Func, const T&>)
			{
				m_event.setHandled(func(*ev));
			}
			else
			{
				func(*ev);
				m_event.setHandled(true);
			}
			return true;
		}
		return false;
	}

private:
	const Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& event)
{
	os << "Event(type=" << static_cast<int>(event.getType()) << ").";
	return os;
}

}
