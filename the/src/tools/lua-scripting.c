#include "lua-scripting.h"
#include "core/definitions.h"
#include "tools/lua-functions.h"

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

static lua_State *L;

void THE_ScriptingInit()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "CreateEntity", LUA_CreateEntity);
	lua_register(L, "SetLocation", LUA_SetLocation);
	lua_register(L, "SetSunDirection", LUA_SetSunDirection);
	lua_register(L, "SetSunIntensity", LUA_SetSunIntensity);
}

void THE_ScriptingExecute(const char *path)
{
	if (luaL_dofile(L, path)) {
#ifdef THE_DEBUG
		const char *err = lua_tostring(L, -1);
		THE_LOG_ERROR("%s", err);
#endif
		lua_pop(L, 1);
	}
}

void THE_ScriptingCommand(const char *command)
{
	if (luaL_dostring(L, command)) {
#ifdef THE_DEBUG 
		const char *err = lua_tostring(L, -1);
		THE_LOG_ERROR("%s", err);
#endif
		lua_pop(L, 1);
	}
}

void THE_ScriptingSetGlobal(const char *name, float value)
{
	lua_pushnumber(L, value);
	lua_setglobal(L, name);
}
