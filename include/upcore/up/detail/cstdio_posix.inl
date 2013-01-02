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

#ifndef UP_DETAIL_CSTDIO_POSIX_INL
#define UP_DETAIL_CSTDIO_POSIX_INL

#ifndef UP_CSTDIO_HPP
#   error "Do not include this header file directly! Instead include <up/cstdio.hpp>"
#endif

namespace up
{
#ifdef UP_HAS_POSIX_FSEEKO
    using ::fseeko;
#endif

#ifdef UP_HAS_POSIX_FTELLO
    using ::ftello;
#endif

#ifdef UP_HAS_POSIX_GETDELIM
    using ::getdelim;
#endif

#ifdef UP_HAS_POSIX_GETDELIM
    using ::getline;
#endif

#ifdef UP_HAS_STDC_WCHAR
    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    FILE* fopen(wchar_t const* UPRESTRICT filename, wchar_t const* UPRESTRICT mode) noexcept;

    extern LIBUPCOREAPI UPALLOC UPWARNRESULT
    FILE* freopen(wchar_t const* UPRESTRICT filename, wchar_t const* UPRESTRICT mode, FILE* UPRESTRICT stream) noexcept;

    extern LIBUPCOREAPI
    int remove(wchar_t const* filename) noexcept;

    extern LIBUPCOREAPI
    int rename(wchar_t const* old_filename, wchar_t const* new_filename);

    extern LIBUPCOREAPI
    wchar_t* tmpnam(wchar_t* s) noexcept;
#endif
}

#endif
