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

#include <up/cctype.hpp>
#include <up/test.hpp>

namespace cctype
{
    UP_TEST_CASE(fast_islower) {
        for (int c = 0; c < 256; ++c) {
            int result = up::fast_islower(c);
            if (('a' <= c) && (c <= 'z')) {
                require(result);
            }
            else {
                require(!result);
            }
        }
    }

    UP_TEST_CASE(fast_isupper) {
        for (int c = 0; c < 256; ++c) {
            int result = up::fast_isupper(c);
            if (('A' <= c) && (c <= 'Z')) {
                require(result);
            }
            else {
                require(!result);
            }
        }
    }

    UP_TEST_CASE(fast_tolower) {
        for (int c = 0; c < 256; ++c) {
            int result = up::fast_tolower(c);
            if (('A' <= c) && (c <= 'Z')) {
                require(result == ((c - 'A') + 'a'));
            }
            else {
                require(result == c);
            }
        }
    }

    UP_TEST_CASE(fast_toupper) {
        for (int c = 0; c < 256; ++c) {
            int result = up::fast_toupper(c);
            if (('a' <= c) && (c <= 'z')) {
                require(result == ((c - 'a') + 'A'));
            }
            else {
                require(result == c);
            }
        }
    }
}
