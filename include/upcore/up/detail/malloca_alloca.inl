//
//  Upcaste Performance Libraries
//  Copyright (C) 2012 Jesse W. Towner
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

#ifndef UP_DETAIL_MALLOCA_ALLOCA_INL
#define UP_DETAIL_MALLOCA_ALLOCA_INL

#ifndef UP_CSTDLIB_HPP
#   error "Do not include this header directly. Instead include <up/cstdlib.hpp>"
#endif

#if (SIZE_MAX == 0xFFFFFFFFu)
#   define UP_DETAIL_MALLOCA_MAGIC (0xDEADBEEFu)
#elif (SIZE_MAX == 0xFFFFFFFFFFFFFFFFull)
#   define UP_DETAIL_MALLOCA_MAGIC (0xBADF00D0DEADBEEFull)
#endif

#define UP_DETAIL_MALLOCA_MAGIC_SIZE ((sizeof(::up::size_t) + (alignof(::up::max_align_t) - 1)) & ~(alignof(::up::max_align_t) - 1))
#define UP_DETAIL_MALLOCA_GUARD_SIZE (UP_MIN_TLB_PAGE_SIZE - 64)

#define malloca(n) \
    ( ((n) < (UP_DETAIL_MALLOCA_GUARD_SIZE - UP_DETAIL_MALLOCA_MAGIC_SIZE)) \
    ? ((void*)((char*)(*((::up::size_t*)alloca((n) + UP_DETAIL_MALLOCA_MAGIC_SIZE)) = ::up::size_t(0)) + UP_DETAIL_MALLOCA_MAGIC_SIZE)) \
    : ((void*)(::up::detail::malloca_impl(n))) )

#define freea(p) \
    { \
        if (p && (UP_DETAIL_MALLOCA_MAGIC == *((::up::size_t*)(((char*)p) - UP_DETAIL_MALLOCA_MAGIC_SIZE)))) { \
            ::up::detail::freea_impl(p); \
        } \
    }

namespace up { namespace detail
{
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    void* malloca_impl(size_t n) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void freea_impl(void* p) noexcept;
}}

#endif
