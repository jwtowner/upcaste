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

#include <up/cstring.hpp>
#include <up/cassert.hpp>
#include <up/cctype.hpp>

#if UP_COMPILER == UP_COMPILER_MSVC
#   pragma warning(disable:4706) // assignment within conditional expression
#endif

namespace up
{
    LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    int fast_strncasecmp(char const* s1, char const* s2, size_t n) noexcept {
        assert((s1 && s2) || !n);

        int result = 0;

        for ( ; n && !(result = fast_tolower(*s1) - fast_tolower(*s2)) && *s1; ++s1, ++s2, --n) ;

        if (result < 0) {
            result = -1;
        }
        else if (result > 0) {
            result = 1;
        }

        return result;
    }
}