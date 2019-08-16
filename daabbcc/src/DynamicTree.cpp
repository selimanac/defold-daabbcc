#include <DynamicTree.hpp>

DynamicTree::DynamicTree()
{
    result.SetCapacity(100);
    ray_result.SetCapacity(100);

    ht.Create(numelements, htmem);
}

DynamicTree::~DynamicTree()
{
    ResetTree();
}

void DynamicTree::ResetTree()
{
    hashtable_t::Iterator it = ht.Begin();
    hashtable_t::Iterator itend = ht.End();
    for (; it != itend; ++it)
    {
        delete it.GetValue()->m_tree;
    }

    free(htmem);
}

int DynamicTree::AddGroup()
{
    int c = groupCounter;

    b2DynamicTree *m_tree = new b2DynamicTree();

    Groups value;
    value.m_tree = m_tree;
    ht.Put(c, value);

    groupCounter++;

    return c;
}

void DynamicTree::RemoveGroup(int groupId)
{
    delete ht.Get(groupId)->m_tree;
    ht.Erase(groupId);
}

void DynamicTree::SetGroups(uint32_t num, uint32_t load)
{
    ResetTree();

    numelements = num; 
    load_factor = load; 
    tablesize = uint32_t(numelements / (load_factor / 100.0f));
    sizeneeded = hashtable_t::CalcSize(tablesize);
    htmem = malloc(sizeneeded);
    ht.Create(numelements, htmem);
}

int32 DynamicTree::AddProxy(int groupId, float x, float y, int w, int h)
{
    b2AABB aabb;
    aabb.lowerBound = Bound(0, x, y, w, h);
    aabb.upperBound = Bound(1, x, y, w, h);

    Groups *g = ht.Get(groupId);

    int32 proxyId = g->m_tree->CreateProxy(aabb, nullptr);

    return proxyId;
}

void DynamicTree::RemoveProxy(int groupId, int proxyID)
{
    ht.Get(groupId)->m_tree->DestroyProxy(proxyID);
}

/******************************
 *            RAY
 ******************************/

float32 DynamicTree::RayCastCallback(const b2RayCastInputAABB &input, int32 proxyId, int groupId)
{
    b2AABB aabb = GetAABB(groupId, proxyId);
    b2RayCastOutputAABB output;
    bool hit = aabb.RayCast(&output, input);
    if (hit)
    {
        if (ray_result.Full())
        {
            ray_result.SetCapacity(ray_result.Capacity() + 100);
        }
        ray_result.Push(proxyId);
        // return output.fraction;
    }

    return input.maxFraction;
}

void DynamicTree::RayCast(int groupId, float start_x, float start_y, float end_x, float end_y)
{
    ray_result.SetSize(0);

    b2RayCastInputAABB m_rayCastInput;
    m_rayCastInput.p1.Set(start_x, start_y);
    m_rayCastInput.p2.Set(end_x, end_y);
    m_rayCastInput.maxFraction = 1.0f;

    ht.Get(groupId)->m_tree->RayCast(this, m_rayCastInput, groupId);

    //return ray_result;
}

/******************************
 *          QUERY
 ******************************/

bool DynamicTree::QueryCallback(int32 proxyId)
{

    if (nodeProxyID == -1 || nodeProxyID != proxyId)
    {
        if (result.Full())
        {
            result.SetCapacity(result.Capacity() + 100);
        }
        result.Push(proxyId);
    }

    return true;
}

void DynamicTree::QueryID(int groupId, int proxyID)
{
    nodeProxyID = proxyID;
    b2AABB aabb = GetAABB(groupId, proxyID);
    Query(groupId, aabb);
    //  return Query(groupId, aabb);
}

void DynamicTree::QueryAABB(int groupId, float x, float y, int w, int h)
{
    nodeProxyID = -1;
    b2AABB aabb;
    aabb.lowerBound = Bound(0, x, y, w, h);
    aabb.upperBound = Bound(1, x, y, w, h);
    Query(groupId, aabb);
    //return Query(groupId, aabb);
}

void DynamicTree::Query(int groupId, b2AABB aabb)
{
    result.SetSize(0);

    ht.Get(groupId)->m_tree->Query(this, aabb);
    // return result;
}

void DynamicTree::MoveProxy(int groupId, int proxyID, float x, float y, int w, int h)
{
    b2AABB current_aabb = GetAABB(groupId, proxyID);

    b2AABB next_aabb;
    next_aabb.lowerBound = Bound(0, x, y, w, h);
    next_aabb.upperBound = Bound(1, x, y, w, h);

    b2Vec2 displacement; // = next_aabb.GetCenter() - current_aabb.GetCenter();

    displacement.x = 0.2;
    displacement.y = 0.2;
    ht.Get(groupId)->m_tree->MoveProxy(proxyID, next_aabb, displacement);
}

b2AABB DynamicTree::GetAABB(int groupId, int proxyID)
{
    b2AABB aabb = ht.Get(groupId)->m_tree->GetFatAABB(proxyID);
    b2Vec2 r(b2_aabbExtension, b2_aabbExtension);
    aabb.lowerBound = aabb.lowerBound + r;
    aabb.upperBound = aabb.upperBound - r;
    return aabb;
}

b2Vec2 DynamicTree::Bound(int type, float x, float y, int w, int h)
{
    b2Vec2 bound;
    if (type == 0)
    {
        bound.x = x - (w / 2);
        bound.y = y - (h / 2);
    }
    else
    {
        bound.x = x + (w / 2);
        bound.y = y + (h / 2);
    }

    return bound;
}

bool DynamicTree::CheckGroup(int groupID)
{
    if (ht.Get(groupID) == NULL)
    {
        return true;
    }
    return false;
}
