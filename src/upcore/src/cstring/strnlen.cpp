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

#ifndef UP_HAS_POSIX_STRNLEN

#include <up/cassert.hpp>
#include <up/cstring.hpp>
#include "cstring_internal.inl"

namespace up
{
    LIBUPCOREAPI UPPURE UPNONNULL(1)
    size_t strnlen(char const* s, size_t n) noexcept {
        assert(s || !n);

        if (!n) {
            return 0;
        }

        char const* unit_ptr = s;
        char const* const unit_end = s + n;
        
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
        for ( ; unit_ptr < unit_end; unit_ptr += sizeof(uintptr_t)) {
            uintptr_t block = *reinterpret_cast<uintptr_t const*>(unit_ptr);
            if (detail::null8_test(block)) {
                unit_ptr += detail::null8_test_locate(block);
                break;
            }
        }
        
        if (unit_ptr > unit_end) {
            unit_ptr = unit_end;
        }
        
        return unit_ptr - s;
    }
}

#endif
