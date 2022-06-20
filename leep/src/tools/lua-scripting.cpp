#include "lua-scripting.h"
#include "core/Cdefinitions.h"
#include "core/common-defs.h"
#include "tools/lua-functions.h"

#include <../src/lua/lua.hpp>

namespace leep
{
    lua_State *LuaScripting::L;
    void LuaScripting::Init()
    {
        L = luaL_newstate();
        luaL_openlibs(L);
        
        lua_register(L, "CreateRenderable", LUA_CreateRenderable);
        lua_register(L, "RemoveEntity", LUA_RemoveEntity);
        lua_register(L, "SetParent", LUA_SetParent);
        lua_register(L, "DetachFromParent", LUA_DetachFromParent);
        lua_register(L, "SetLocation", LUA_SetLocation);
        lua_register(L, "SetSunDirection", LUA_SetSunDirection);
        lua_register(L, "SetSunIntensity", LUA_SetSunIntensity);
    }

    void LuaScripting::ExecuteScript(String path)
    {
        if (luaL_dofile(L, path.c_str()))
        {
#ifdef LEEP_DEBUG
            const char *err = lua_tostring(L, -1);
            THE_LOG_ERROR("%s", err);
#endif
            lua_pop(L, 1);
        }
    }

    void LuaScripting::ExecuteCommand(String command)
    {
        if (luaL_dostring(L, command.c_str()))
        {
#ifdef LEEP_DEBUG
            const char *err = lua_tostring(L, -1);
            THE_LOG_ERROR("%s", err);
#endif
            lua_pop(L, 1);
        }
    }

    void LuaScripting::SetGlobal(String name, float value)
    {
        lua_pushnumber(L, value);
        lua_setglobal(L, name.c_str());
    }
}
