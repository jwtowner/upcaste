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
    char16_t const* u16spbrk(char16_t const* s, char16_t const* accept) noexcept {
        assert(s && accept);

        // fast-path
        if (accept[0] == 0) {
            return nullptr;
        }
        
        // normal-path
        for (;;) {
            // determine start and end bounds of current character in 's'
            char16_t const* char_ptr, * end_ptr, * s_next;
            int char_length = u16len(s);
            if (char_length == 0) {
                break;
            }
            else if (char_length > 0) {
                char_ptr = s;
                end_ptr = s + char_length;
                s_next = end_ptr;
            }
            else {
                char_ptr = detail::u16_replacement_sequence;
                end_ptr = detail::u16_replacement_sequence + 1;
                s_next = s + 1;
            }

            // search for current character sequence in 'accept'
            for (char16_t const* a = accept; *a; ++a) {
                char16_t const* c = char_ptr;
                for (char16_t const* a2 = a; (c < end_ptr) && !(*a2 - *c); ++a2, ++c) ;
                if (c == end_ptr) {
                    return s;
                }
            }
            
            s = s_next;
        }

        return nullptr;
    }
}
