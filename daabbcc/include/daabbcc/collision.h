// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

#include "math_functions.h"
#include <stdint.h>

namespace daabbcc
{
    /**
     * @defgroup geometry Geometry
     * @brief Geometry types and algorithms
     *
     * Definitions of circles, capsules, segments, and polygons. Various algorithms
     * to compute hulls, mass properties, and so on.
     * @{
     */

    /// Low level ray-cast input data
    typedef struct b2RayCastInput
    {
        /// Start point of the ray cast
        b2Vec2 origin;

        /// Translation of the ray cast
        b2Vec2 translation;

        /// The maximum fraction of the translation to consider, typically 1
        float maxFraction;
    } b2RayCastInput;

    /// Low level ray-cast or shape-cast output data
    typedef struct b2CastOutput
    {
        /// The surface normal at the hit point
        b2Vec2 normal;

        /// The surface hit point
        b2Vec2 point;

        /// The fraction of the input translation at collision
        float fraction;

        /// The number of iterations used
        int32_t iterations;

        /// Did the cast hit?
        bool hit;
    } b2CastOutput;

/**@}*/

/**
 * @defgroup tree Dynamic Tree
 * The dynamic tree is a binary AABB tree to organize and query large numbers of
 * geometric objects
 *
 * Box2D uses the dynamic tree internally to sort collision shapes into a binary
 * bounding volume hierarchy. This data structure may have uses in games for
 * organizing other geometry data and may be used independently of Box2D rigid
 * body simulation.
 *
 * A dynamic AABB tree broad-phase, inspired by Nathanael Presson's btDbvt.
 * A dynamic tree arranges data in a binary tree to accelerate
 * queries such as AABB queries and ray casts. Leaf nodes are proxies
 * with an AABB. These are used to hold a user collision object, such as a
 * reference to a b2Shape. Nodes are pooled and relocatable, so I use node
 * indices rather than pointers. The dynamic tree is made available for advanced
 * users that would like to use it to organize spatial game data besides rigid
 * bodies.
 *
 * @note This is an advanced feature and normally not used by applications
 * directly.
 * @{
 */

/// The default category bit for a tree proxy. Used for collision filtering.
#define b2_defaultCategoryBits (1)

/// Convenience mask bits to use when you don't need collision filtering and
/// just want
///	all results.
#define b2_defaultMaskBits (UINT64_MAX)

    /// A node in the dynamic tree. This is private data placed here for performance
    /// reasons.
    typedef struct b2TreeNode
    {
        /// The node bounding box
        b2AABB aabb; // 16

        /// Category bits for collision filtering
        uint64_t categoryBits; // 8

        union
        {
            /// The node parent index
            int32_t parent;

            /// The node freelist next index
            int32_t next;
        }; // 4

        /// Child 1 index
        int32_t child1; // 4

        /// Child 2 index
        int32_t child2; // 4

        /// User data
        // todo could be union with child index
        int32_t userData; // 4

        /// Leaf = 0, free node = -1
        int16_t height; // 2

        /// Has the AABB been enlarged?
        bool enlarged; // 1

        /// Padding for clarity
        char pad[5];
    } b2TreeNode;

    /// The dynamic tree structure. This should be considered private data.
    /// It is placed here for performance reasons.
    typedef struct b2DynamicTree
    {
        /// The tree nodes
        b2TreeNode* nodes;

        /// The root index
        int32_t root;

        /// The number of nodes
        int32_t nodeCount;

        /// The allocated node space
        int32_t nodeCapacity;

        /// Node free list
        int32_t freeList;

        /// Number of proxies created
        int32_t proxyCount;

        /// Leaf indices for rebuild
        int32_t* leafIndices;

        /// Leaf bounding boxes for rebuild
        b2AABB* leafBoxes;

        /// Leaf bounding box centers for rebuild
        b2Vec2* leafCenters;

        /// Bins for sorting during rebuild
        int32_t* binIndices;

        /// Allocated space for rebuilding
        int32_t rebuildCapacity;
    } b2DynamicTree;

    /// Constructing the tree initializes the node pool.
    b2DynamicTree b2DynamicTree_Create(void);

    /// Destroy the tree, freeing the node pool.
    void b2DynamicTree_Destroy(b2DynamicTree* tree);

    /// Create a proxy. Provide an AABB and a userData value.
    int32_t b2DynamicTree_CreateProxy(b2DynamicTree* tree, b2AABB aabb, uint64_t categoryBits, int32_t userData);

    /// Destroy a proxy. This asserts if the id is invalid.
    void b2DynamicTree_DestroyProxy(b2DynamicTree* tree, int32_t proxyId);

    /// Move a proxy to a new AABB by removing and reinserting into the tree.
    void b2DynamicTree_MoveProxy(b2DynamicTree* tree, int32_t proxyId, b2AABB aabb);

    /// Enlarge a proxy and enlarge ancestors as necessary.
    void b2DynamicTree_EnlargeProxy(b2DynamicTree* tree, int32_t proxyId, b2AABB aabb);

    /// This function receives proxies found in the AABB query.
    /// @return true if the query should continue
    typedef bool b2TreeQueryCallbackFcn(int32_t proxyId, int32_t userData, void* context);

    /// Query an AABB for overlapping proxies. The callback class is called for each
    /// proxy that overlaps the supplied AABB.
    void b2DynamicTree_Query(const b2DynamicTree* tree, b2AABB aabb, uint64_t maskBits, b2TreeQueryCallbackFcn* callback, void* context);

    /// This function receives clipped raycast input for a proxy. The function
    /// returns the new ray fraction.
    /// - return a value of 0 to terminate the ray cast
    /// - return a value less than input->maxFraction to clip the ray
    /// - return a value of input->maxFraction to continue the ray cast without
    /// clipping
    typedef float b2TreeRayCastCallbackFcn(const b2RayCastInput* input, int32_t proxyId, int32_t userData, void* context);

    /// Ray-cast against the proxies in the tree. This relies on the callback
    /// to perform a exact ray-cast in the case were the proxy contains a shape.
    /// The callback also performs the any collision filtering. This has performance
    /// roughly equal to k * log(n), where k is the number of collisions and n is
    /// the number of proxies in the tree.
    ///	Bit-wise filtering using mask bits can greatly improve performance in
    /// some scenarios.
    ///	@param tree the dynamic tree to ray cast
    /// @param input the ray-cast input data. The ray extends from p1 to p1 +
    /// maxFraction * (p2 - p1)
    ///	@param maskBits filter bits: `bool accept = (maskBits &
    /// node->categoryBits) != 0;`
    /// @param callback a callback class that is called for each proxy that is hit
    /// by the ray
    ///	@param context user context that is passed to the callback
    void b2DynamicTree_RayCast(const b2DynamicTree* tree, const b2RayCastInput* input, uint64_t maskBits, b2TreeRayCastCallbackFcn* callback, void* context);

    /// Validate this tree. For testing.
    void b2DynamicTree_Validate(const b2DynamicTree* tree);

    /// Compute the height of the binary tree in O(N) time. Should not be
    /// called often.
    int b2DynamicTree_GetHeight(const b2DynamicTree* tree);

    /// Get the maximum balance of the tree. The balance is the difference in height
    /// of the two children of a node.
    int b2DynamicTree_GetMaxBalance(const b2DynamicTree* tree);

    /// Get the ratio of the sum of the node areas to the root area.
    float b2DynamicTree_GetAreaRatio(const b2DynamicTree* tree);

    /// Build an optimal tree. Very expensive. For testing.
    void b2DynamicTree_RebuildBottomUp(b2DynamicTree* tree);

    /// Get the number of proxies created
    int b2DynamicTree_GetProxyCount(const b2DynamicTree* tree);

    /// Rebuild the tree while retaining subtrees that haven't changed. Returns the
    /// number of boxes sorted.
    int b2DynamicTree_Rebuild(b2DynamicTree* tree, bool fullBuild);

    /// Shift the world origin. Useful for large worlds.
    /// The shift formula is: position -= newOrigin
    /// @param tree the tree to shift
    /// @param newOrigin the new origin with respect to the old origin
    void b2DynamicTree_ShiftOrigin(b2DynamicTree* tree, b2Vec2 newOrigin);

    /// Get the number of bytes used by this tree
    int b2DynamicTree_GetByteCount(const b2DynamicTree* tree);

    /// Get proxy user data
    /// @return the proxy user data or 0 if the id is invalid
    static inline int32_t b2DynamicTree_GetUserData(const b2DynamicTree* tree, int32_t proxyId)
    {
        return tree->nodes[proxyId].userData;
    }

    /// Get the AABB of a proxy
    static inline b2AABB b2DynamicTree_GetAABB(const b2DynamicTree* tree, int32_t proxyId)
    {
        return tree->nodes[proxyId].aabb;
    }

    /**@}*/
} // namespace daabbcc
