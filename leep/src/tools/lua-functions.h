#ifndef THE_TOOLS_LUA_FUNCTIONS_H
#define THE_TOOLS_LUA_FUNCTIONS_H

#include <core/Cdefinitions.h>
#include <../src/lua/lua.hpp>

s32 LUA_CreateRenderable(lua_State *L);    
s32 LUA_RemoveEntity(lua_State *L);
s32 LUA_SetParent(lua_State *L);
s32 LUA_DetachFromParent(lua_State *L);
s32 LUA_SetLocation(lua_State *L);
s32 LUA_SetSunDirection(lua_State *L);
s32 LUA_SetSunIntensity(lua_State *L);

#endif
