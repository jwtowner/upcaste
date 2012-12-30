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

#ifndef UP_CWCTYPE_HPP
#define UP_CWCTYPE_HPP

#include <up/cstddef.hpp>

#ifndef UP_HAS_STDC_WCHAR
#   error "Header file unavailable! Target configuration, compiler, or platform does not support wchar_t!"
#endif

#include <cwctype>

namespace up
{
    using ::std::wctrans_t;
    using ::std::wctype_t;
    using ::std::wint_t;
    using ::std::iswalnum;
    using ::std::iswalpha;
    using ::std::iswcntrl;
    using ::std::iswctype;
    using ::std::iswdigit;
    using ::std::iswlower;
    using ::std::iswgraph;
    using ::std::iswprint;
    using ::std::iswpunct;
    using ::std::iswspace;
    using ::std::iswupper;
    using ::std::iswxdigit;
    using ::std::towctrans;
    using ::std::towlower;
    using ::std::towupper;
    using ::std::wctrans;
    using ::std::wctype;
}

#if defined(UP_HAS_POSIX_LOCALE) && (UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC)
#   include <up/detail/cwctype_msvc.inl>
#elif defined(UP_HAS_POSIX_LOCALE) && (UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_POSIX)
#   include <up/detail/cwctype_posix.inl>
#else
#   error No POSIX locale extensions compatability layer for target platform!
#endif

#endif
