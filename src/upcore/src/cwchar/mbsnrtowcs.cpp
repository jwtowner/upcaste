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

#if defined(UP_HAS_STDC_WCHAR) && !defined(UP_HAS_POSIX_MBSNRTOWCS)

#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstdlib.hpp>
#include <up/cwchar.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(2)
    size_t mbsnrtowcs(
        wchar_t* UPRESTRICT dst,
        char const** UPRESTRICT src,
        size_t nmc,
        size_t len,
        mbstate_t* UPRESTRICT ps
    )
    noexcept {
        assert(src && (*src || !nmc));

        size_t length, total_length;
        char const* src_cur = *src;
        wchar_t* dst_cur = dst;
        wchar_t* const dst_end = dst_cur + len;
        
        if (!dst_cur) {
            // dst is nullptr, no conversion performed, just determine length
            // of src in wide characters
            for (total_length = 0; nmc; nmc -= length, src_cur += length, total_length += length) {
                length = mbrlen(src_cur, nmc, ps);
                if (!length) {
                    break;
                }
                else if ((length | 1) == static_cast<size_t>(-1)) {
                    errno = EILSEQ;
                    return static_cast<size_t>(-1);
                }
            }
            return total_length;
        }
        
        // convert up to nmc bytes from src into the dst wide character
        // buffer, stopping at a null character or an invalid conversion
        for ( ; nmc && (dst_cur < dst_end); nmc -= length, src_cur += length, ++dst_cur) {
            length = mbrtowc(dst_cur, src_cur, nmc, ps);
            if (!length) {
                if (!mbsinit(ps)) {
                    errno = EILSEQ;
                    return static_cast<size_t>(-1);
                }
                src_cur = nullptr;
                break;
            }
            else if ((length | 1) == static_cast<size_t>(-1)) {
                errno = EILSEQ;
                return static_cast<size_t>(-1);
            }
        }

        *src = src_cur;
        return static_cast<size_t>(dst_cur - dst);
    }
}

#endif
