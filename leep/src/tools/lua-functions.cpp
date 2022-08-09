#include "lua-functions.h"
#include "tools/helper-functions.h"

s32 LUA_CreateRenderable(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 3) {
		return luaL_error(L, "Invalid call, expecting three arguments");
	}

	leep::String entity_name = lua_tostring(L, 1);
	leep::String geometry_name = lua_tostring(L, 2);
	leep::String texture_name = lua_tostring(L, 3);
	THE_CreateRenderable(entity_name, geometry_name, texture_name);
	lua_pop(L, 1);
	return 0;
}

s32 LUA_RemoveEntity(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 1) {
		return luaL_error(L, "Invalid call, expecting one argument");
	}

	leep::String entity_name = lua_tostring(L, 1);
	THE_RemoveEntity(entity_name);
	lua_pop(L, 1);
	return 0;
}

s32 LUA_SetParent(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 2) {
		return luaL_error(L, "Invalid call, expecting two arguments");
	}

	leep::String entity_name = lua_tostring(L, 1);
	leep::String parent_name = lua_tostring(L, 2);
	THE_SetParent(entity_name, parent_name);
	lua_pop(L, 1);
	return 0;
}

s32 LUA_DetachFromParent(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 1) {
		return luaL_error(L, "Invalid call, expecting one argument");
	}

	leep::String entity_name = lua_tostring(L, 1);
	THE_DetachFromParent(entity_name);
	lua_pop(L, 1);
	return 0;
}

s32 LUA_SetLocation(lua_State *L)
{
	int32_t args = lua_gettop(L);
	if (args != 4) {
		return luaL_error(L, "Invalid call, expecting four arguments");
	}

	leep::String entity_name = lua_tostring(L, 1);
	float new_x = (float)lua_tonumber(L, 2);
	float new_y = (float)lua_tonumber(L, 3);
	float new_z = (float)lua_tonumber(L, 4);
	THE_SetLocation(entity_name, new_x, new_y, new_z);
	lua_pop(L, 1);
	return 0;
}

s32 LUA_SetSunDirection(lua_State *L)
{
	s32 args = lua_gettop(L);
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

s32 LUA_SetSunIntensity(lua_State *L)
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
