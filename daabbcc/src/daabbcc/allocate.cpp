// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#include "daabbcc/allocate.h"

namespace daabbcc
{

#define b2TracyCAlloc(ptr, size)
#define b2TracyCFree(ptr)

    static b2AllocFcn* b2_allocFcn = NULL;
    static b2FreeFcn*  b2_freeFcn = NULL;

    static _Atomic int b2_byteCount;

    void               b2SetAllocator(b2AllocFcn* allocFcn, b2FreeFcn* freeFcn)
    {
        b2_allocFcn = allocFcn;
        b2_freeFcn = freeFcn;
    }

// Use 32 byte alignment for everything. Works with 256bit SIMD.
#define B2_ALIGNMENT 32

    void* b2Alloc(uint32_t size)
    {
        // This could cause some sharing issues, however Box2D rarely calls b2Alloc.
        atomic_fetch_add_explicit(&b2_byteCount, size, memory_order_relaxed);

        // Allocation must be a multiple of 32 or risk a seg fault
        // https://en.cppreference.com/w/c/memory/aligned_alloc
        uint32_t size32 = ((size - 1) | 0x1F) + 1;

        if (b2_allocFcn != NULL)
        {
            void* ptr = b2_allocFcn(size32, B2_ALIGNMENT);
            b2TracyCAlloc(ptr, size);

            B2_ASSERT(ptr != NULL);
            B2_ASSERT(((uintptr_t)ptr & 0x1F) == 0);

            return ptr;
        }

#ifdef B2_PLATFORM_WINDOWS
        void* ptr = _aligned_malloc(size32, B2_ALIGNMENT);
#elif defined(B2_PLATFORM_ANDROID)
        void* ptr = NULL;
        if (posix_memalign(&ptr, B2_ALIGNMENT, size32) != 0)
        {
            // allocation failed, exit the application
            exit(EXIT_FAILURE);
        }
#else
        void* ptr = aligned_alloc(B2_ALIGNMENT, size32);
#endif

        b2TracyCAlloc(ptr, size);

        B2_ASSERT(ptr != NULL);
        B2_ASSERT(((uintptr_t)ptr & 0x1F) == 0);

        return ptr;
    }

    void b2Free(void* mem, uint32_t size)
    {
        if (mem == NULL)
        {
            return;
        }

        b2TracyCFree(mem);

        if (b2_freeFcn != NULL)
        {
            b2_freeFcn(mem);
        }
        else
        {
#ifdef B2_PLATFORM_WINDOWS
            _aligned_free(mem);
#else
            free(mem);
#endif
        }

        atomic_fetch_sub_explicit(&b2_byteCount, size, memory_order_relaxed);
    }

    int b2GetByteCount(void)
    {
        return atomic_load_explicit(&b2_byteCount, memory_order_relaxed);
    }
} // namespace daabbcc
