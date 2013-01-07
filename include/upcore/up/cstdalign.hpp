//
//  Upcaste Performance Libraries
//  Copyright (C) 2012-2013 Jesse W. Towner
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef UP_CSTDALIGN_HPP
#define UP_CSTDALIGN_HPP

#include <up/cstddef.hpp>
#include <up/cstdint.hpp>

#undef __alignas_is_defined
#if !defined(UP_NO_ALIGNAS) || defined(alignas)
#   define __alignas_is_defined 1
#endif

#undef __alignof_is_defined
#if !defined(UP_NO_ALIGNOF) || defined(alignof)
#   define __alignof_is_defined 1
#endif

namespace up
{
    extern LIBUPCOREAPI
    void* align(size_t alignment, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept;

    extern LIBUPCOREAPI
    void* align(size_t alignment, size_t offset, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept;

    extern LIBUPCOREAPI
    void* align_bidirectional(size_t alignment, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept;

    extern LIBUPCOREAPI
    void* align_bidirectional(size_t alignment, size_t offset, size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept;

    inline UPALWAYSINLINE
    void align_advance(size_t size, void** UPRESTRICT ptr, size_t* UPRESTRICT space) noexcept {
        *ptr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(*ptr) + size);
        *space -= size;
    }
}

#endif
