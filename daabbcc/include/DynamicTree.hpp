#pragma once

#include "DynamicTree/b2DynamicTree.h"
#include <dmsdk/dlib/hashtable.h>
#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>

#define JC_SORT_IMPLEMENTATION
#include "jc/sort.h"

namespace daabbcc {
struct orderResultValues {
  int32 proxyID;
  float32 distance;
};

class DynamicTree {
public:
  DynamicTree();
  ~DynamicTree();

  int m_UpdateFrequency; // = dmConfigFile::GetInt(m_Config,
                         // "display.update_frequency", 0);
  // Add new group
  int AddGroup();

  // Remove group
  void RemoveGroup(int groupId);

  // Add new proxy(aabb)
  int32 AddProxy(int groupId, float x, float y, int w, int h);

  // Move Proxy
  void MoveProxy(int groupId, int proxyID, float x, float y, int w, int h);

  // Update GO W&H
  void updateGameobjectSize(int groupID, int proxyID, int w, int h);

  // Remove Proxy
  void RemoveProxy(int groupID, int proxyID);

  void RemoveProxyGameobject(int groupID, int proxyID);

  // b2DynamicTree query callback
  bool QueryCallback(int32 proxyId, int groupId);

  // b2DynamicTree raycast callback
  float32 RayCastCallback(const b2RayCastInputAABB &input, int32 proxyId,
                          int groupId);

  // RayCast to AABB
  void RayCast(int groupId, float start_x, float start_y, float end_x,
               float end_y);

  void RayCastSort(int groupId, float start_x, float start_y, float end_x,
                   float end_y);

  // Query with AABB
  void QueryAABB(int groupId, float x, float y, int w, int h);

  // Query with ID
  void
  QueryID(int groupId,
          int proxyID); // std::vector<int32> QueryID(int groupId, int proxyID);

  // Query with ID - Distance Ordered
  void QueryIDSort(int groupId, int proxyID);

  void QueryAABBSort(int groupId, float x, float y, int w, int h);

  void Run(bool toggle);

  void SetUpdateFrequency(int32_t updateFrequency);

  // Query result
  dmArray<int32> result;

  // Raycast result
  dmArray<int32> ray_result;

  // Query order result
  dmArray<orderResultValues> orderResult;

  // Assert
  bool CheckGroup(int groupID);

  bool isSorted;

  void AddGameObject(uint32_t groupID, int32 proxyId,
                     dmGameObject::HInstance instance, int32 w, int32 h);

  void GameobjectUpdate();

  void Clear();

private:
  int groupCounter;
  int nodeProxyID;
  bool state;

  uint64_t m_PreviousFrameTime;
  float m_AccumFrameTime;

  struct Groups {
    b2DynamicTree *m_tree;
    dmScript::LuaCallbackInfo *cbk;
  };
  dmHashTable<uint32_t, Groups> ht;

  struct GameObjectContainer {
    uint32_t groupID;
    int32 proxyId;
    dmGameObject::HInstance instance;
    int32 w;
    int32 h;
  };
  dmArray<GameObjectContainer> m_GameObjectContainer;
  uint32_t updateCounter;
  GameObjectContainer *updateContainer;

  // Bound calculation
  b2Vec2 Bound(int type, float x, float y, int w, int h);

  // GetFatAABB -> GetAABB
  b2AABB GetAABB(int groupId, int proxyID);

  // Get AABB Position (Center)
  b2Vec2 GetAABBPosition(int groupId, int proxyID);

  // Query
  // std::vector<int32> Query(int groupId, b2AABB aabb);
  void Query(int groupId, b2AABB aabb);

  b2Vec2 nodeProxyCenter;
  b2Vec2 targetProxyCenter;
  dmVMath::Point3 goPosition;

  orderResultValues tmpOrder;
  dmArray<orderResultValues> tmpOrderResult;

  static void IterateRemoveCallback(DynamicTree *context, const uint32_t *key,
                                    Groups *value);
  void CalcTimeStep(float &step_dt, uint32_t &num_steps);

  void DestroyProxyID(int groupId, int proxyID);

  // Reset class
  void ResetTree();
};
} // namespace daabbcc
