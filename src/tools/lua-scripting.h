// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_TOOLS_LUA_SCRIPTING_H__
#define __LEEP_TOOLS_LUA_SCRIPTING_H__

#include <../src/lua/lua.hpp>
#include <string>

namespace leep
{
    class LuaScripting
    {
    public:
        LuaScripting() = delete;
        ~LuaScripting() = delete;

        static void Init();
        static void ExecuteScript(std::string path);

    private:
        static lua_State *L;
    };
}

#endif // __LEEP_TOOLS_LUA_SCRIPTING_H__