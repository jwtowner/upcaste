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

#include <up/prolog.hpp>

#ifndef UP_HAS_STDCXX_ATOMIC

#include <up/atomic.hpp>
#include <up/climits.hpp>
#include <up/cstdint.hpp>

namespace up { namespace detail
{
    namespace
    {
        constexpr size_t storage_lock_count = 16;

        struct alignas(UP_MAX_CACHE_LINE_SIZE) storage_spin_lock
        {
            atomic_flag flag;
            char cache_line_pad[UP_MAX_CACHE_LINE_SIZE - sizeof(atomic_flag)];
#if !defined(UP_NO_DEFAULTED_FUNCTIONS) && !defined(UP_NO_DELETED_FUNCTIONS) && !defined(UP_NO_CONSTEXPR)
            storage_spin_lock() = default;
            storage_spin_lock(storage_spin_lock const&) = delete;
            storage_spin_lock& operator=(storage_spin_lock const&) = delete;
            storage_spin_lock& operator=(storage_spin_lock const&) volatile = delete;
#endif
        };

#if (UP_COMPILER == UP_COMPILER_CLANG) || (UP_COMPILER == UP_COMPILER_GCC)
        storage_spin_lock storage_locks[storage_lock_count] __attribute__((section(".bss")));
#elif UP_COMPILER == UP_COMPILER_MSVC
#       pragma bss_seg()
        storage_spin_lock storage_locks[storage_lock_count];
#endif
    }

    LIBUPCOREAPI
    atomic_flag* atomic_storage_spin_lock(void const volatile* storage_ptr) noexcept {
        uintptr_t key = reinterpret_cast<uintptr_t>(storage_ptr);
#if UINTPTR_MAX > 0xFFFFFFFF
        key ^= key >> 7;
        key ^= key << 19;
        key ^= key >> 17;
#else
        key ^= key >> 5;
        key ^= key << 15;
        key ^= key >> 17;
#endif
        size_t index = static_cast<size_t>(key & (storage_lock_count - 1));
        return &storage_locks[index].flag;
    }
}}

#endif
