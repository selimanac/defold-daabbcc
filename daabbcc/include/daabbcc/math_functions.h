// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

#include "base.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>

namespace daabbcc
{

    /// 2D vector
    /// This can be used to represent a point or free vector
    typedef struct b2Vec2
    {
        /// coordinates
        float x, y;
    } b2Vec2;

    /// Axis-aligned bounding box
    typedef struct b2AABB
    {
        b2Vec2 lowerBound;
        b2Vec2 upperBound;
    } b2AABB;

    static const b2Vec2 b2Vec2_zero = { 0.0f, 0.0f };

    /// @return the minimum of two floats
    static inline float b2MinFloat(float a, float b)
    {
        return a < b ? a : b;
    }

    /// @return the maximum of two floats
    static inline float b2MaxFloat(float a, float b)
    {
        return a > b ? a : b;
    }

    /// @return the absolute value of a float
    static inline float b2AbsFloat(float a)
    {
        return a < 0 ? -a : a;
    }

    /// @return the maximum of two integers
    static inline int b2MaxInt(int a, int b)
    {
        return a > b ? a : b;
    }

    /// Vector dot product
    static inline float b2Dot(b2Vec2 a, b2Vec2 b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /// Perform the cross product on a vector and a scalar. In 2D this produces a
    /// vector.
    static inline b2Vec2 b2CrossVS(b2Vec2 v, float s)
    {
        return B2_LITERAL(b2Vec2) { s * v.y, -s * v.x };
    }

    /// Perform the cross product on a scalar and a vector. In 2D this produces a
    /// vector.
    static inline b2Vec2 b2CrossSV(float s, b2Vec2 v)
    {
        return B2_LITERAL(b2Vec2) { -s * v.y, s * v.x };
    }

    /// Vector addition
    static inline b2Vec2 b2Add(b2Vec2 a, b2Vec2 b)
    {
        return B2_LITERAL(b2Vec2) { a.x + b.x, a.y + b.y };
    }

    /// Vector subtraction
    static inline b2Vec2 b2Sub(b2Vec2 a, b2Vec2 b)
    {
        return B2_LITERAL(b2Vec2) { a.x - b.x, a.y - b.y };
    }

    /// Vector linear interpolation
    /// https://fgiesen.wordpress.com/2012/08/15/linear-interpolation-past-present-and-future/
    static inline b2Vec2 b2Lerp(b2Vec2 a, b2Vec2 b, float t)
    {
        return B2_LITERAL(b2Vec2) { (1.0f - t) * a.x + t * b.x, (1.0f - t) * a.y + t * b.y };
    }

    /// a + s * b
    static inline b2Vec2 b2MulAdd(b2Vec2 a, float s, b2Vec2 b)
    {
        return B2_LITERAL(b2Vec2) { a.x + s * b.x, a.y + s * b.y };
    }

    /// Get the distance between two points
    static inline float b2Distance(b2Vec2 a, b2Vec2 b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return sqrtf(dx * dx + dy * dy);
    }

    /// Component-wise absolute vector
    static inline b2Vec2 b2Abs(b2Vec2 a)
    {
        b2Vec2 b;
        b.x = b2AbsFloat(a.x);
        b.y = b2AbsFloat(a.y);
        return b;
    }

    /// Component-wise minimum vector
    static inline b2Vec2 b2Min(b2Vec2 a, b2Vec2 b)
    {
        b2Vec2 c;
        c.x = b2MinFloat(a.x, b.x);
        c.y = b2MinFloat(a.y, b.y);
        return c;
    }

    /// Component-wise maximum vector
    static inline b2Vec2 b2Max(b2Vec2 a, b2Vec2 b)
    {
        b2Vec2 c;
        c.x = b2MaxFloat(a.x, b.x);
        c.y = b2MaxFloat(a.y, b.y);
        return c;
    }

    /// Convert a vector into a unit vector if possible, otherwise returns the zero
    /// vector.
    static inline b2Vec2 b2Normalize(b2Vec2 v)
    {
        float length = sqrtf(v.x * v.x + v.y * v.y);
        if (length < FLT_EPSILON)
        {
            return b2Vec2_zero;
        }

        float  invLength = 1.0f / length;
        b2Vec2 n = { invLength * v.x, invLength * v.y };
        return n;
    }

    /// Get the length squared of this vector
    static inline float b2LengthSquared(b2Vec2 v)
    {
        return v.x * v.x + v.y * v.y;
    }

    /// Get the distance squared between points
    static inline float b2DistanceSquared(b2Vec2 a, b2Vec2 b)
    {
        b2Vec2 c = { b.x - a.x, b.y - a.y };
        return c.x * c.x + c.y * c.y;
    }

    /// Does a fully contain b
    static inline bool b2AABB_Contains(b2AABB a, b2AABB b)
    {
        bool s = true;
        s = s && a.lowerBound.x <= b.lowerBound.x;
        s = s && a.lowerBound.y <= b.lowerBound.y;
        s = s && b.upperBound.x <= a.upperBound.x;
        s = s && b.upperBound.y <= a.upperBound.y;
        return s;
    }

    /// Get the center of the AABB.
    static inline b2Vec2 b2AABB_Center(b2AABB a)
    {
        b2Vec2 b = { 0.5f * (a.lowerBound.x + a.upperBound.x), 0.5f * (a.lowerBound.y + a.upperBound.y) };
        return b;
    }

    /// Get the extents of the AABB (half-widths).
    static inline b2Vec2 b2AABB_Extents(b2AABB a)
    {
        b2Vec2 b = { 0.5f * (a.upperBound.x - a.lowerBound.x), 0.5f * (a.upperBound.y - a.lowerBound.y) };
        return b;
    }

    /// Union of two AABBs
    static inline b2AABB b2AABB_Union(b2AABB a, b2AABB b)
    {
        b2AABB c;
        c.lowerBound.x = b2MinFloat(a.lowerBound.x, b.lowerBound.x);
        c.lowerBound.y = b2MinFloat(a.lowerBound.y, b.lowerBound.y);
        c.upperBound.x = b2MaxFloat(a.upperBound.x, b.upperBound.x);
        c.upperBound.y = b2MaxFloat(a.upperBound.y, b.upperBound.y);
        return c;
    }

    /// Is this a valid number? Not NaN or infinity.
    bool b2IsValidFloat(float a);

    /// Is this a valid vector? Not NaN or infinity.
    bool b2IsValidVec2(b2Vec2 v);

    /// Is this a valid bounding box? Not Nan or infinity. Upper bound greater than
    /// or equal to lower bound.
    bool b2IsValidAABB(b2AABB aabb);

    /**@}*/

    /**
     * @defgroup math_cpp C++ Math
     * @brief Math operator overloads for C++
     *
     * See math_functions.h for details.
     * @{
     */

    // NOT USING

#ifdef __cplusplus

    /// Unary add one vector to another
    inline void operator+=(b2Vec2& a, b2Vec2 b)
    {
        a.x += b.x;
        a.y += b.y;
    }

    /// Unary subtract one vector from another
    inline void operator-=(b2Vec2& a, b2Vec2 b)
    {
        a.x -= b.x;
        a.y -= b.y;
    }

    /// Unary multiply a vector by a scalar
    inline void operator*=(b2Vec2& a, float b)
    {
        a.x *= b;
        a.y *= b;
    }

    /// Unary negate a vector
    inline b2Vec2 operator-(b2Vec2 a)
    {
        return { -a.x, -a.y };
    }

    /// Binary vector addition
    inline b2Vec2 operator+(b2Vec2 a, b2Vec2 b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    /// Binary vector subtraction
    inline b2Vec2 operator-(b2Vec2 a, b2Vec2 b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    /// Binary scalar and vector multiplication
    inline b2Vec2 operator*(float a, b2Vec2 b)
    {
        return { a * b.x, a * b.y };
    }

    /// Binary scalar and vector multiplication
    inline b2Vec2 operator*(b2Vec2 a, float b)
    {
        return { a.x * b, a.y * b };
    }

    /// Binary vector equality
    inline bool operator==(b2Vec2 a, b2Vec2 b)
    {
        return a.x == b.x && a.y == b.y;
    }

    /// Binary vector inequality
    inline bool operator!=(b2Vec2 a, b2Vec2 b)
    {
        return a.x != b.x || a.y != b.y;
    }

#endif

    /**@}*/
} // namespace daabbcc
