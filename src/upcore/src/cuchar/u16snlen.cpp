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

#include <up/cuchar.hpp>
#include <up/cassert.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI size_t u16snlen(char16_t const* s, size_t n) noexcept {
        assert(s || !n);

        char16_t const* unit_ptr = s, * unit_end = s + n;
        
        if (n == 0) {
            return 0;
        }
        
        // test to ensure we can align the string on machine word boundary
        if (reinterpret_cast<uintptr_t>(s) & (sizeof(char16_t) - 1)) {
            goto unaligned;
        }

        // align string pointer on machine word boundary
        for ( ; reinterpret_cast<uintptr_t>(unit_ptr) & (sizeof(uintptr_t) - 1); ++unit_ptr) {
            if (*unit_ptr == 0) {
                if (unit_ptr > unit_end) {
                    unit_ptr = unit_end;
                }
                
                return unit_ptr - s;
            }
        }

        // scan string for null-terminator a machine word at a time
        for ( ; unit_ptr < unit_end; unit_ptr += (sizeof(uintptr_t) / sizeof(char16_t))) {
            uintptr_t block = *reinterpret_cast<uintptr_t const*>(unit_ptr);
            if (::up::detail::null16_test(block)) {
                unit_ptr += ::up::detail::null16_test_locate(block);
                break;
            }
        }

        if (unit_ptr > unit_end) {
            unit_ptr = unit_end;
        }
        
        return unit_ptr - s;
        
    unaligned:

        // assume machine supports unaligned loads
        char16_t const* unit_end_4 = unit_ptr + (n >> 2);
        while (unit_ptr < unit_end_4) {
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
        }

        for ( ; (unit_ptr < unit_end) && *unit_ptr; ++unit_ptr) ;
        return unit_ptr - s;
    }
}
