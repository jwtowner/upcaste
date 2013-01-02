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

namespace up
{
    LIBUPCOREAPI size_t u32scspn(char32_t const* s, char32_t const* reject) noexcept {
        assert(s && reject);

        // fast-path
        if (reject[0] == 0) {
            return ::up::u32slen(s);
        }
        
        // normal-path
        char32_t const* start = s;

        for ( ; ; ++s) {
            // fetch and validate current utf-32 character
            char32_t cp = *s;
            if (!cp) {
                break;
            }
            else if (((0xD800 <= cp) && (cp < 0xE000)) || (0x110000 <= cp)) {
                cp = 0x0000FFFD;
            }

            // search for current character in 'reject'
            for (char32_t const* r = reject; *r; ++r) {
                if (*r == cp) {
                    return static_cast<size_t>(s - start);
                }
            }
        }

        return static_cast<size_t>(s - start);
    }
}
