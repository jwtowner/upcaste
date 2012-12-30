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

#include <up/cuchar.hpp>
#include <up/cassert.hpp>
#include "cuchar_internal.inl"

namespace up
{
    LIBUPCOREAPI size_t u8scspn(char const* s, char const* reject) noexcept {
        assert(s && reject);

        // fast-path
        if (reject[0] == 0) {
            return ::up::strlen(s);
        }
        
        // normal-path
        char const* start = s;

        for (;;) {
            // determine start and end bounds of current character in 's'
            char const* char_ptr, * end_ptr, * s_next;
            int char_length = ::up::u8len(s);
            if (char_length == 0) {
                break;
            }
            else if (char_length > 0) {
                char_ptr = s;
                end_ptr = s + char_length;
                s_next = end_ptr;
            }
            else {
                char_ptr = reinterpret_cast<char const*>(detail::u8_replacement_sequence);
                end_ptr = reinterpret_cast<char const*>(detail::u8_replacement_sequence) + 3;
                s_next = s + 1;
            }

            // search for current character sequence in 'reject'
            for (char const* r = reject; *r; ++r) {
                char const* c = char_ptr;
                for (char const* r2 = r; (c < end_ptr) && !(*r2 - *c); ++r2, ++c) ;
                if (c == end_ptr) {
                    return static_cast<size_t>(s - start);
                }
            }
            
            s = s_next;
        }

        return static_cast<size_t>(s - start);
    }
}
