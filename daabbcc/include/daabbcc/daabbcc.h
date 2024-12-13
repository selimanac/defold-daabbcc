#pragma once

#include "daabbcc/math_functions.h"
#include <cstdint>
#include <daabbcc/constants.h>
#include <daabbcc/collision.h>
#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/hashtable.h>
#include <dmsdk/dlib/log.h>
#include <dmsdk/dlib/time.h>
#include <dmsdk/gameobject/gameobject.h>

namespace daabbcc
{
    // Manifold
    typedef struct b2Manifold
    {
        uint16_t count;
        float    depth;
        b2Vec2   contact_point;
        b2Vec2   n;
    } b2Manifold;

    struct ManifoldResult
    {
        int32_t    m_proxyID;
        float      m_distance;
        b2Manifold m_manifold;
    };

    // For Query callbacks
    struct QueryContainer
    {
        int32_t m_proxyID;
        b2Vec2  m_center;
        bool    m_isAABB;
        bool    m_isManifold;
    };

    // Update dt calc
    struct GameUpdate
    {
        uint8_t  m_updateFrequency = 0;
        uint64_t m_previousFrameTime;
        float    m_accumFrameTime;
        bool     m_updateLoopState = true;
        uint32_t m_loopCounter;
    };

    typedef struct GameObject
    {
        uint8_t                 m_groupID;
        int32_t                 m_proxyID;
        dmVMath::Point3         m_position;
        dmGameObject::HInstance m_gameObjectInstance;
        uint32_t                m_width;
        uint32_t                m_height;
        bool                    m_getWorldPosition = false;
    } GameObject;

    typedef struct Ray
    {
        b2Vec2 m_startPoint;
        b2Vec2 m_endPoint;
    } Ray;

    enum TreeBuildType
    {
        UPDATE_INCREMENTAL,
        UPDATE_FULLREBUILD,
        UPDATE_PARTIALREBUILD
    };

    struct DAABBCC
    {
        struct TreeGroup
        {
            b2DynamicTree m_dynamicTree;
            TreeBuildType m_buildType = UPDATE_FULLREBUILD;
        };

        // b2DynamicTree Group container
        dmHashTable<uint8_t, TreeGroup> m_dynamicTreeGroup;

        TreeGroup*                      m_treeGroup;

        // Query result
        dmArray<uint16_t> m_queryResult;

        // Querymanifold result
        dmArray<ManifoldResult> m_queryManifoldResult;
        ManifoldResult          m_manifoldResult;

        // Increment Group ID
        uint8_t m_groupID = 0;
        uint8_t m_currentGroupID = 0;

        // Query temp AABB, AABB Center, SortContainer
        b2AABB              m_aabb;
        b2AABB              m_manifoldAABB;
        b2Manifold          m_manifold;
        QueryContainer      m_queryContainer;
        b2RayCastInput      m_raycastInput;
        b2CastOutput        m_raycastOutput;
        dmArray<GameObject> m_gameObjectContainer;
        GameObject*         m_gameObject;
        Ray                 m_ray;

        // Max allocations
        const uint8_t  m_max_group_count = 3;
        const uint16_t m_max_gameobject_count = 128;
        const uint16_t m_max_query_count = 32;
        const uint16_t m_max_raycast_count = 32;
    };

    ////////////////////////////////////////
    // Initialize dynamic tree
    ////////////////////////////////////////

    void Setup(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count);

    ////////////////////////////////////////
    // Group Operations
    ////////////////////////////////////////

    uint8_t AddGroup(uint8_t treeBuildType);

    void    RemoveGroup(uint8_t groupID);

    bool    SetTreeGroup(uint8_t groupID); // THIS IS FOR INTERNAL USE ONLY

    ////////////////////////////////////////
    // Proxy Operations
    ////////////////////////////////////////

    int32_t AddProxy(uint8_t groupID, float x, float y, uint32_t width, uint32_t height, uint64_t categoryBits);

    void    AddGameObject(uint8_t groupID, int32_t proxyID, dmVMath::Point3 position, uint32_t width, uint32_t height, dmGameObject::HInstance gameObjectInstance, bool getWorldPosition);

    void    MoveProxy(int32_t proxyID, float x, float y, uint32_t width, uint32_t height);

    void    UpdateGameobjectSize(uint8_t groupID, int32_t proxyID, uint32_t width, uint32_t height);

    void    RemoveProxy(uint8_t groupID, int32_t proxyID);

    ////////////////////////////////////////
    // Query Operations
    ////////////////////////////////////////

    void QueryAABB(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits, bool isManifold);

    void QueryID(int32_t proxyID, uint64_t maskBits, bool isManifold);

    void QueryAABBSort(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits, bool isManifold);

    void QueryIDSort(int32_t proxyID, uint64_t maskBits, bool isManifold);

    // Query Results
    uint32_t                 GetQueryResultSize();

    uint32_t                 GetQueryManifoldResultSize();

    dmArray<uint16_t>&       GetQueryResults();

    dmArray<ManifoldResult>& GetQueryManifoldResults();

    ////////////////////////////////////////
    // Raycast Operations
    ////////////////////////////////////////

    void RayCast(float start_x, float start_y, float end_x, float end_y, uint64_t maskBits, bool isManifold);

    void RayCastSort(float start_x, float start_y, float end_x, float end_y, uint64_t maskBits, bool isManifold);

    ////////////////////////////////////////
    // Gameobject Update Operations
    ////////////////////////////////////////

    void Run(bool toggle);

    void SetUpdateFrequency(int32_t updateFrequency);

    void GameObjectUpdate();

    ////////////////////////////////////////
    // Tree Operations
    ////////////////////////////////////////

    void Rebuild(uint8_t groupID, bool fullBuild);

    void RebuildAll(bool fullBuild);

    ////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////
    static inline int CompareDistance(const ManifoldResult* a, const ManifoldResult* b)
    {
        return a->m_distance - b->m_distance;
    }

    static void        AABBtoAABBManifold(b2AABB A, b2AABB B, b2Manifold* m);

    static inline void Bound(b2AABB* aabb, float x, float y, uint32_t width, uint32_t height);

    static void        CalcTimeStep(float& step_dt, uint32_t& num_steps);

    void               Reset();

    void               ErrorAssert(const char* info, uint8_t groupID);

    void               LimitErrorAssert(const char* info, uint16_t count);

    ////////////////////////////////////////
    // Tests
    ////////////////////////////////////////
    void DumpQueryResult(char* title);

    void DumpManifoldResult(char* title);

} // namespace daabbcc
