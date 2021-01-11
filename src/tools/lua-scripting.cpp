#include "lua-scripting.h"
#include "core/common-defs.h"

namespace leep
{
    lua_State *LuaScripting::L;
    void LuaScripting::Init()
    {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    void LuaScripting::ExecuteScript(std::string path)
    {
        if (luaL_dofile(L, path.c_str()))
        {
#ifdef LEEP_DEBUG
            const char *err = lua_tostring(L, -1);
            LEEP_CORE_ERROR("{0}", err);
            lua_pop(L, 1);
#endif
        }
    }
}