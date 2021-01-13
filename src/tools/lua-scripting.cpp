#include "lua-scripting.h"
#include "core/common-defs.h"
#include "tools/lua-functions.h"

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
    }

    void LuaScripting::ExecuteScript(std::string path)
    {
        if (luaL_dofile(L, path.c_str()))
        {
#ifdef LEEP_DEBUG
            const char *err = lua_tostring(L, -1);
            LEEP_CORE_ERROR("{0}", err);
#endif
            lua_pop(L, 1);
        }
    }
}
