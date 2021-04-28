#ifndef __LEEP_TOOLS_LUA_FUNCTIONS_H__
#define __LEEP_TOOLS_LUA_FUNCTIONS_H__

#include <stdint.h>
#include <../src/lua/lua.hpp>

namespace leep
{
    int32_t LUA_CreateRenderable(lua_State *L);    
    int32_t LUA_RemoveEntity(lua_State *L);
    int32_t LUA_SetParent(lua_State *L);
    int32_t LUA_DetachFromParent(lua_State *L);
    int32_t LUA_SetLocation(lua_State *L);
    int32_t LUA_SetSunDirection(lua_State *L);
    int32_t LUA_SetSunIntensity(lua_State *L);
};

#endif // __LEEP_TOOLS_LUA_FUNCTIONS_H__
