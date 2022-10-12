#include <DynamicTree.hpp>

DynamicTree::DynamicTree()
{
    result.SetCapacity(100);
    ray_result.SetCapacity(100);

    orderResult.SetCapacity(100);
    tmpOrderResult.SetCapacity(100);
}

DynamicTree::~DynamicTree() { ResetTree(); }

// ITERATE CALLBACKS
void DynamicTree::IterateRemoveCallback(DynamicTree *context, const uint32_t *key, Groups *value)
{
    delete value->m_tree;
}

// Reset
void DynamicTree::ResetTree()
{
    ht.Iterate(IterateRemoveCallback, this);
    ht.Clear();
    m_GameObjectContainer.SetSize(0);
    result.SetSize(0);
    ray_result.SetSize(0);
    orderResult.SetSize(0);
    tmpOrderResult.SetSize(0);
}

void DynamicTree::Run(bool toggle)
{
    state = toggle;
}

void DynamicTree::Clear()
{
    ResetTree();
}

int DynamicTree::AddGroup()
{
    int c = groupCounter;

    b2DynamicTree *m_tree = new b2DynamicTree();

    Groups value;
    value.m_tree = m_tree;

    if (ht.Full())
    {
        ht.SetCapacity(ht.Capacity() + 10, ht.Capacity() + 10);
    }

    ht.Put(c, value);

    groupCounter++;

    return c;
}

void DynamicTree::AddGameObject(uint32_t groupID, int32 proxyId, dmGameObject::HInstance instance, int32 w, int32 h)
{
    GameObjectContainer goContainer;
    goContainer.groupID = groupID;
    goContainer.proxyId = proxyId;
    goContainer.instance = instance;
    goContainer.w = w;
    goContainer.h = h;

    if (m_GameObjectContainer.Full())
    {
        m_GameObjectContainer.SetCapacity(m_GameObjectContainer.Capacity() + 100);
    }
    m_GameObjectContainer.Push(goContainer);
}

void DynamicTree::GameobjectUpdate()
{
    if (!state || m_GameObjectContainer.Size() == 0)
    {
        return;
    }

    for (updateCounter = 0; updateCounter < m_GameObjectContainer.Size(); ++updateCounter)
    {
        updateContainer = &m_GameObjectContainer[updateCounter];

        goPosition = dmGameObject::GetPosition(updateContainer->instance);
        MoveProxy(updateContainer->groupID, updateContainer->proxyId, goPosition.getX(), goPosition.getY(), updateContainer->w, updateContainer->h);
    }
}

void DynamicTree::RemoveGroup(int groupId)
{
    int n = (int)m_GameObjectContainer.Size();
    for (int i = 0; i < n; ++i)
    {
        if (m_GameObjectContainer[i].groupID == groupId)
        {
            m_GameObjectContainer.EraseSwap(i);
            --n;
            --i;
        }
    }

    delete ht.Get(groupId)->m_tree;
    ht.Erase(groupId);
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

void DynamicTree::RemoveProxyGameobject(int groupID, int proxyID)
{
    for (uint32_t i = 0; i < m_GameObjectContainer.Size(); ++i)
    {
        if (m_GameObjectContainer[i].groupID == groupID && m_GameObjectContainer[i].proxyId == proxyID)
        {
            ht.Get(m_GameObjectContainer[i].groupID)->m_tree->DestroyProxy(m_GameObjectContainer[i].proxyId);
            m_GameObjectContainer.EraseSwap(i);
        }
    }
}

void DynamicTree::RemoveProxy(int groupId, int proxyID)
{
    for (int i = 0; i < m_GameObjectContainer.Size(); i++)
    {
        if (m_GameObjectContainer[i].groupID == groupId && m_GameObjectContainer[i].proxyId == proxyID)
        {
            m_GameObjectContainer.EraseSwap(i);
        }
    }

    ht.Get(groupId)->m_tree->DestroyProxy(proxyID);
}

/******************************
 *            RAY
 ******************************/

float32 DynamicTree::RayCastCallback(const b2RayCastInputAABB &input,
                                     int32 proxyId, int groupId)
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

        if (isSorted)
        {

            if (orderResult.Full())
            {
                orderResult.SetCapacity(orderResult.Capacity() + 100);
            }

            if (tmpOrderResult.Full())
            {
                tmpOrderResult.SetCapacity(orderResult.Capacity() + 100);
            }

            targetProxyCenter = GetAABBPosition(groupId, proxyId);
            // dmLogInfo("targetProxyCenter %f %f", targetProxyCenter.x,
            // targetProxyCenter.y);
            float32 distance = b2Distance(targetProxyCenter, nodeProxyCenter);
            // dmLogInfo("distance %i - %f", proxyId, distance);

            tmpOrder.proxyID = proxyId;
            tmpOrder.distance = distance;
            orderResult.Push(tmpOrder);
            tmpOrderResult.Push(tmpOrder);
        }
        else
        {
            ray_result.Push(proxyId);
        }

        // return output.fraction;
    }

    return input.maxFraction;
}

void DynamicTree::RayCastSort(int groupId, float start_x, float start_y, float end_x, float end_y)
{
    isSorted = true;
    ray_result.SetSize(0);
    orderResult.SetSize(0);
    tmpOrderResult.SetSize(0);

    b2RayCastInputAABB m_rayCastInput;
    m_rayCastInput.p1.Set(start_x, start_y);
    m_rayCastInput.p2.Set(end_x, end_y);
    m_rayCastInput.maxFraction = 1.0f;
    nodeProxyCenter.x = start_x;
    nodeProxyCenter.y = start_y;
    ht.Get(groupId)->m_tree->RayCast(this, m_rayCastInput, groupId);

    jc::radix_sort(orderResult.Begin(), orderResult.End(),
                   tmpOrderResult.Begin());
}

void DynamicTree::RayCast(int groupId, float start_x, float start_y, float end_x, float end_y)
{
    ray_result.SetSize(0);

    b2RayCastInputAABB m_rayCastInput;
    m_rayCastInput.p1.Set(start_x, start_y);
    m_rayCastInput.p2.Set(end_x, end_y);
    m_rayCastInput.maxFraction = 1.0f;

    ht.Get(groupId)->m_tree->RayCast(this, m_rayCastInput, groupId);
}

/******************************
 *          QUERY
 ******************************/

bool DynamicTree::QueryCallback(int32 proxyId, int groupId)
{

    if (nodeProxyID == -1 || nodeProxyID != proxyId)
    {
        if (result.Full())
        {
            result.SetCapacity(result.Capacity() + 100);
        }

        if (isSorted)
        {

            if (orderResult.Full())
            {
                orderResult.SetCapacity(orderResult.Capacity() + 100);
            }

            if (tmpOrderResult.Full())
            {
                tmpOrderResult.SetCapacity(orderResult.Capacity() + 100);
            }

            targetProxyCenter = GetAABBPosition(groupId, proxyId);
            // dmLogInfo("targetProxyCenter %f %f", targetProxyCenter.x,
            // targetProxyCenter.y);
            float32 distance = b2Distance(targetProxyCenter, nodeProxyCenter);
            // dmLogInfo("distance %i - %f", proxyId, distance);

            tmpOrder.proxyID = proxyId;
            tmpOrder.distance = distance;
            orderResult.Push(tmpOrder);
            tmpOrderResult.Push(tmpOrder);
        }
        else
        {
            result.Push(proxyId);
        }
    }

    return true;
}

/* Sort */
void DynamicTree::QueryIDSort(int groupId, int proxyID)
{
    isSorted = true;
    nodeProxyID = proxyID;
    nodeProxyCenter = GetAABBPosition(groupId, proxyID);
    b2AABB aabb = GetAABB(groupId, proxyID);
    Query(groupId, aabb);

    jc::radix_sort(orderResult.Begin(), orderResult.End(),
                   tmpOrderResult.Begin());
}

void DynamicTree::QueryAABBSort(int groupId, float x, float y, int w, int h)
{
    isSorted = true;
    nodeProxyID = -1;
    b2AABB aabb;
    aabb.lowerBound = Bound(0, x, y, w, h);
    aabb.upperBound = Bound(1, x, y, w, h);
    nodeProxyCenter = aabb.GetCenter();
    Query(groupId, aabb);

    jc::radix_sort(orderResult.Begin(), orderResult.End(),
                   tmpOrderResult.Begin());
}

void DynamicTree::QueryID(int groupId, int proxyID)
{
    isSorted = false;
    nodeProxyID = proxyID;
    b2AABB aabb = GetAABB(groupId, proxyID);
    Query(groupId, aabb);
}

void DynamicTree::QueryAABB(int groupId, float x, float y, int w, int h)
{
    isSorted = false;
    nodeProxyID = -1;
    b2AABB aabb;
    aabb.lowerBound = Bound(0, x, y, w, h);
    aabb.upperBound = Bound(1, x, y, w, h);
    Query(groupId, aabb);
}

void DynamicTree::Query(int groupId, b2AABB aabb)
{
    orderResult.SetSize(0);
    tmpOrderResult.SetSize(0);
    result.SetSize(0);
    ht.Get(groupId)->m_tree->Query(this, aabb, groupId);
}

void DynamicTree::updateGameobjectSize(int groupID, int proxyID, int w, int h)
{
    for (uint32_t i = 0; i < m_GameObjectContainer.Size(); ++i)
    {
        if (m_GameObjectContainer[i].groupID == groupID && m_GameObjectContainer[i].proxyId == proxyID)
        {
            m_GameObjectContainer[i].w = w;
            m_GameObjectContainer[i].h = h;
        }
    }
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

b2Vec2 DynamicTree::GetAABBPosition(int groupId, int proxyID)
{
    b2AABB aabb = GetAABB(groupId, proxyID);
    b2Vec2 aabbCenter = aabb.GetCenter();
    return aabbCenter;
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
