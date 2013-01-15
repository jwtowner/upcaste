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

#include <up/cassert.hpp>
#include <up/cstring.hpp>
#include <up/cstdlib.hpp>

namespace up
{
#ifndef UP_HAS_GNU_STRNDUP
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* strndup(char const* s, size_t n) noexcept {
        assert(s);

        size_t sz = strlen(s);
        if (sz > n) {
            sz = n;
        }

        void* d = malloc(sz + 1);
        if (!d) {
            return nullptr;
        }

        *static_cast<char*>(mempcpy(d, s, sz)) = '\0';
        return static_cast<char*>(d);
    }
#endif

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* strndup(char const* UPRESTRICT s, size_t n, allocator* UPRESTRICT alloc) noexcept {
        assert(s && alloc);

        size_t sz = strlen(s);
        if (sz > n) {
            sz = n;
        }

        void* d = alloc->allocate(sz + 1);
        if (!d) {
            return nullptr;
        }

        *static_cast<char*>(mempcpy(d, s, sz)) = '\0';
        return static_cast<char*>(d);
    }
}
