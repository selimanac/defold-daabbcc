#pragma once

#include <cstdint>
#include <daabbcc/collision.h>
#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/hashtable.h>
#include <dmsdk/dlib/log.h>
#include <dmsdk/dlib/time.h>
#include <dmsdk/gameobject/gameobject.h>

#define JC_SORT_IMPLEMENTATION
#include <jc/sort.h>

namespace daabbcc
{

// For Query sorting using JC/SORT
struct SortResult
{
  int32_t m_proxyID;
  float m_distance;
};

// For Query callbacks
struct QueryContainer
{
  int32_t m_proxyID;
  b2Vec2 m_center;
  bool m_isAABB;
};

// Update dt calc
struct GameUpdate
{
  uint8_t m_updateFrequency = 0;
  uint64_t m_previousFrameTime;
  float m_accumFrameTime;
  bool m_updateLoopState = true;
  uint32_t m_loopCounter;
};

struct GameObject
{
  uint8_t m_groupID;
  int32_t m_proxyID;
  dmVMath::Point3 m_position;
  dmGameObject::HInstance m_gameObjectInstance;
  uint32_t m_width;
  uint32_t m_height;
};

enum TreeBuildType
{
  UPDATE_INCREMENTAL,
  UPDATE_FULLREBUILD,
  UPDATE_PARTIALREBUILD
};

/*
TODO
-- Update all moving proxies at once from lua as table to C
*/

struct DAABBCC
{
  struct TreeGroup
  {
    b2DynamicTree m_dynamicTree;
    TreeBuildType m_buildType = UPDATE_FULLREBUILD;
  };

  //b2DynamicTree Group container
  dmHashTable<uint8_t, TreeGroup> m_dynamicTreeGroup;

  TreeGroup* m_treeGroup;

  // Raycast result
  dmArray<uint16_t> m_rayResult;

  // Raycast short result
  dmArray<SortResult> m_sortRayResults;
  dmArray<SortResult> m_tempSortRayResults;
  SortResult m_sortRayResult;

  // Query result
  dmArray<uint16_t> m_queryResult;

  // Query short result
  dmArray<SortResult> m_sortResults;
  dmArray<SortResult> m_tempSortResults;
  SortResult m_sortResult;

  // Increment Group ID
  uint8_t m_groupID = 0;
  uint8_t m_currentGroupID = 0;

  // Query temp AABB, AABB Center, SortContainer
  b2AABB m_aabb;
  b2Vec2 m_aabbCenter;
  QueryContainer m_queryContainer;

  //Ray-cast input
  b2RayCastInput m_raycast_input;

  // Game-object
  dmArray<GameObject> m_GameObjectContainer;
  GameObject* m_gameObject;

  // Max allocations
  const uint8_t m_max_group_count = 3;
  const uint16_t m_max_gameobject_count = 128;
  const uint16_t m_max_query_count = 32;
  const uint16_t m_max_raycast_count = 32;
};

////////////////////////////////////////
// Initialize dynamic tree
////////////////////////////////////////

void Initialize();

void Initialize(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count, uint16_t max_raycast_count);

static void Setup(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count, uint16_t max_raycast_count);

////////////////////////////////////////
// Group Operations
////////////////////////////////////////

uint8_t AddGroup(uint8_t treeBuildType);

void RemoveGroup(uint8_t groupID);

bool SetTreeGroup(uint8_t groupID); // THIS IS FOR INTERNAL USE ONLY

////////////////////////////////////////
// Proxy Operations
////////////////////////////////////////

int32_t AddProxy(uint8_t groupID, float x, float y, uint32_t width, uint32_t height, uint64_t categoryBits);

void AddGameObject(uint8_t groupID, int32_t proxyID, dmVMath::Point3 position, uint32_t width, uint32_t height, dmGameObject::HInstance gameObjectInstance);

void MoveProxy(int32_t proxyID, float x, float y, uint32_t width, uint32_t height);

void UpdateGameobjectSize(uint8_t groupID, int32_t proxyID, uint32_t width, uint32_t height);

void RemoveProxy(uint8_t groupID, int32_t proxyID);

////////////////////////////////////////
// Query Callbacks
////////////////////////////////////////

static bool QueryCallback(int32_t proxyID, int32_t groupID, void* context);

static bool QuerySortCallback(int32_t proxyID, int32_t groupID, void* context);

////////////////////////////////////////
// Query Operations
////////////////////////////////////////

static void Query(b2AABB* aabb, b2TreeQueryCallbackFcn* callback, void* context, uint64_t maskBits);

static void QuerySort(int32_t proxyID, uint64_t maskBits, bool isAABB);

void QueryAABB(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits);

void QueryID(int32_t proxyID, uint64_t maskBits);

void QueryAABBSort(float x, float y, uint32_t width, uint32_t height, uint64_t maskBits);

void QueryIDSort(int32_t proxyID, uint64_t maskBits);

uint32_t GetQueryResultSize();
uint32_t GetQuerySortResultSize();

dmArray<uint16_t>& GetQueryResults();
dmArray<SortResult>& GetQuerySortResults();

////////////////////////////////////////
// Raycast Operations
////////////////////////////////////////

static float RayCastCallback(const b2RayCastInput* input, int32_t proxyID, int32_t groupID, void* context);

static float RayCastSortCallback(const b2RayCastInput* input, int32_t proxyID, int32_t groupID, void* context);

void RayCast(uint8_t groupID, float start_x, float start_y, float end_x, float end_y, uint64_t maskBits);

void RayCastSort(uint8_t groupID, float start_x, float start_y, float end_x, float end_y, uint64_t maskBits);

uint32_t GetRayResultSize();
uint32_t GetRaySortResultSize();

dmArray<uint16_t>& GetRayResults();
dmArray<SortResult>& GetRaySortResults();

////////////////////////////////////////
// Gameobject Update Operations
////////////////////////////////////////

void Run(bool toggle);

void SetUpdateFrequency(int32_t updateFrequency);

void GameObjectUpdate();

////////////////////////////////////////
// Tree Operations
////////////////////////////////////////

static inline void RemoveGroupsIterateCallback(void*, const uint8_t* key, DAABBCC::TreeGroup* treeGroup);

static inline void GameobjectRebuildIterateCallback(void*, const uint8_t* key, DAABBCC::TreeGroup* treeGroup);

static inline void RebuildIterateCallback(bool* fullBuild, const uint8_t* key, DAABBCC::TreeGroup* treeGroup);

void Rebuild(uint8_t groupID, bool fullBuild);

void RebuildAll(bool fullBuild);

////////////////////////////////////////
// Helpers
////////////////////////////////////////

static void Bound(b2AABB* aabb, float x, float y, uint32_t width, uint32_t height);

static void CalcTimeStep(float& step_dt, uint32_t& num_steps);

void Reset();

void ErrorAssert(const char* info, uint8_t groupID);

void LimitErrorAssert(const char* info, uint16_t count);

////////////////////////////////////////
// Tests
////////////////////////////////////////
void DumpQueryResult(char* title);
void DumpRayResult(char* title);
void DumpSortResult(char* title);

} // namespace daabbcc
