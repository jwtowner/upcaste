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

#ifndef UP_CSTDATOMIC_HPP
#define UP_CSTDATOMIC_HPP

#include <up/cstddef.hpp>
#include <up/cstdint.hpp>

#ifdef UP_HAS_STDCXX_ATOMIC_CXX11
#   include <up/detail/cstdatomic_cxx11.inl>
#else
#   include <up/detail/cstdatomic_generic.inl>
#endif

#ifndef ATOMIC_COUNT_INIT
#   define ATOMIC_COUNT_INIT ATOMIC_VAR_INIT(0)
#endif

namespace up
{
    template <class T>
    inline UPALWAYSINLINE
    void sink_dependency(T const& value) UPNOEXCEPT(is_nothrow_copy_constructible<T>::value) {
        T volatile sinkhole(value);
        UPIGNORE(sinkhole);
    }

    typedef atomic_int_least32_t atomic_count32;
    typedef atomic_int_least64_t atomic_count64;
#ifdef UP_ARCHITECTURE_32BIT
    typedef atomic_count32 atomic_count;
#else
    typedef atomic_count64 atomic_count;
#endif

    inline UPALWAYSINLINE UPNONNULLALL
    int_least32_t atomic_count_load(atomic_count32 const volatile* count) noexcept {
        return atomic_load_explicit(count, memory_order_acquire);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_count_retain(atomic_count32 volatile* count) noexcept {
        atomic_fetch_add_explicit(count, 1, memory_order_relaxed);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    bool atomic_count_release(atomic_count32 volatile* count) noexcept {
        if (atomic_fetch_add_explicit(count, -1, memory_order_release) != 1) {
            return false;
        }
        atomic_thread_fence(memory_order_acquire);
        return true;
    }

    inline UPALWAYSINLINE UPNONNULLALL
    int_least64_t atomic_count_load(atomic_count64 const volatile* count) noexcept {
        return atomic_load_explicit(count, memory_order_acquire);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_count_retain(atomic_count64 volatile* count) noexcept {
        atomic_fetch_add_explicit(count, 1, memory_order_relaxed);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    bool atomic_count_release(atomic_count64 volatile* count) noexcept {
        if (atomic_fetch_add_explicit(count, -1, memory_order_release) != 1) {
            return false;
        }
        atomic_thread_fence(memory_order_acquire);
        return true;
    }
}

#endif
