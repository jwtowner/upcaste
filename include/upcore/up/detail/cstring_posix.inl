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

#ifndef UP_DETAIL_CSTRING_POSIX_INL
#define UP_DETAIL_CSTRING_POSIX_INL

#ifndef UP_CSTRING_HPP
#   error "Do not include this header file directly! Instead include <up/cstring.hpp>"
#endif

namespace up
{
#ifdef UP_HAS_POSIX_MEMCCPY
    using ::memccpy;
#endif
#ifdef UP_HAS_POSIX_STPCPY
    using ::stpcpy;
#endif
#ifdef UP_HAS_POSIX_STPNCPY
    using ::stpncpy;
#endif
#ifdef UP_HAS_POSIX_STRCASECMP
    using ::strcasecmp;
#endif
#ifdef UP_HAS_POSIX_STRNCASECMP
    using ::strncasecmp;
#endif
#ifdef UP_HAS_POSIX_STRDUP
    using ::strdup;
#endif
#ifdef UP_HAS_GNU_STRNDUP
    using ::strndup;
#endif
#ifdef UP_HAS_POSIX_STRNLEN
    using ::strnlen;
#endif
#ifdef UP_HAS_POSIX_STRTOK_R
    using ::strtok_r;
#endif
#ifdef UP_HAS_POSIX_LOCALE
    using ::strcasecmp_l;
    using ::strncasecmp_l;
    using ::strcoll_l;
    using ::strxfrm_l;
#endif
}

#endif
