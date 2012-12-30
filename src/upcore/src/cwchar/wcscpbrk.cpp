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

#ifdef UP_HAS_STDC_WCHAR

#include <up/cwchar.hpp>
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPPURE UPNONNULL(1,2)
    wchar_t const* wcscpbrk(wchar_t const* s, wchar_t const* reject) noexcept {
        assert(s && reject);

        for ( ; *s; ++s) {
            wint_t const c = *s;
            bool match = false;

            for (wchar_t const* r = reject; *r; ++r) {
                if (*r == c) {
                    match = true;
                    break;
                }
            }

            if (!match) {
                return s;
            }
        }

        return nullptr;
    }
}

#endif
