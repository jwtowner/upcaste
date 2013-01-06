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

#ifndef UP_DETAIL_CWCHAR_POSIX_INL
#define UP_DETAIL_CWCHAR_POSIX_INL

#ifndef UP_CWCHAR_HPP
#   error "Do not include this header file directly! Instead include <up/cwchar.hpp>"
#endif

namespace up
{
    using ::wcstok;
#ifdef UP_HAS_POSIX_WMEMCCPY
    using ::wmemccpy;
#endif
#ifdef UP_HAS_POSIX_MBSNRTOWCS
    using ::mbsnrtowcs;
#endif
#ifdef UP_HAS_POSIX_WCSNRTOMBS
    using ::wcsnrtombs;
#endif
#ifdef UP_HAS_POSIX_WCPCPY
    using ::wcpcpy;
#endif
#ifdef UP_HAS_POSIX_WCPNCPY
    using ::wcpncpy;
#endif
#ifdef UP_HAS_POSIX_WCSCASECMP
    using ::wcscasecmp;
#endif
#ifdef UP_HAS_POSIX_WCSNCASECMP
    using ::wcsncasecmp;
#endif
#ifdef UP_HAS_POSIX_WCSDUP
    using ::wcsdup;
#endif
#ifdef UP_HAS_GNU_WCSNDUP
    using ::wcsndup;
#endif
#ifdef UP_HAS_POSIX_WCSNLEN
    using ::wcsnlen;
#endif
#ifdef UP_HAS_POSIX_LOCALE
    using ::wcscasecmp_l;
    using ::wcsncasecmp_l;
    using ::wcscoll_l;
    using ::wcsxfrm_l;
#endif
}

#endif
