#ifndef DynamicTree_hpp
#define DynamicTree_hpp

#include <jc/hashtable.h>
#include <jc/array.h>
#include <DynamicTree/b2DynamicTree.h>

class DynamicTree
{
public:
    DynamicTree();
    ~DynamicTree();

    // Add new group
    int AddGroup();

    // Remove group
    void RemoveGroup(int groupId);

    // Set groups
    void SetGroups(uint32_t num, uint32_t load);

    // Add new proxy(aabb)
    int32 AddProxy(int groupId, float x, float y, int w, int h);

    // Move Proxy
    void MoveProxy(int groupId, int proxyID, float x, float y, int w, int h);

    // Remove Proxy
    void RemoveProxy(int groupId, int proxyID);

    // b2DynamicTree query callback
    bool QueryCallback(int32 proxyId);

    // b2DynamicTree raycast callback
    float32 RayCastCallback(const b2RayCastInputAABB &input, int32 proxyId, int groupId);

    // RayCast to AABB
    void RayCast(int groupId, float start_x, float start_y, float end_x, float end_y); //std::vector<int32> RayCast(int groupId,  float start_x, float start_y, float end_x, float end_y);

    // Query with AABB
    void QueryAABB(int groupId, float x, float y, int w, int h); //std::vector<int32> QueryAABB(int groupId, float x, float y, int w, int h);

    // Query with ID
    void QueryID(int groupId, int proxyID); //std::vector<int32> QueryID(int groupId, int proxyID);

    // Query result
    jc::Array<int32> result;

    // Raycast result
    jc::Array<int32> ray_result;

    //Assert
    bool CheckGroup(int groupID);

private:
    //Hashtable for Groups
    struct Groups
    {
        b2DynamicTree *m_tree;
    };
    typedef jc::HashTable<uint32_t, Groups> hashtable_t;

    //Hashtable defaults
    uint32_t numelements = 20; // The maximum number of entries to store
    uint32_t load_factor = 50; // percent
    uint32_t tablesize = uint32_t(numelements / (load_factor / 100.0f));
    uint32_t sizeneeded = hashtable_t::CalcSize(tablesize);
    void *htmem = malloc(sizeneeded);
    hashtable_t ht;
    int groupCounter = 0;

    // Bound calculation
    b2Vec2 Bound(int type, float x, float y, int w, int h);

    // GetFatAABB -> GetAABB
    b2AABB GetAABB(int groupId, int proxyID);

    // Query
    void Query(int groupId, b2AABB aabb); //std::vector<int32> Query(int groupId, b2AABB aabb);

    int nodeProxyID;

    // Reset class
    void ResetTree();
};

#endif /* DynamicTree_hpp */
