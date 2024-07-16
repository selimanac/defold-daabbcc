/*
 * Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <assert.h>
#include <float.h>
#include <stddef.h>

namespace daabbcc {

#if !defined(NDEBUG)
#define b2DEBUG
#endif

#define B2_NOT_USED(x) ((void)(x))
#define b2Assert(A) assert(A)

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

#define b2_maxFloat FLT_MAX
#define b2_epsilon FLT_EPSILON
#define b2_pi 3.14159265359f

/// The maximum number of vertices on a convex polygon. You cannot increase
/// this too much because b2BlockAllocator has a maximum object size.
#define b2_maxPolygonVertices 8

/// This is used to fatten AABBs in the dynamic tree. This allows proxies
/// to move by a small amount without triggering a tree adjustment.
/// This is in meters.
#define b2_aabbExtension 0.1f

/// This is used to fatten AABBs in the dynamic tree. This is used to predict
/// the future position based on the current displacement.
/// This is a dimensionless multiplier.
#define b2_aabbMultiplier 2.0f

// Memory Allocation

/// Implement this function to use your own memory allocator.
void *b2Alloc(int32 size);

/// If you implement b2Alloc, you should also implement this function.
void b2Free(void *mem);

/// Logging function.
void b2Log(const char *string, ...);

/// Version numbering scheme.
/// See http://en.wikipedia.org/wiki/Software_versioning
struct b2Version {
  int32 major;    ///< significant changes
  int32 minor;    ///< incremental changes
  int32 revision; ///< bug fixes
};

/// Current version.
extern b2Version b2_version;
} // namespace daabbcc
