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

#ifndef UP_DETAIL_CWCTYPE_POSIX_INL
#define UP_DETAIL_CWCTYPE_POSIX_INL

#ifndef UP_CWCTYPE_HPP
#   error "Do not include this header file directly! Instead include <up/cwctype.hpp>"
#endif

#ifdef UP_HAS_POSIX_LOCALE
namespace up
{
    using ::std::iswblank;
    using ::iswalnum_l;
    using ::iswalpha_l;
    using ::iswblank_l;
    using ::iswcntrl_l;
    using ::iswctype_l;
    using ::iswdigit_l;
    using ::iswgraph_l;
    using ::iswlower_l;
    using ::iswprint_l;
    using ::iswpunct_l;
    using ::iswspace_l;
    using ::iswupper_l;
    using ::iswxdigit_l;
    using ::towctrans_l;
    using ::towlower_l;
    using ::towupper_l;
    using ::wctrans_l;
    using ::wctype_l;
}
#endif

#endif
