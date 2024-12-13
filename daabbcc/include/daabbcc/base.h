// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

namespace daabbcc
{
    // clang-format off
	#define B2_LITERAL(T) T
	#define B2_ZERO_INIT {}
    // clang-format on

    /// Prototype for user allocation function
    ///	@param size the allocation size in bytes
    ///	@param alignment the required alignment, guaranteed to be a power of 2
    typedef void* b2AllocFcn(unsigned int size, int alignment);

    /// Prototype for user free function
    ///	@param mem the memory previously allocated through `b2AllocFcn`
    typedef void b2FreeFcn(void* mem);

    /// Prototype for the user assert callback. Return 0 to skip the debugger break.
    typedef int b2AssertFcn(const char* condition, const char* fileName, int lineNumber);

    /// This allows the user to override the allocation functions. These should be
    ///	set during application startup.
    void b2SetAllocator(b2AllocFcn* allocFcn, b2FreeFcn* freeFcn);

    /// Override the default assert callback
    ///	@param assertFcn a non-null assert callback
    void b2SetAssertFcn(b2AssertFcn* assertFcn);

    // see https : // github.com/scottt/debugbreak
#if defined(_MSC_VER)
#define B2_BREAKPOINT __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define B2_BREAKPOINT __builtin_trap()
#else
// Unknown compiler
#include <assert.h>
#define B2_BREAKPOINT assert(0)
#endif

#if !defined(NDEBUG) || defined(B2_ENABLE_ASSERT)
    int
    b2InternalAssertFcn(const char* condition, const char* fileName, int lineNumber);
#define B2_ASSERT(condition) \
    do \
    { \
        if (!(condition) && b2InternalAssertFcn(#condition, __FILE__, (int)__LINE__)) \
            B2_BREAKPOINT; \
    } while (0)
#else
#define B2_ASSERT(...) ((void)0)
#endif

} // namespace daabbcc
