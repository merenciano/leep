#ifndef THE_TOOLS_LUA_FUNCTIONS_H
#define THE_TOOLS_LUA_FUNCTIONS_H

#include "core/definitions.h"

typedef struct lua_State lua_State;

int32_t LUA_CreateEntity(lua_State *L);    
int32_t LUA_SetLocation(lua_State *L);
int32_t LUA_SetSunDirection(lua_State *L);
int32_t LUA_SetSunIntensity(lua_State *L);

#endif
