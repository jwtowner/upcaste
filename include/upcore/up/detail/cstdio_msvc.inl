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

#ifndef UP_DETAIL_CSTDIO_MSVC_INL
#define UP_DETAIL_CSTDIO_MSVC_INL

#ifndef UP_CSTDIO_HPP
#   error "Do not include this header file directly! Instead include <up/cstdio.hpp>"
#endif

#ifdef UP_HAS_STDC_WCHAR
#include <wchar.h>
namespace up
{
    typedef fpos_t off_t;

#ifdef UP_HAS_STDC_FSEEKO
    inline UPALWAYSINLINE
    int fseeko(FILE* stream, off_t offset, int whence) noexcept {
        return ::_fseeki64(stream, offset, whence);
    }
#endif

#ifdef UP_HAS_STDC_FTELLO
    inline UPALWAYSINLINE
    off_t ftello(FILE* stream) noexcept {
        return ::_ftelli64(stream);
    }
#endif

    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    FILE* fopen(wchar_t const* UPRESTRICT filename, wchar_t const* UPRESTRICT mode) noexcept {
        return ::_wfopen(filename, mode);
    }

    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    FILE* freopen(wchar_t const* UPRESTRICT filename, wchar_t const* UPRESTRICT mode, FILE* UPRESTRICT stream) noexcept {
        return ::_wfreopen(filename, mode, stream);
    }

    inline UPALWAYSINLINE
    int remove(wchar_t const* filename) noexcept {
        return ::_wremove(filename);
    }

    inline UPALWAYSINLINE
    int rename(wchar_t const* old_filename, wchar_t const* new_filename) noexcept {
        return ::_wrename(old_filename, new_filename);
    }

    inline UPALWAYSINLINE
    wchar_t* tmpnam(wchar_t* s) noexcept {
        return ::_wtmpnam(s);
    }
}
#endif

#endif
