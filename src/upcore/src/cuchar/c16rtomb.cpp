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
    size_t c16rtomb(char* UPRESTRICT s, char16_t c16, mbstate_t* UPRESTRICT ps) noexcept {
#if WCHAR_MAX <= UINT_LEAST8_MAX
        if (c16 > 0x7F) {
            errno = EILSEQ;
            return static_cast<size_t>(-1);
        }
        wchar_t wc = static_cast<wchar_t>(c16);
#elif WCHAR_MAX <= UINT_LEAST16_MAX
        wchar_t wc = static_cast<wchar_t>(c16);
#elif WCHAR_MAX <= UINT_LEAST32_MAX 
        char32_t temp;
        int length = u16tou32(&temp, &c16, 1);
        if (length < 0) {
            errno = EILSEQ;
            return -1;
        }
        wchar_t wc = static_cast<wchar_t>(temp);
#endif
        return wcrtomb(s, wc, ps);
    }
}

#endif
