// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once
#include "daabbcc/collision.h"
#include "daabbcc/math_functions.h"

namespace daabbcc
{

// Ray cast an AABB
b2CastOutput b2AABB_RayCast(b2AABB a, b2Vec2 p1, b2Vec2 p2);

// Get the perimeter length
static inline float b2Perimeter(b2AABB a)
{
  float wx = a.upperBound.x - a.lowerBound.x;
  float wy = a.upperBound.y - a.lowerBound.y;
  return 2.0f * (wx + wy);
}

/// Enlarge a to contain b
/// @return true if the AABB grew
static inline bool b2EnlargeAABB(b2AABB* a, b2AABB b)
{
  bool changed = false;
  if (b.lowerBound.x < a->lowerBound.x)
  {
    a->lowerBound.x = b.lowerBound.x;
    changed = true;
  }

  if (b.lowerBound.y < a->lowerBound.y)
  {
    a->lowerBound.y = b.lowerBound.y;
    changed = true;
  }

  if (a->upperBound.x < b.upperBound.x)
  {
    a->upperBound.x = b.upperBound.x;
    changed = true;
  }

  if (a->upperBound.y < b.upperBound.y)
  {
    a->upperBound.y = b.upperBound.y;
    changed = true;
  }

  return changed;
}

/// Do a and b overlap
static inline bool b2AABB_Overlaps(b2AABB a, b2AABB b)
{
  b2Vec2 d1 = {b.lowerBound.x - a.upperBound.x, b.lowerBound.y - a.upperBound.y};
  b2Vec2 d2 = {a.lowerBound.x - b.upperBound.x, a.lowerBound.y - b.upperBound.y};

  if (d1.x > 0.0f || d1.y > 0.0f)
    return false;

  if (d2.x > 0.0f || d2.y > 0.0f)
    return false;

  return true;
}
} // namespace daabbcc
