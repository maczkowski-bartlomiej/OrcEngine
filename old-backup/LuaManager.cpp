#include "OrcPch.hpp"

#include "Engine/LuaManager.hpp"

namespace orc {

bool LuaManager::init()
{
	lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
}

void LuaManager::deinit()
{
}

void LuaManager::loadScript(const FilePath& filePath)
{
	auto luaFunction = lua.load_file(filePath.string());
	luaFunction.valid()
}

void LuaManager::executeScript(const FilePath& filePath)
{
	
}

}
