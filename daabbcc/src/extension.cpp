// Extension lib defines
#define LIB_NAME "DAABBCC"
#define MODULE_NAME "daabbcc"

// include the Defold SDK
#include <daabbcc/daabbcc.h>
#include <dmsdk/sdk.h>

////////////////////////////////////////
// Group Operations
////////////////////////////////////////

static int AddGroup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    uint8_t treeBuildType = daabbcc::UPDATE_INCREMENTAL;

    if (lua_isnumber(L, 1))
    {
        treeBuildType = luaL_checkint(L, 1);
    }

    uint8_t groupID = daabbcc::AddGroup(treeBuildType);
    lua_pushinteger(L, groupID);
    return 1;
}

static int RemoveGroup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("RemoveGroup", groupID);
        return 0;
    }

    daabbcc::RemoveGroup(groupID);

    return 0;
}

////////////////////////////////////////
// Query/Ray Results
////////////////////////////////////////

static inline void Result(lua_State* L, uint32_t queryResultSize, dmArray<uint16_t>& queryResult)
{
    lua_createtable(L, queryResultSize, 0);

    int newTable = lua_gettop(L);

    for (int i = 0; i < queryResultSize; i++)
    {
        lua_pushnumber(L, queryResult[i]);
        lua_rawseti(L, newTable, i + 1);
    }

    lua_pushinteger(L, queryResultSize);
}

static inline void SortResult(lua_State* L, uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult>& queryResult)
{
    lua_createtable(L, queryResultSize, 0);

    int newTable = lua_gettop(L);

    for (int i = 0; i < queryResultSize; i++)
    {
        lua_createtable(L, 2, 0);
        lua_pushstring(L, "id");
        lua_pushinteger(L, queryResult[i].m_proxyID);
        lua_settable(L, -3);
        lua_pushstring(L, "distance");
        lua_pushnumber(L, queryResult[i].m_distance);
        lua_settable(L, -3);

        lua_rawseti(L, newTable, i + 1);
    }

    lua_pushinteger(L, queryResultSize);
}

static inline void ManifoldResult(lua_State* L, uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult>& queryResult)
{
    lua_createtable(L, queryResultSize, 0);

    int newTable = lua_gettop(L);

    for (int i = 0; i < queryResultSize; i++)
    {
        lua_createtable(L, 2, 0);
        lua_pushstring(L, "id");
        lua_pushinteger(L, queryResult[i].m_proxyID);
        lua_settable(L, -3);
        lua_pushstring(L, "distance");
        lua_pushnumber(L, queryResult[i].m_distance);
        lua_settable(L, -3);
        lua_pushstring(L, "depth");
        lua_pushnumber(L, queryResult[i].m_manifold.depth);
        lua_settable(L, -3);
        lua_pushstring(L, "contact_point_x");
        lua_pushnumber(L, queryResult[i].m_manifold.contact_point.x);
        lua_settable(L, -3);
        lua_pushstring(L, "contact_point_y");
        lua_pushnumber(L, queryResult[i].m_manifold.contact_point.y);
        lua_settable(L, -3);
        lua_pushstring(L, "normal_x");
        lua_pushnumber(L, queryResult[i].m_manifold.n.x);
        lua_settable(L, -3);
        lua_pushstring(L, "normal_y");
        lua_pushnumber(L, queryResult[i].m_manifold.n.y);
        lua_settable(L, -3);

        lua_rawseti(L, newTable, i + 1);
    }

    lua_pushinteger(L, queryResultSize);
}

////////////////////////////////////////
// Query Operations
////////////////////////////////////////

static inline int QueryIDSort(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("QueryIDSort", groupID);
        return 0;
    }

    int32_t  proxyID = luaL_checkint(L, 2);
    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 3))
    {
        maskBits = luaL_checkinteger(L, 3);
    }

    if (lua_isboolean(L, 4))
    {
        isManifold = lua_toboolean(L, 4);
    }

    daabbcc::QueryIDSort(proxyID, maskBits, isManifold);

    // Return Result
    uint32_t queryResultSize = 0;

    queryResultSize = daabbcc::GetQueryManifoldResultSize();

    if (queryResultSize > 0)
    {
        dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();
        if (!isManifold)
        {
            SortResult(L, queryResultSize, queryResult);
        }
        else
        {
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

static int QueryAABBSort(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("QueryAABBSort", groupID);
        return 0;
    }

    float    x = luaL_checknumber(L, 2);
    float    y = luaL_checknumber(L, 3);
    uint32_t width = luaL_checkint(L, 4);
    uint32_t height = luaL_checkint(L, 5);
    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 6))
    {
        maskBits = luaL_checkinteger(L, 6);
    }

    if (lua_isboolean(L, 7))
    {
        isManifold = lua_toboolean(L, 7);
    }

    daabbcc::QueryAABBSort(x, y, width, height, maskBits, isManifold);

    // Return Result

    uint32_t queryResultSize = 0;

    queryResultSize = daabbcc::GetQueryManifoldResultSize();

    if (queryResultSize > 0)
    {
        dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();

        if (!isManifold)
        {
            SortResult(L, queryResultSize, queryResult);
        }
        else
        {
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

static int QueryAABB(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("QueryAABB", groupID);
        return 0;
    }

    float    x = luaL_checknumber(L, 2);
    float    y = luaL_checknumber(L, 3);
    uint32_t width = luaL_checkint(L, 4);
    uint32_t height = luaL_checkint(L, 5);
    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 6))
    {
        maskBits = luaL_checkinteger(L, 6);
    }

    if (lua_isboolean(L, 7))
    {
        isManifold = lua_toboolean(L, 7);
    }

    daabbcc::QueryAABB(x, y, width, height, maskBits, isManifold);

    // Return Result

    uint32_t queryResultSize = 0;

    if (!isManifold)
    {
        queryResultSize = daabbcc::GetQueryResultSize();
    }
    else
    {
        queryResultSize = daabbcc::GetQueryManifoldResultSize();
    }

    if (queryResultSize > 0)
    {
        if (!isManifold)
        {
            dmArray<uint16_t>& queryResult = daabbcc::GetQueryResults();
            Result(L, queryResultSize, queryResult);
        }
        else
        {
            dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

static int QueryID(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("QueryID", groupID);
        return 0;
    }

    int32_t  proxyID = luaL_checkint(L, 2);
    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 3))
    {
        maskBits = luaL_checkinteger(L, 3);
    }

    if (lua_isboolean(L, 4))
    {
        isManifold = lua_toboolean(L, 4);
    }

    daabbcc::QueryID(proxyID, maskBits, isManifold);

    // Return Result
    uint32_t queryResultSize = 0;

    if (!isManifold)
    {
        queryResultSize = daabbcc::GetQueryResultSize();
    }
    else
    {
        queryResultSize = daabbcc::GetQueryManifoldResultSize();
    }

    if (queryResultSize > 0)
    {
        if (!isManifold)
        {
            dmArray<uint16_t>& queryResult = daabbcc::GetQueryResults();
            Result(L, queryResultSize, queryResult);
        }
        else
        {
            dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

////////////////////////////////////////
// Raycast Operations
////////////////////////////////////////

static int RayCast(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("RayCast", groupID);
        return 0;
    }

    float    start_x = luaL_checknumber(L, 2);
    float    start_y = luaL_checknumber(L, 3);
    float    end_x = luaL_checknumber(L, 4);
    float    end_y = luaL_checknumber(L, 5);

    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 6))
    {
        maskBits = luaL_checkinteger(L, 6);
    }

    if (lua_isboolean(L, 7))
    {
        isManifold = lua_toboolean(L, 7);
    }

    daabbcc::RayCast(start_x, start_y, end_x, end_y, maskBits, isManifold);

    uint32_t queryResultSize = 0;

    if (!isManifold)
    {
        queryResultSize = daabbcc::GetQueryResultSize();
    }
    else
    {
        queryResultSize = daabbcc::GetQueryManifoldResultSize();
    }

    if (queryResultSize > 0)
    {
        if (!isManifold)
        {
            dmArray<uint16_t>& queryResult = daabbcc::GetQueryResults();
            Result(L, queryResultSize, queryResult);
        }
        else
        {
            dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

static int RayCastSort(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("RayCast", groupID);
        return 0;
    }

    float    start_x = luaL_checknumber(L, 2);
    float    start_y = luaL_checknumber(L, 3);
    float    end_x = luaL_checknumber(L, 4);
    float    end_y = luaL_checknumber(L, 5);

    uint64_t maskBits = B2_DEFAULT_MASK_BITS;
    bool     isManifold = false;

    if (lua_isnumber(L, 6))
    {
        maskBits = luaL_checkinteger(L, 6);
    }

    if (lua_isboolean(L, 7))
    {
        isManifold = lua_toboolean(L, 7);
    }

    daabbcc::RayCastSort(start_x, start_y, end_x, end_y, maskBits, isManifold);

    uint32_t queryResultSize = 0;

    queryResultSize = daabbcc::GetQueryManifoldResultSize();

    if (queryResultSize > 0)
    {
        dmArray<daabbcc::ManifoldResult>& queryResult = daabbcc::GetQueryManifoldResults();
        if (!isManifold)
        {
            SortResult(L, queryResultSize, queryResult);
        }
        else
        {
            ManifoldResult(L, queryResultSize, queryResult);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushinteger(L, queryResultSize);
    }

    return 2;
}

////////////////////////////////////////
// Proxy Operations
////////////////////////////////////////

static int AddProxy(lua_State* L)
{
    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("AddProxy", groupID);
        return 0;
    }

    float    x = luaL_checknumber(L, 2);
    float    y = luaL_checknumber(L, 3);
    uint32_t width = luaL_checkint(L, 4);
    uint32_t height = luaL_checkint(L, 5);
    uint64_t categoryBits = B2_DEFAULT_CATEGORY_BITS;

    if (lua_isnumber(L, 6))
    {
        categoryBits = luaL_checkinteger(L, 6);
    }

    int32_t proxyID = daabbcc::AddProxy(groupID, x, y, width, height, categoryBits);

    lua_pushinteger(L, proxyID);

    return 1;
}

static int AddGameObject(lua_State* L)
{
    // DM_LUA_STACK_CHECK(L, 1);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        dmLogError("AddGameObject: Group ID %i is invalid or already removed!", groupID);
        return 0;
    }

    dmGameObject::HInstance gameobjectInstance = dmScript::CheckGOInstance(L, 2);

    dmVMath::Point3         gameobjectPosition = dmGameObject::GetPosition(gameobjectInstance);
    float                   x = gameobjectPosition.getX();
    float                   y = gameobjectPosition.getY();

    uint32_t                width = luaL_checkint(L, 3);
    uint32_t                height = luaL_checkint(L, 4);

    uint64_t                categoryBits = B2_DEFAULT_CATEGORY_BITS;
    bool                    getWorldPosition = false;

    if (lua_isnumber(L, 5))
    {
        categoryBits = luaL_checkinteger(L, 5);
    }

    if (lua_isboolean(L, 6))
    {
        getWorldPosition = lua_toboolean(L, 6);
    }

    int32_t proxyID = daabbcc::AddProxy(groupID, x, y, width, height, categoryBits);

    daabbcc::AddGameObject(groupID, proxyID, gameobjectPosition, width, height, gameobjectInstance, getWorldPosition);

    lua_pushinteger(L, proxyID);

    return 1;
}

static int MoveProxy(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("MoveProxy", groupID);
        return 0;
    }

    int32_t  proxyID = luaL_checkint(L, 2);
    float    x = luaL_checknumber(L, 3);
    float    y = luaL_checknumber(L, 4);
    uint32_t width = luaL_checkint(L, 5);
    uint32_t height = luaL_checkint(L, 6);

    daabbcc::MoveProxy(proxyID, x, y, width, height);

    return 0;
}

static int UpdateGameobjectSize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("UpdateGameobjectSize", groupID);
        return 0;
    }

    int32_t  proxyID = luaL_checkint(L, 2);
    uint32_t width = luaL_checkint(L, 3);
    uint32_t height = luaL_checkint(L, 4);

    daabbcc::UpdateGameobjectSize(groupID, proxyID, width, height);

    return 0;
}

static int RemoveProxy(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("RemoveProxy", groupID);
        return 0;
    }

    int32_t proxyID = luaL_checkint(L, 2);

    daabbcc::RemoveProxy(groupID, proxyID);

    return 0;
}

////////////////////////////////////////
// Gameobject Update Operations
////////////////////////////////////////

static int Run(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    if (lua_isboolean(L, 1))
    {
        bool state = lua_toboolean(L, 1);
        daabbcc::Run(state);
    }
    return 0;
}

static int UpdateFrequency(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    int32_t updateFrequency = luaL_checkint(L, 1);
    daabbcc::SetUpdateFrequency(updateFrequency);
    return 0;
}

static int Reset(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    daabbcc::Reset();
    return 0;
}

static int Rebuild(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    uint8_t groupID = luaL_checkint(L, 1);

    bool    isSet = daabbcc::SetTreeGroup(groupID);
    if (!isSet)
    {
        daabbcc::ErrorAssert("Rebuild", groupID);
        return 0;
    }

    bool fullBuild = lua_toboolean(L, 2);

    daabbcc::Rebuild(groupID, fullBuild);

    return 0;
}

static int RebuildAll(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    bool fullBuild = lua_toboolean(L, 2);
    daabbcc::RebuildAll(fullBuild);
    return 0;
}

// clang-format off
// Functions exposed to Lua
static const luaL_reg Module_methods[] = {
 // {"init", Init}, 

  // Group Operations
  {"new_group", AddGroup}, 
  {"remove_group", RemoveGroup}, 

  // Proxy Operations
  {"insert_aabb", AddProxy},
  {"insert_gameobject", AddGameObject},
  {"update_aabb", MoveProxy},
  {"update_gameobject_size", UpdateGameobjectSize},
  {"remove", RemoveProxy},

  // Query Operations
  {"query_aabb", QueryAABB},
  {"query_id", QueryID},
  {"query_aabb_sort", QueryAABBSort},
  {"query_id_sort", QueryIDSort},

    // Raycast Operations
  {"raycast", RayCast},
  {"raycast_sort", RayCastSort},

  // Gameobject Update Operations
  {"run", Run},
  {"update_frequency", UpdateFrequency},

  // Tree Operations
{"rebuild", Rebuild},
{"rebuild_all", RebuildAll},

  // Helpers
{"reset", Reset},

  {0, 0}
};
// clang-format on

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

#define SETCONSTANT(name) \
    lua_pushnumber(L, (lua_Number)daabbcc::name); \
    lua_setfield(L, -2, #name);

    SETCONSTANT(UPDATE_INCREMENTAL);
    SETCONSTANT(UPDATE_FULLREBUILD);
    SETCONSTANT(UPDATE_PARTIALREBUILD);
#undef SETCONSTANT

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result AppInitializeDAABBCC(dmExtension::AppParams* params)
{
    dmLogInfo("AppInitializeDAABBCC");

    uint8_t  max_group_count = dmConfigFile::GetInt(params->m_ConfigFile, "daabbcc.max_group_count", 3);

    uint16_t max_gameobject_count = dmConfigFile::GetInt(params->m_ConfigFile, "daabbcc.max_gameobject_count", 128);

    uint16_t max_query_count = dmConfigFile::GetInt(params->m_ConfigFile, "daabbcc.max_query_result_count", 32);

    int32_t  updateFrequency = dmConfigFile::GetInt(params->m_ConfigFile, "display.update_frequency", 0);

    daabbcc::Setup(max_group_count, max_gameobject_count, max_query_count);
    daabbcc::SetUpdateFrequency(updateFrequency);

    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeDAABBCC(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension", MODULE_NAME);

    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeDAABBCC(dmExtension::Params* params)
{
    daabbcc::Reset();
    return dmExtension::RESULT_OK;
}

static dmExtension::Result OnUpdateDAABBCC(dmExtension::Params* params)
{
    daabbcc::GameObjectUpdate();
    return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update,
// on_event, final)

// DAABBCC is the C++ symbol that holds all relevant extension data.
// It must match the name field in the `ext.manifest`
DM_DECLARE_EXTENSION(DAABBCC, LIB_NAME, AppInitializeDAABBCC, 0, InitializeDAABBCC, OnUpdateDAABBCC, 0, FinalizeDAABBCC)
