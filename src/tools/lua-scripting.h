// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_TOOLS_LUA_SCRIPTING_H__
#define __LEEP_TOOLS_LUA_SCRIPTING_H__

#include "core/string.h"
#include <../src/lua/lua.hpp>

namespace leep
{
    class LuaScripting
    {
    public:
        LuaScripting() = delete;
        ~LuaScripting() = delete;

        static void Init();
        static void SetGlobal(String name, float value);
        static void ExecuteScript(String path);
        static void ExecuteCommand(String command);

    private:
        static lua_State *L;
    };
}

#endif // __LEEP_TOOLS_LUA_SCRIPTING_H__
