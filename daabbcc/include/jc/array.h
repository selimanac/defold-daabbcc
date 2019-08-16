/*
ABOUT:
    A simple array class
        - Very small with very low overhead

    Currently doesn't support user allocated memory

VERSION:
    1.00 - (2018-01-22) Initial commit

LICENSE:

    The MIT License (MIT)

    Copyright (c) 2018 Mathias Westerdahl

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

#include <stdlib.h>
#include <assert.h> // disable with NDEBUG

namespace jc
{

/*
 * Uses realloc to allocate memory. Don't use for aligned types
 */
template <typename T>
class Array
{
public:
    Array() : m_Start(0), m_End(0), m_Back(0)   {}
    Array(size_t capacity)                      { SetCapacity(capacity); }
    ~Array()                                    { if (m_Start) free(m_Start); }

    T*          Begin()                         { return m_Start; }
    const T*    Begin() const                   { return m_Start; }
    T*          End()                           { return m_End; }
    const T*    End() const                     { return m_End; }
    bool        Empty() const                   { return m_End == m_Start; }
    bool        Full() const                    { return m_End == m_Back; }
    size_t      Size() const                    { return m_End - m_Start; }
    /// Gets the capacity of the array
    size_t      Capacity() const                { return m_Back - m_Start; }
    /// Increases or decreases capacity. Invalidates pointers to elements
    void        SetCapacity(size_t capacity);
    /// If the size is larger than the current capacity, it reallocates, thus invalidates pointers to elements
    void        SetSize(size_t size);
    /// Adds one item to the array. Asserts if the array is full
    void        Push(const T& item);
    /// Removes (and returns) the last item from the array. Asserts if the array is empty
    T           Pop();
    /// Removes one item by swapping in from the back.  Asserts if the index is out of range
    void        EraseSwap(size_t index);
    /// Returns the first item in the array
    T&          First()                         { assert(!Empty()); return *m_Start; }
    const T&    First() const                   { assert(!Empty()); return *m_Start; }
    /// Returns the last item in the array
    T&          Last()                          { assert(!Empty()); return *(m_End-1); }
    const T&    Last() const                    { assert(!Empty()); return *(m_End-1); }

    T&          operator[] (size_t i)           { assert(i < Size()); return m_Start[i]; }
    const T&    operator[] (size_t i) const     { assert(i < Size()); return m_Start[i]; }
private:
    T* m_Start;
    T* m_End;
    T* m_Back;

    Array(const Array<T>& rhs);
    Array<T>& operator= (const Array<T>& rhs);
    bool operator== (const Array<T>& rhs);
};

template <typename T>
void Array<T>::SetCapacity(size_t capacity)
{
    size_t old_capacity = m_Back - m_Start;
    if (capacity == old_capacity)
        return;

    size_t size = m_End - m_Start;
    m_Start = (T*)realloc(m_Start, sizeof(T) * capacity);
    m_End = m_Start + size;
    m_Back = m_Start + capacity;
}

template <typename T>
void Array<T>::SetSize(size_t size)
{
    if (size > Capacity())
        SetCapacity(size);
    m_End = m_Start + size;
}

template <typename T>
void Array<T>::Push(const T& item)
{
    assert(!Full());
    *m_End++ = item;
}

template <typename T>
T Array<T>::Pop()
{
    assert(!Empty());
    return *(--m_End);
}

template <typename T>
void Array<T>::EraseSwap(size_t index)
{
    assert(index < Size());
    m_Start[index] = *(--m_End);
}

} // namespace
