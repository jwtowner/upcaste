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

#include <up/prolog.hpp>

#ifndef UP_HAS_POSIX_STRTOK_R

#include <up/cstring.hpp>
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(2,3)
    char* strtok_r(char* UPRESTRICT s, char const* UPRESTRICT delim, char** UPRESTRICT state) noexcept {
        assert(delim && state);

        if (!s) {
            s = *state;
        }
        
        s += strspn(s, delim);
        if (*s == 0) {
            *state = s;
            return nullptr;
        }
        
        char* token = s;
        s = strpbrk(s, delim);
        if (!s) {
            *state = strchr(token, 0);
            return nullptr;
        }

        *s = 0;
        *state = s + 1;
        return token;
    }
}

#endif
