/*
ABOUT:
    A radix sort function
        - Supports values with cast operator to uint64_t

VERSION:
    1.00 - (2018-01-21) Added radix_sort_stable()
    1.01 - (2021-01-05)
        * Added radix_sort_n()
        * Renamed radix_sort_stable() to radix_sort()

LICENSE:

    The MIT License (MIT)

    Copyright (c) 2018-2021 Mathias Westerdahl

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.


DISCLAIMER:

    This software is supplied "AS IS" without any warranties and support

*/

#pragma once

namespace jc
{
    /**
     * Sorts an array of items of type T. The entire struct of type T is used as a key.
     * The result is written to the input array (begin)
     *
     * @param begin the start of the contiguous array
     * @param end the end of the contiguous array
     * @param tmp a temporary array, it must be at least as large as the input array
     */
    template<typename T>
    void radix_sort(T* begin, T* end, T* tmp);

    /**
     * Sorts an array of items of type T, using the N first bytes as the key.
     * The result is written to the input array (begin)
     *
     * @param begin the start of the contiguous array
     * @param end the end of the contiguous array
     * @param tmp a temporary array, it must be at least as large as the input array
     */
    template<int N, typename T>
    void radix_sort_n(T* begin, T* end, T* tmp);
}

#ifdef JC_SORT_IMPLEMENTATION

#include <string.h> // memcpy
#include <assert.h>

// Notes on radix sort
// Pierre Terdiman, http://codercorner.com/RadixSortRevisited.htm
// Multiple Histogramming:
// Michael Herf, http://stereopsis.com/radix.html
// https://github.com/gorset/radix/blob/master/radix.cc
// http://www.drdobbs.com/parallel/parallel-in-place-radix-sort-simplified/229000734?pgno=2
// https://github.com/coderodde/radixsort.cpp/blob/master/radixsort.cpp/radixsort.h
// https://github.com/maybeshewill/openmp_msd_radix/blob/master/radix.cpp

namespace jc
{

template<int N, typename T>
void radix_sort_stable_internal(T* begin, T* end, T* out){
    #define KEY_TO_INDEX(_X) ((*(_X) >> shift) & 0xFF)
#if defined(__GNUC__)
    #define PREFETCH(_A) __builtin_prefetch(_A, 0, 2)
#else
    #define PREFETCH
#endif

    size_t size = (size_t)(end - begin);

    uint32_t counts[N][256] = {{0}};
    for( size_t i = 0; i < size; ++i )
    {
        PREFETCH(begin + i + 2);
        uint8_t* key = (uint8_t*)&begin[i];
        for (uint32_t c = 0; c < N; ++c)
        {
            counts[c][key[c]]++;
        }
    }

    uint32_t offsets[N][256] = {{0}};
    for (uint32_t c = 0; c < N; ++c)
    {
        offsets[c][0] = 0;
        for( size_t i = 1; i < 256; ++i )
        {
            PREFETCH(&counts[c][0] + i + 3);
            offsets[c][i] = offsets[c][i-1] + counts[c][i-1];
        }
    }

    for (int32_t c = 0; c < N; ++c)
    {
        uint32_t* curoffsets = &offsets[c][0];
        for( uint32_t i = 0; i < size; ++i )
        {
            PREFETCH(begin + i + 2);
            uint8_t* key = (uint8_t*)&begin[i];
            out[curoffsets[key[c]]++] = begin[i]; // Also increment the bucket, so the next item ends up in the correct place
        }

        T* tmp = begin;
        begin = out;
        out = tmp;
    }
#undef KEY_TO_INDEX
#undef PREFETCH

#if defined(_MSC_VER)
    #pragma warning( push )
    #pragma warning( disable : 4127 ) // disable constant expression
#endif

    if (N & 1)
        memcpy(out, begin, size*sizeof(T));

#if defined(_MSC_VER)
    #pragma warning( pop )
#endif
}

template<typename T>
void radix_sort(T* begin, T* end, T* tmp){
    radix_sort_stable_internal<sizeof(T), T>(begin, end, tmp);
}

template<int N, typename T>
void radix_sort_n(T* begin, T* end, T* tmp){
    assert(N <= (int)sizeof(T));
    radix_sort_stable_internal<N, T>(begin, end, tmp);
}


} // namespace

#endif
