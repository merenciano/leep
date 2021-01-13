#include "lua-functions.h"
#include "helper-functions.h"

#include <string>

int32_t leep::LUA_CreateRenderable(lua_State *L)
{
    int32_t args = lua_gettop(L);
    if (args != 1)
    {
        return luaL_error(L, "Invalid call, expecting one argument");
    }

    std::string entity_name = lua_tostring(L, 1);
    CreateRenderable(entity_name);
    lua_pop(L, 1);
    return 0;
}

int32_t leep::LUA_RemoveEntity(lua_State *L)
{
    int32_t args = lua_gettop(L);
    if (args != 1)
    {
        return luaL_error(L, "Invalid call, expecting one argument");
    }

    std::string entity_name = lua_tostring(L, 1);
    RemoveEntity(entity_name);
    lua_pop(L, 1);
    return 0;
}

int32_t leep::LUA_SetParent(lua_State *L)
{
    int32_t args = lua_gettop(L);
    if (args != 2)
    {
        return luaL_error(L, "Invalid call, expecting two arguments");
    }

    std::string entity_name = lua_tostring(L, 1);
    std::string parent_name = lua_tostring(L, 2);
    SetParent(entity_name, parent_name);
    lua_pop(L, 1);
    return 0;
}

int32_t leep::LUA_DetachFromParent(lua_State *L)
{
    int32_t args = lua_gettop(L);
    if (args != 1)
    {
        return luaL_error(L, "Invalid call, expecting one argument");
    }

    std::string entity_name = lua_tostring(L, 1);
    DetachFromParent(entity_name);
    lua_pop(L, 1);
    return 0;
}

int32_t leep::LUA_SetLocation(lua_State *L)
{
    int32_t args = lua_gettop(L);
    if (args != 4)
    {
        return luaL_error(L, "Invalid call, expecting four arguments");
    }

    std::string entity_name = lua_tostring(L, 1);
    float new_x = (float)lua_tonumber(L, 2);
    float new_y = (float)lua_tonumber(L, 3);
    float new_z = (float)lua_tonumber(L, 4);
    SetLocation(entity_name, new_x, new_y, new_z);
    lua_pop(L, 1);
    return 0;
}
