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

#if defined(UP_HAS_STDC_WCHAR) && !defined(UP_HAS_POSIX_WCSNRTOMBS)

#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstdlib.hpp>
#include <up/cwchar.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(2)
    size_t wcsnrtombs(
        char* UPRESTRICT dst,
        wchar_t const** UPRESTRICT src,
        size_t nwc,
        size_t len,
        mbstate_t* UPRESTRICT ps
    )
    noexcept {
        assert(src && (*src || !nwc));

        size_t i, length, total_length;
        wchar_t const* src_cur = *src;
        wchar_t const* const src_end = src_cur + nwc;
        char* dst_cur = dst;
        char* const dst_end = dst_cur + len;
        char* buffer = static_cast<char*>(alloca(MB_CUR_MAX));

        if (!dst_cur) {
            // dst is nullptr, no conversion performed, just determine length
            // of src in multi-byte characters
            for (total_length = 0; (src_cur < src_end) && *src_cur; ++src_cur) {
                length = wcrtomb(buffer, *src_cur, ps);
                if (length == static_cast<size_t>(-1)) {
                    return length;
                }
                total_length += length;
            }
            return total_length;
        }
        
        // convert up to nwc wide characters from src into dst multi-byte
        // character buffer, stopping at a null character or an invalid conversion
        for ( ; src_cur < src_end; ++src_cur) {
            length = wcrtomb(buffer, *src_cur, ps);
            if (length == static_cast<size_t>(-1)) {
                return length;
            }
            else if ((dst_cur + length) > dst_end) {
                break;
            }

            for (i = 0; i < length; ++dst_cur, ++i) {
                *dst_cur = buffer[i];
            }

            if (!*src_cur) {
                if (!mbsinit(ps)) {
                    errno = EILSEQ;
                    return static_cast<size_t>(-1);
                }
                src_cur = nullptr;
                --dst_cur;
                break;
            }
        }

        *src = src_cur;
        return static_cast<size_t>(dst_cur - dst);
    }
}

#endif
