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

#ifdef UP_HAS_STDC_WCHAR

#include <up/cwchar.hpp>
#include <up/cassert.hpp>
#include <up/cstring.hpp>
#include <up/cstdlib.hpp>

namespace up
{
#ifndef UP_HAS_GNU_WCSNDUP
    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* wcsndup(wchar_t const* s, size_t n) noexcept {
        assert(s || !n);

        size_t const sz = wcsnlen(s, n);
        size_t const wsz = sz * sizeof(wchar_t);
        wchar_t* r = static_cast<wchar_t*>(malloc(wsz + sizeof(wchar_t)));
        if (!r) {
            return nullptr;
        }

        *static_cast<wchar_t*>(mempcpy(r, s, wsz)) = L'\0';
        return r;
    }
#endif

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    wchar_t* wcsndup(wchar_t const* s, size_t n, allocator* alloc) noexcept {
        assert((s || !n) && alloc);

        size_t const sz = wcsnlen(s, n);
        size_t const wsz = sz * sizeof(wchar_t);
        wchar_t* r = static_cast<wchar_t*>(alloc->allocate(wsz + sizeof(wchar_t)));
        if (!r) {
            return nullptr;
        }

        *static_cast<wchar_t*>(mempcpy(r, s, wsz)) = L'\0';
        return r;
    }
}

#endif
