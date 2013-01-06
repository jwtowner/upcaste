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
    LIBUPCOREAPI
    size_t u16slen(char16_t const* s) noexcept {
        assert(s);

        char16_t const* unit_ptr = s;
     
        // test to ensure we can align the string on machine word boundary
        if (reinterpret_cast<uintptr_t>(s) & (sizeof(char16_t) - 1)) {
            goto unaligned;
        }
        
        // align string pointer on machine word boundary
        for ( ; reinterpret_cast<uintptr_t>(unit_ptr) & (sizeof(uintptr_t) - 1); ++unit_ptr) {
            if (*unit_ptr == 0) {
                return unit_ptr - s;
            }
        }

        // scan string for null-terminator a machine word at a time
        for ( ; ; unit_ptr += (sizeof(uintptr_t) / sizeof(char16_t))) {
            uintptr_t block = *reinterpret_cast<uintptr_t const*>(unit_ptr);
            if (detail::null16_test(block)) {
                unit_ptr += detail::null16_test_locate(block);
                break;
            }
        }
        
        return unit_ptr - s;

    unaligned:

        // assume machine supports unaligned loads
        for (;;) {
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
            if (!*unit_ptr) { break; }
            ++unit_ptr;
        }

        return unit_ptr - s;
    }
}
