#pragma once

#include "Engine/Core.hpp"

#include <string_view>

#include <sol/sol.hpp>

namespace orc { 

class LuaManager
{
public:
	bool init();
	void deinit();

	void loadScript(const FilePath& filePath);
	void executeScript(const FilePath& filePath);

	template<typename ReturnValue = void, typename... Args>
	decltype(auto) callFunction(std::string_view name, Args&&... args);

	template<typename Class, typename... Args>
	void registerClass(Args&& ...args);

	template<typename Function>
	void registerFunction(std::string_view name, Function&& function);

private:
	sol::state lua;
};
	
template<typename ReturnValue, typename ...Args>
inline decltype(auto) LuaManager::callFunction(std::string_view name, Args&& ...args)
{
	sol::protected_function luaFunction = lua[name];
	if (luaFunction)
	{
		auto result = luaFunction(std::forward<Args>(args)...);
		if (result.valid())
		{
			if constexpr (std::is_void<ReturnValue>::value) return void;
			else result.get<ReturnValue>(); //check without check
		}
		else
		{
			ORC_LOG_WARNING("Failed '{}' script execution", name);
		}
	}
	else
	{
		ORC_LOG_WARNING("Couldn't find script '{}'", name);
	}
}

template<typename Class, typename ...Args>
inline void LuaManager::registerClass(Args&& ...args)
{
	lua.new_usertype<Class>(std::forward<Args>(args)...);
}

template<typename Function>
inline void LuaManager::registerFunction(std::string_view name, Function&& function)
{
	lua.set_function(name, function);
}

}
