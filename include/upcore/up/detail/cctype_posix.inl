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

#ifndef UP_DETAIL_CCTYPE_POSIX_INL
#define UP_DETAIL_CCTYPE_POSIX_INL

#ifndef UP_CCTYPE_HPP
#   error "Do not include this header file directly! Instead include <up/cctype.hpp>"
#endif

namespace up
{
    using ::std::isascii;
    using ::std::isblank;
#ifdef UP_HAS_POSIX_LOCALE
    using ::isalnum_l;
    using ::isalpha_l;
    using ::isblank_l;
    using ::iscntrl_l;
    using ::isdigit_l;
    using ::isgraph_l;
    using ::islower_l;
    using ::isprint_l;
    using ::ispunct_l;
    using ::isspace_l;
    using ::isupper_l;
    using ::isxdigit_l;
    using ::tolower_l;
    using ::toupper_l;
#endif
}

#endif
