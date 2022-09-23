
#define LIB_NAME "DAABBCC"
#define MODULE_NAME "aabb"
#define DLIB_LOG_DOMAIN "DAABBCC"

#include <DynamicTree.hpp>
#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>

DynamicTree dynamicTree;

static int AddGroup(lua_State *L)
{
    int groupID = dynamicTree.AddGroup();
    lua_pushinteger(L, groupID);
    return 1;
}

static int RemoveGroup(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    dynamicTree.RemoveGroup(groupID);
    return 0;
}

static int AddProxy(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    int w = luaL_checkint(L, 4);
    int h = luaL_checkint(L, 5);

    int proxyID = dynamicTree.AddProxy(groupID, x, y, w, h);

    lua_pushinteger(L, proxyID);
    return 1;
}

static int RemoveProxy(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    int proxyID = luaL_checkint(L, 2);

    dynamicTree.RemoveProxy(groupID, proxyID);

    return 0;
}

static int QueryIDShort(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    int proxyID = luaL_checkint(L, 2);

    dynamicTree.QueryIDShort(groupID, proxyID);

    lua_createtable(L, dynamicTree.orderResult.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.orderResult.Size(); i++)
    {
        lua_createtable(L, 2, 0);
        lua_pushstring(L, "id");
        lua_pushinteger(L, dynamicTree.orderResult[i].proxyID);
        lua_settable(L, -3);
        lua_pushstring(L, "distance");
        lua_pushinteger(L, dynamicTree.orderResult[i].distance);
        lua_settable(L, -3);

        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int QueryAABBShort(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    int w = luaL_checkint(L, 4);
    int h = luaL_checkint(L, 5);

    dynamicTree.QueryAABBShort(groupID, x, y, w, h);

    lua_createtable(L, dynamicTree.orderResult.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.orderResult.Size(); i++)
    {
        lua_createtable(L, 2, 0);
        lua_pushstring(L, "id");
        lua_pushinteger(L, dynamicTree.orderResult[i].proxyID);
        lua_settable(L, -3);
        lua_pushstring(L, "distance");
        lua_pushinteger(L, dynamicTree.orderResult[i].distance);
        lua_settable(L, -3);

        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int RayCastShort(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    float start_x = luaL_checknumber(L, 2);
    float start_y = luaL_checknumber(L, 3);
    float end_x = luaL_checknumber(L, 4);
    float end_y = luaL_checknumber(L, 5);

    dynamicTree.RayCastShort(groupID, start_x, start_y, end_x, end_y);

    lua_createtable(L, dynamicTree.orderResult.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.orderResult.Size(); i++)
    {
        lua_createtable(L, 2, 0);
        lua_pushstring(L, "id");
        lua_pushinteger(L, dynamicTree.orderResult[i].proxyID);
        lua_settable(L, -3);
        lua_pushstring(L, "distance");
        lua_pushinteger(L, dynamicTree.orderResult[i].distance);
        lua_settable(L, -3);

        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int QueryID(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    int proxyID = luaL_checkint(L, 2);

    dynamicTree.QueryID(groupID, proxyID);

    lua_createtable(L, dynamicTree.result.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.result.Size(); i++)
    {
        lua_pushnumber(L, dynamicTree.result[i]);
        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int QueryAABB(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    int w = luaL_checkint(L, 4);
    int h = luaL_checkint(L, 5);

    dynamicTree.QueryAABB(groupID, x, y, w, h);

    lua_createtable(L, dynamicTree.result.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.result.Size(); i++)
    {
        lua_pushnumber(L, dynamicTree.result[i]);
        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int RayCast(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    float start_x = luaL_checknumber(L, 2);
    float start_y = luaL_checknumber(L, 3);
    float end_x = luaL_checknumber(L, 4);
    float end_y = luaL_checknumber(L, 5);

    dynamicTree.RayCast(groupID, start_x, start_y, end_x, end_y);

    lua_createtable(L, dynamicTree.ray_result.Size(), 0);
    int newTable = lua_gettop(L);
    for (int i = 0; i < dynamicTree.ray_result.Size(); i++)
    {
        lua_pushnumber(L, dynamicTree.ray_result[i]);
        lua_rawseti(L, newTable, i + 1);
    }

    return 1;
}

static int MoveProxy(lua_State *L)
{
    int groupID = luaL_checkint(L, 1);

    if (dynamicTree.CheckGroup(groupID))
    {
        dmLogError("Group ID is invalid");
        return 0;
    }

    int proxyID = luaL_checkint(L, 2);
    float x = luaL_checknumber(L, 3);
    float y = luaL_checknumber(L, 4);
    int w = luaL_checkint(L, 5);
    int h = luaL_checkint(L, 6);

    dynamicTree.MoveProxy(groupID, proxyID, x, y, w, h);

    return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
    {
        {"raycast", RayCast},
        {"raycast_short", RayCastShort},
        {"query_id", QueryID},
        {"query_id_short", QueryIDShort},
        {"query", QueryAABB},
        {"query_short", QueryAABBShort},
        {"new_group", AddGroup},
        {"remove_group", RemoveGroup},
        {"insert", AddProxy},
        {"remove", RemoveProxy},
        {"update", MoveProxy},
        {NULL, NULL}};

static void LuaInit(lua_State *L)
{
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeDAABBCC(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDAABBCC(dmExtension::Params *params)
{
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDAABBCC(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDAABBCC(dmExtension::Params *params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(DAABBCC, LIB_NAME, AppInitializeDAABBCC, AppFinalizeDAABBCC, InitializeDAABBCC, 0, 0, FinalizeDAABBCC)
