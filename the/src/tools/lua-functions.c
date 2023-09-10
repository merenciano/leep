#include "lua-functions.h"
#include "tools/helper-functions.h"

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

int32_t LUA_CreateEntity(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 2) {
		return luaL_error(L, "Invalid call, expecting three arguments");
	}

	const char *mesh = lua_tostring(L, 1);
	const char *tex = lua_tostring(L, 2);
	THE_CreateEntity(mesh, tex);
	lua_pop(L, 1);
	return 0;
}

int32_t LUA_SetLocation(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 4) {
		return luaL_error(L, "Invalid call, expecting four arguments");
	}

	int32_t entity_idx = (int32_t)lua_tointeger(L, 1);
	float new_x = (float)lua_tonumber(L, 2);
	float new_y = (float)lua_tonumber(L, 3);
	float new_z = (float)lua_tonumber(L, 4);
	THE_SetLocation(entity_idx, new_x, new_y, new_z);
	lua_pop(L, 1);
	return 0;
}

int32_t LUA_SetSunDirection(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 3) {
		return luaL_error(L, "Invalid call, expecting three arguments");
	}

	float new_x = (float)lua_tonumber(L, 1);
	float new_y = (float)lua_tonumber(L, 2);
	float new_z = (float)lua_tonumber(L, 3);
	THE_SetSunDirection(new_x, new_y, new_z);
	lua_pop(L, 1);
	return 0;
}

int32_t LUA_SetSunIntensity(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 1) {
		return luaL_error(L, "Invalid call, expecting one argument");
	}

	float new_intensity = (float)lua_tonumber(L, 1);
	THE_SetSunIntensity(new_intensity);
	lua_pop(L, 1);
	return 0;
}
