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

#ifndef UP_HAS_STDC_UCHAR

#include <up/cuchar.hpp>
#include <up/cerrno.hpp>
#include "cuchar_internal.inl"

namespace up
{     
    LIBUPCOREAPI
    size_t mbrtoc32(char32_t* UPRESTRICT c32, char const* UPRESTRICT s, size_t n, mbstate_t* UPRESTRICT ps) noexcept {
        wchar_t wc;
        size_t r = mbrtowc(&wc, s, n, ps);
        if (r >= static_cast<size_t>(-2)) {
            return r;
        }
#if WCHAR_MAX <= UINT_LEAST8_MAX
        if (wc > 0x7F) {
            errno = EILSEQ;
            return static_cast<size_t>(-1);
        }
        *c32 = static_cast<char32_t>(wc);
#elif WCHAR_MAX <= UINT_LEAST16_MAX
        int length = u16tou32(c32, reinterpret_cast<char16_t const*>(&wc), 1);
        if (length < 0) {
            errno = EILSEQ;
            return static_cast<size_t>(-1);
        }
#elif WCHAR_MAX <= UINT_LEAST32_MAX 
        *c32 = static_cast<char32_t>(wc);
#endif
        return r;
    }
}

#endif
