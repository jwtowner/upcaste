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

#ifndef UP_DETAIL_ATOMIC_CXX11_INL
#define UP_DETAIL_ATOMIC_CXX11_INL

#ifndef UP_ATOMIC_HPP
#   error "Do not include this header file directly! Instead include <up/atomic.hpp>"
#endif

#include <atomic>

namespace up
{
    using ::std::kill_dependency;
    using ::std::memory_order;
    using ::std::memory_order_relaxed;
	using ::std::memory_order_consume;
	using ::std::memory_order_acquire;
	using ::std::memory_order_release;
	using ::std::memory_order_acq_rel;
	using ::std::memory_order_seq_cst;
    using ::std::atomic;
    using ::std::atomic_flag;
    using ::std::atomic_bool;
    using ::std::atomic_char;
    using ::std::atomic_schar;
    using ::std::atomic_uchar;
    using ::std::atomic_short;
    using ::std::atomic_ushort;
    using ::std::atomic_int;
    using ::std::atomic_uint;
    using ::std::atomic_long;
    using ::std::atomic_ulong;
    using ::std::atomic_llong;
    using ::std::atomic_ullong;
    using ::std::atomic_char16_t;
    using ::std::atomic_char32_t;
    using ::std::atomic_wchar_t;
    using ::std::atomic_int8_t;
    using ::std::atomic_uint8_t;
    using ::std::atomic_int16_t;
    using ::std::atomic_uint16_t;
    using ::std::atomic_int32_t;
    using ::std::atomic_uint32_t;
    using ::std::atomic_int64_t;
    using ::std::atomic_uint64_t;
    using ::std::atomic_int_least8_t;
    using ::std::atomic_uint_least8_t;
    using ::std::atomic_int_least16_t;
    using ::std::atomic_uint_least16_t;
    using ::std::atomic_int_least32_t;
    using ::std::atomic_uint_least32_t;
    using ::std::atomic_int_least64_t;
    using ::std::atomic_uint_least64_t;
    using ::std::atomic_int_fast8_t;
    using ::std::atomic_uint_fast8_t;
    using ::std::atomic_int_fast16_t;
    using ::std::atomic_uint_fast16_t;
    using ::std::atomic_int_fast32_t;
    using ::std::atomic_uint_fast32_t;
    using ::std::atomic_int_fast64_t;
    using ::std::atomic_uint_fast64_t;
    using ::std::atomic_intptr_t;
    using ::std::atomic_uintptr_t;
    using ::std::atomic_size_t;
    using ::std::atomic_ptrdiff_t;
    using ::std::atomic_intmax_t;
    using ::std::atomic_uintmax_t;
    using ::std::atomic_thread_fence;
    using ::std::atomic_signal_fence;
    using ::std::atomic_init;
    using ::std::atomic_is_lock_free;
    using ::std::atomic_store;
    using ::std::atomic_store_explicit;
    using ::std::atomic_load;
    using ::std::atomic_load_explicit;
    using ::std::atomic_exchange;
    using ::std::atomic_exchange_explicit;
    using ::std::atomic_compare_exchange_strong;
    using ::std::atomic_compare_exchange_strong_explicit;
    using ::std::atomic_compare_exchange_weak;
    using ::std::atomic_compare_exchange_weak_explicit;
    using ::std::atomic_fetch_add;
    using ::std::atomic_fetch_add_explicit;
    using ::std::atomic_fetch_sub;
    using ::std::atomic_fetch_sub_explicit;
    using ::std::atomic_fetch_or;
    using ::std::atomic_fetch_or_explicit;
    using ::std::atomic_fetch_xor;
    using ::std::atomic_fetch_xor_explicit;
    using ::std::atomic_fetch_and;
    using ::std::atomic_fetch_and_explicit;
    using ::std::atomic_flag_test_and_set;
    using ::std::atomic_flag_test_and_set_explicit;
    using ::std::atomic_flag_clear;
    using ::std::atomic_flag_clear_explicit;

    extern LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_explicit(atomic_flag* flag, memory_order order) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_explicit(atomic_flag volatile* flag, memory_order order) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_backoff_explicit(atomic_flag* flag, memory_order order) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    void atomic_flag_spin_lock_backoff_explicit(atomic_flag volatile* flag, memory_order order) noexcept;

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_flag_spin_lock(atomic_flag* flag) noexcept {
        atomic_flag_spin_lock_explicit(flag, memory_order_seq_cst);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_flag_spin_lock(atomic_flag volatile* flag) noexcept {
        atomic_flag_spin_lock_explicit(flag, memory_order_seq_cst);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_flag_spin_lock_backoff(atomic_flag* flag) noexcept {
        atomic_flag_spin_lock_backoff_explicit(flag, memory_order_seq_cst);
    }

    inline UPALWAYSINLINE UPNONNULLALL
    void atomic_flag_spin_lock_backoff(atomic_flag volatile* flag) noexcept {
        atomic_flag_spin_lock_backoff_explicit(flag, memory_order_seq_cst);
    }
}

#if (UP_COMPILER == UP_COMPILER_GCC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/atomic_yield_gcc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#elif (UP_COMPILER == UP_COMPILER_MSVC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/atomic_yield_msvc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#else
#   error "Compiler not currently supported for atomic operations!"
#endif

#endif
