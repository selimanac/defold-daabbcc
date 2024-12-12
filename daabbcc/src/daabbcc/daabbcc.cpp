#include "daabbcc/math_functions.h"
#include "dmsdk/dlib/log.h"
#include "dmsdk/gameobject/gameobject.h"
#include <cstdint>
#include <daabbcc/aabb.h>
#include <daabbcc/daabbcc.h>

namespace daabbcc
{

    DAABBCC    m_daabbcc;
    GameUpdate m_gameUpdate;

    ////////////////////////////////////////
    // Initialize dynamic tree
    ////////////////////////////////////////
    void Setup(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count)
    {
        m_daabbcc.m_dynamicTreeGroup.SetCapacity(max_group_count, max_group_count);

        // Query arrays
        m_daabbcc.m_queryResult.SetCapacity(max_query_count);

        // QueryManifold
        m_daabbcc.m_queryManifoldResult.SetCapacity(max_query_count);

        m_daabbcc.m_gameObjectContainer.SetCapacity(max_gameobject_count);
    }

    ////////////////////////////////////////
    // Group Operations
    ////////////////////////////////////////

    /*
     * Add new group
     * @param m_treeBuildType Tree rebuild type
     * @return groupID GroupID
     */
    uint8_t AddGroup(uint8_t treeBuildType)
    {
        if (m_daabbcc.m_dynamicTreeGroup.Full())
        {
            LimitErrorAssert("Max Group Count", m_daabbcc.m_dynamicTreeGroup.Size());
            return 0;
        }

        DAABBCC::TreeGroup treeGroup;
        m_daabbcc.m_groupID++;

        treeGroup.m_dynamicTree = b2DynamicTree_Create();
        treeGroup.m_buildType = (TreeBuildType)treeBuildType;

        m_daabbcc.m_dynamicTreeGroup.Put(m_daabbcc.m_groupID, treeGroup);

        return m_daabbcc.m_groupID;
    }

    void RemoveGroup(uint8_t groupID)
    {
        // Remove all Gameobjects
        uint32_t n = m_daabbcc.m_gameObjectContainer.Size();
        for (int i = 0; i < n; ++i)
        {
            if (m_daabbcc.m_gameObjectContainer[i].m_groupID == groupID)
            {
                m_daabbcc.m_gameObjectContainer.EraseSwap(i);
                --n;
                --i;
            }
        }

        // Destroy tree
        b2DynamicTree_Destroy(&m_daabbcc.m_treeGroup->m_dynamicTree);

        // Erase from groups
        m_daabbcc.m_dynamicTreeGroup.Erase(groupID);

        m_daabbcc.m_currentGroupID = 0;
    }

    bool SetTreeGroup(uint8_t groupID)
    {
        if (m_daabbcc.m_currentGroupID != groupID)
        {
            m_daabbcc.m_currentGroupID = groupID;
            m_daabbcc.m_treeGroup = m_daabbcc.m_dynamicTreeGroup.Get(groupID);
        }

        if (m_daabbcc.m_treeGroup == NULL)
        {
            return false;
        }
        return true;
    }

    ////////////////////////////////////////
    // Proxy Operations
    ////////////////////////////////////////

    int32_t AddProxy(uint8_t groupID, float x, float y, uint32_t width, uint32_t height, uint64_t categoryBits)
    {
        Bound(&m_daabbcc.m_aabb, x, y, width, height);
        int32_t proxyID = b2DynamicTree_CreateProxy(&m_daabbcc.m_treeGroup->m_dynamicTree, m_daabbcc.m_aabb, categoryBits, groupID);

        return proxyID;
    }

    void AddGameObject(uint8_t groupID, int32_t proxyID, dmVMath::Point3 position, uint32_t width, uint32_t height, dmGameObject::HInstance gameObjectInstance, bool getWorldPosition)
    {
        GameObject gameObject;

        gameObject.m_groupID = groupID;
        gameObject.m_proxyID = proxyID;
        gameObject.m_position = position;
        gameObject.m_gameObjectInstance = gameObjectInstance;
        gameObject.m_width = width;
        gameObject.m_height = height;
        gameObject.m_getWorldPosition = getWorldPosition;

        if (m_daabbcc.m_gameObjectContainer.Full())
        {
            LimitErrorAssert("Max Gameobject Count", m_daabbcc.m_gameObjectContainer.Size());
        }
        else
        {
            m_daabbcc.m_gameObjectContainer.Push(gameObject);
        }
    }

    void MoveProxy(int32_t proxyID, float x, float y, uint32_t width, uint32_t height)
    {
        Bound(&m_daabbcc.m_aabb, x, y, width, height);
        b2DynamicTree_MoveProxy(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID, m_daabbcc.m_aabb);
    }

    void UpdateGameobjectSize(uint8_t groupID, int32_t proxyID, uint32_t width, uint32_t height)
    {
        for (uint32_t i = 0; i < m_daabbcc.m_gameObjectContainer.Size(); ++i)
        {
            if (m_daabbcc.m_gameObjectContainer[i].m_groupID == groupID && m_daabbcc.m_gameObjectContainer[i].m_proxyID == proxyID)
            {
                m_daabbcc.m_gameObjectContainer[i].m_width = width;
                m_daabbcc.m_gameObjectContainer[i].m_width = height;
            }
        }
    }

    void RemoveProxy(uint8_t groupID, int32_t proxyID)
    {
        for (uint32_t i = 0; i < m_daabbcc.m_gameObjectContainer.Size(); ++i)
        {
            if (m_daabbcc.m_gameObjectContainer[i].m_groupID == groupID && m_daabbcc.m_gameObjectContainer[i].m_proxyID == proxyID)
            {
                m_daabbcc.m_gameObjectContainer.EraseSwap(i);
            }
        }

        b2DynamicTree_DestroyProxy(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);
    }

    ////////////////////////////////////////
    // Query Callbacks
    ////////////////////////////////////////

    static bool QueryCallback(int32_t proxyID, int32_t groupID, void* queryContainer)
    {
        QueryContainer* m_queryContainer = (QueryContainer*)queryContainer;

        if (!m_queryContainer->m_isAABB && proxyID == m_queryContainer->m_proxyID)
        {
            return true;
        }

        if (m_daabbcc.m_queryResult.Full() || m_daabbcc.m_queryManifoldResult.Full())
        {
            LimitErrorAssert("Max Query Result Count", m_daabbcc.m_queryResult.Size());
        }
        else
        {
            if (!m_queryContainer->m_isManifold)
            {
                m_daabbcc.m_queryResult.Push(proxyID);
            }
            else
            {
                m_daabbcc.m_manifoldAABB = b2DynamicTree_GetAABB(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);

                AABBtoAABBManifold(m_daabbcc.m_aabb, m_daabbcc.m_manifoldAABB, &m_daabbcc.m_manifold);

                m_daabbcc.m_manifoldResult = {
                    proxyID,
                    b2Distance(m_queryContainer->m_center, b2AABB_Center(m_daabbcc.m_manifoldAABB)),
                    m_daabbcc.m_manifold
                };

                m_daabbcc.m_queryManifoldResult.Push(m_daabbcc.m_manifoldResult);
            }
        }

        return true;
    }

    static bool QuerySortCallback(int32_t proxyID, int32_t groupID, void* queryContainer)
    {
        QueryContainer* m_queryContainer = (QueryContainer*)queryContainer;

        if (!m_queryContainer->m_isAABB && proxyID == m_queryContainer->m_proxyID)
        {
            return true;
        }

        if (m_daabbcc.m_queryManifoldResult.Full())
        {
            LimitErrorAssert("Max Query Result Count", m_daabbcc.m_queryManifoldResult.Size());
        }
        else
        {
            m_daabbcc.m_manifoldAABB = b2DynamicTree_GetAABB(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);

            if (!m_queryContainer->m_isManifold)
            {
                m_daabbcc.m_manifoldResult = {
                    proxyID,
                    b2Distance(m_queryContainer->m_center, b2AABB_Center(m_daabbcc.m_manifoldAABB))
                };
            }
            else
            {
                AABBtoAABBManifold(m_daabbcc.m_aabb, m_daabbcc.m_manifoldAABB, &m_daabbcc.m_manifold);

                m_daabbcc.m_manifoldResult = {
                    proxyID,
                    b2Distance(m_queryContainer->m_center, b2AABB_Center(m_daabbcc.m_manifoldAABB)),
                    m_daabbcc.m_manifold
                };
            }

            m_daabbcc.m_queryManifoldResult.Push(m_daabbcc.m_manifoldResult);
        }
        return true;
    }

    ////////////////////////////////////////
    // Query Operations
    ////////////////////////////////////////

    /**************************/
    // HELPERS
    /**************************/

    static void Query(b2AABB* aabb, b2TreeQueryCallbackFcn* callback, void* context, uint64_t maskBits)
    {
        // Clear the results
        m_daabbcc.m_queryResult.SetSize(0);
        m_daabbcc.m_queryManifoldResult.SetSize(0);

        b2DynamicTree_Query(&m_daabbcc.m_treeGroup->m_dynamicTree, *aabb, maskBits, callback, context);
    }

    static void QuerySort(int32_t proxyID, uint64_t maskBits)
    {
        Query(&m_daabbcc.m_aabb, QuerySortCallback, &m_daabbcc.m_queryContainer, maskBits);

        qsort(m_daabbcc.m_queryManifoldResult.Begin(), m_daabbcc.m_queryManifoldResult.Size(), sizeof(ManifoldResult), (int (*)(const void*, const void*))CompareDistance);
    }

    /**************************/
    // QUERIES
    /**************************/

    void QueryAABB(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits, bool isManifold)
    {
        Bound(&m_daabbcc.m_aabb, x, y, width, height);

        m_daabbcc.m_queryContainer = { 0, b2AABB_Center(m_daabbcc.m_aabb), true, isManifold };

        Query(&m_daabbcc.m_aabb, QueryCallback, &m_daabbcc.m_queryContainer, maskBits);
    }

    void QueryID(int32_t proxyID, uint64_t maskBits, bool isManifold)
    {
        m_daabbcc.m_aabb = b2DynamicTree_GetAABB(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);

        m_daabbcc.m_queryContainer = { proxyID, b2AABB_Center(m_daabbcc.m_aabb), false, isManifold };

        Query(&m_daabbcc.m_aabb, QueryCallback, &m_daabbcc.m_queryContainer, maskBits);
    }

    void QueryAABBSort(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits, bool isManifold)
    {
        Bound(&m_daabbcc.m_aabb, x, y, width, height);

        m_daabbcc.m_queryContainer = { 0, b2AABB_Center(m_daabbcc.m_aabb), true, isManifold };

        QuerySort(0, maskBits);
    }

    void QueryIDSort(int32_t proxyID, uint64_t maskBits, bool isManifold)
    {
        m_daabbcc.m_aabb = b2DynamicTree_GetAABB(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);

        m_daabbcc.m_queryContainer = { proxyID, b2AABB_Center(m_daabbcc.m_aabb), false, isManifold };

        QuerySort(proxyID, maskBits);
    }

    uint32_t GetQueryResultSize()
    {
        return m_daabbcc.m_queryResult.Size();
    }

    uint32_t GetQueryManifoldResultSize()
    {
        return m_daabbcc.m_queryManifoldResult.Size();
    }

    dmArray<uint16_t>& GetQueryResults()
    {
        return m_daabbcc.m_queryResult;
    }

    dmArray<ManifoldResult>& GetQueryManifoldResults()
    {
        return m_daabbcc.m_queryManifoldResult;
    }

    ////////////////////////////////////////
    // Raycast Callbacks
    ////////////////////////////////////////

    static float RayCastCallback(const b2RayCastInput* input, int32_t proxyID, int32_t groupID, void* queryContainer)
    {
        QueryContainer* m_queryContainer = (QueryContainer*)queryContainer;

        if (m_daabbcc.m_queryResult.Full() || m_daabbcc.m_queryManifoldResult.Full())
        {
            LimitErrorAssert("Max Query Result Count", m_daabbcc.m_queryResult.Size());
        }
        else
        {
            if (!m_queryContainer->m_isManifold)
            {
                m_daabbcc.m_queryResult.Push(proxyID);
            }
            else
            {
                m_daabbcc.m_manifoldAABB = b2DynamicTree_GetAABB(&m_daabbcc.m_treeGroup->m_dynamicTree, proxyID);

                m_daabbcc.m_raycastOutput = b2AABB_RayCast(m_daabbcc.m_manifoldAABB, input->origin, m_queryContainer->m_center); // m_center is used for m_endPoint here

                m_daabbcc.m_manifold.n = m_daabbcc.m_raycastOutput.normal;
                m_daabbcc.m_manifold.contact_point = m_daabbcc.m_raycastOutput.point;

                m_daabbcc.m_manifoldResult = {
                    proxyID,
                    b2Distance(input->origin, b2AABB_Center(m_daabbcc.m_manifoldAABB)),
                    m_daabbcc.m_manifold
                };

                m_daabbcc.m_queryManifoldResult.Push(m_daabbcc.m_manifoldResult);
            }
        }

        return input->maxFraction;
    }

    ////////////////////////////////////////
    // Raycast Operations
    ////////////////////////////////////////

    void RayCast(uint8_t groupID, float start_x, float start_y, float end_x, float end_y, uint64_t maskBits, bool isManifold, bool isSort)
    {
        m_daabbcc.m_queryResult.SetSize(0);
        m_daabbcc.m_queryManifoldResult.SetSize(0);

        b2Vec2 m_startPoint = { start_x, start_y };
        b2Vec2 m_endPoint = { end_x, end_y };

        m_daabbcc.m_raycastInput = { m_startPoint, b2Sub(m_endPoint, m_startPoint), 1.0f };

        // m_center is used for m_endPoint here
        m_daabbcc.m_queryContainer = { 0, m_endPoint, false, isManifold };

        b2DynamicTree_RayCast(&m_daabbcc.m_treeGroup->m_dynamicTree, &m_daabbcc.m_raycastInput, maskBits, RayCastCallback, &m_daabbcc.m_queryContainer);

        if (isSort)
        {
            qsort(m_daabbcc.m_queryManifoldResult.Begin(), m_daabbcc.m_queryManifoldResult.Size(), sizeof(ManifoldResult), (int (*)(const void*, const void*))CompareDistance);
        }
    }

    ////////////////////////////////////////
    // Gameobject Update Operations
    ////////////////////////////////////////

    void Run(bool toggle)
    {
        m_gameUpdate.m_updateLoopState = toggle;
    };

    void SetUpdateFrequency(int32_t updateFrequency)
    {
        m_gameUpdate.m_updateFrequency = updateFrequency;
    };

    // From Defold source
    // https://github.com/defold/defold/blob/cdaa870389ca00062bfc03bcda8f4fb34e93124a/engine/engine/src/engine.cpp#L1902
    void GameObjectUpdate()
    {
        // If paused or not set
        if (!m_gameUpdate.m_updateLoopState || m_daabbcc.m_gameObjectContainer.Empty())
        {
            return;
        }

        float    step_dt;   // The dt for each step (the game frame)
        uint32_t num_steps; // Number of times to loop over the StepFrame function

        CalcTimeStep(step_dt, num_steps);

        for (uint32_t i = 0; i < num_steps; ++i)
        {
            for (int i = 0; i < m_daabbcc.m_gameObjectContainer.Size(); ++i)
            {
                m_daabbcc.m_gameObject = &m_daabbcc.m_gameObjectContainer[i];

                if (m_daabbcc.m_gameObject->m_getWorldPosition)
                {
                    m_daabbcc.m_gameObject->m_position = dmGameObject::GetWorldPosition(m_daabbcc.m_gameObject->m_gameObjectInstance);
                }
                else
                {
                    m_daabbcc.m_gameObject->m_position = dmGameObject::GetPosition(m_daabbcc.m_gameObject->m_gameObjectInstance);
                }

                // TODO Find a better way:
                b2AABB              m_aabb;
                DAABBCC::TreeGroup* m_treeGroup = m_daabbcc.m_dynamicTreeGroup.Get(m_daabbcc.m_gameObject->m_groupID);

                Bound(&m_aabb, m_daabbcc.m_gameObject->m_position.getX(), m_daabbcc.m_gameObject->m_position.getY(), m_daabbcc.m_gameObject->m_width, m_daabbcc.m_gameObject->m_height);

                b2DynamicTree_MoveProxy(&m_treeGroup->m_dynamicTree, m_daabbcc.m_gameObject->m_proxyID, m_aabb);
            }

            m_daabbcc.m_dynamicTreeGroup.Iterate(GameobjectRebuildIterateCallback, (void*)0x0);
        }
    };

    ////////////////////////////////////////
    // Tree Operations
    ////////////////////////////////////////

    static inline void RemoveGroupsIterateCallback(void*, const uint8_t* key, DAABBCC::TreeGroup* treeGroup)
    {
        b2DynamicTree_Destroy(&treeGroup->m_dynamicTree);
    }

    static inline void GameobjectRebuildIterateCallback(void*, const uint8_t* key, DAABBCC::TreeGroup* treeGroup)
    {
        if (treeGroup->m_buildType == UPDATE_INCREMENTAL)
        {
            return;
        }

        bool fullBuild = (treeGroup->m_buildType == UPDATE_FULLREBUILD) ? true : false;

        b2DynamicTree_Rebuild(&treeGroup->m_dynamicTree, fullBuild);
    }

    static inline void RebuildIterateCallback(bool* fullBuild, const uint8_t* key, DAABBCC::TreeGroup* treeGroup)
    {
        b2DynamicTree_Rebuild(&treeGroup->m_dynamicTree, fullBuild);
    }

    void Rebuild(uint8_t groupID, bool fullBuild)
    {
        b2DynamicTree_Rebuild(&m_daabbcc.m_treeGroup->m_dynamicTree, fullBuild);
    };

    void RebuildAll(bool fullBuild)
    {
        m_daabbcc.m_dynamicTreeGroup.Iterate(RebuildIterateCallback, &fullBuild);
    }

    ////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////

    static void AABBtoAABBManifold(b2AABB A, b2AABB B, b2Manifold* m)
    {
        m->count = 0;

        b2Vec2 mid_a = b2AABB_Center(A);
        b2Vec2 mid_b = b2AABB_Center(B);

        b2Vec2 eA = b2Abs(b2AABB_Extents(A)); // c2Absv(c2Mulvs(b2Sub(A.max, A.min), 0.5f));
        b2Vec2 eB = b2Abs(b2AABB_Extents(B));
        ; // c2Absv(c2Mulvs(b2Sub(B.max, B.min), 0.5f));
        b2Vec2 d = b2Sub(mid_b, mid_a);

        // calc overlap on x and y axes
        float dx = eA.x + eB.x - b2AbsFloat(d.x);
        if (dx < 0)
            return;
        float dy = eA.y + eB.y - b2AbsFloat(d.y);
        if (dy < 0)
            return;

        b2Vec2 n;
        float  depth;
        b2Vec2 p;

        b2Vec2 temp = b2Vec2_zero;

        // x axis overlap is smaller
        if (dx < dy)
        {
            depth = dx;
            if (d.x < 0)
            {
                n = { 1.0f, 0 };
                temp.x = eA.x;
                temp.y = 0;
                p = b2Sub(mid_a, temp);
            }
            else
            {
                n = { -1.0f, 0 };
                temp.x = eA.x;
                temp.y = 0;
                p = b2Add(mid_a, temp);
            }
        }

        // y axis overlap is smaller
        else
        {
            depth = dy;
            if (d.y < 0)
            {
                n = { -0, 1.0f }; // c2V(0, -1.0f);
                temp.x = 0;
                temp.y = eA.y;
                p = b2Sub(mid_a, temp);
            }
            else
            {
                n = { -0, -1.0f }; // c2V(0, 1.0f);
                temp.x = 0;
                temp.y = eA.y;
                p = b2Add(mid_a, temp);
            }
        }

        m->count = 1;
        m->contact_point = p;
        m->depth = depth;
        m->n = n;
    }

    static inline void Bound(b2AABB* aabb, float x, float y, uint32_t width, uint32_t height)
    {
        aabb->lowerBound = { x - (width / 2.0f), y - (height / 2.0f) };
        aabb->upperBound = { x + (width / 2.0f), y + (height / 2.0f) };
    }

    // From Defold source
    // https://github.com/defold/defold/blob/cdaa870389ca00062bfc03bcda8f4fb34e93124a/engine/engine/src/engine.cpp#L1860
    static void CalcTimeStep(float& step_dt, uint32_t& num_steps)
    {
        uint64_t time = dmTime::GetMonotonicTime();
        uint64_t frame_time = time - m_gameUpdate.m_previousFrameTime;
        m_gameUpdate.m_previousFrameTime = time;

        float frame_dt = (float)(frame_time / 1000000.0);

        // Never allow for large hitches
        if (frame_dt > 0.5f)
        {
            frame_dt = 0.5f;
        }

        // Variable frame rate
        if (m_gameUpdate.m_updateFrequency == 0)
        {
            step_dt = frame_dt;
            num_steps = 1;
            return;
        }

        // Fixed frame rate
        float fixed_dt = 1.0f / (float)m_gameUpdate.m_updateFrequency;

        // We don't allow having a higher framerate than the actual variable frame
        // rate since the update+render is currently coupled together and also Flip()
        // would be called more than once. E.g. if the fixed_dt == 1/120 and the
        // frame_dt == 1/60
        if (fixed_dt < frame_dt)
        {
            fixed_dt = frame_dt;
        }

        m_gameUpdate.m_accumFrameTime += frame_dt;

        float num_steps_f = m_gameUpdate.m_accumFrameTime / fixed_dt;

        num_steps = (uint32_t)num_steps_f;
        step_dt = fixed_dt;

        m_gameUpdate.m_accumFrameTime = m_gameUpdate.m_accumFrameTime - num_steps * fixed_dt;
    }

    void Reset()
    {
        m_daabbcc.m_dynamicTreeGroup.Iterate(RemoveGroupsIterateCallback, (void*)0x0);
        m_daabbcc.m_dynamicTreeGroup.Clear();

        m_daabbcc.m_gameObjectContainer.SetSize(0);
        m_daabbcc.m_queryResult.SetSize(0);
        m_daabbcc.m_queryManifoldResult.SetSize(0);

        m_daabbcc.m_treeGroup = NULL;
    }

    void ErrorAssert(const char* info, uint8_t groupID)
    {
        dmLogError("%s: Group ID [%i] is invalid or already removed!", info, groupID);
    }

    void LimitErrorAssert(const char* info, uint16_t count)
    {
        dmLogError("%s reached: %i", info, count);
    }

    ////////////////////////////////////////
    // Tests
    ////////////////////////////////////////

    void DumpQueryResult(char* title)
    {
        dmLogInfo("--- %s DumpQueryResult ---", title);
        for (int i = 0; i < m_daabbcc.m_queryResult.Size(); ++i)
        {
            dmLogInfo("Proxy ID: %i", m_daabbcc.m_queryResult[i]);
        }
    }

    void DumpManifoldResult(char* title)
    {
        dmLogInfo("--- %s DumpSortResult ---", title);
        for (int i = 0; i < m_daabbcc.m_queryManifoldResult.Size(); ++i)
        {
            dmLogInfo("Proxy ID: %i - Distance: %f", m_daabbcc.m_queryManifoldResult[i].m_proxyID, m_daabbcc.m_queryManifoldResult[i].m_distance);
        }
    }

} // namespace daabbcc
