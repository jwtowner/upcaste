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

#ifndef UP_DETAIL_CCTYPE_MSVC_INL
#define UP_DETAIL_CCTYPE_MSVC_INL

#ifndef UP_CCTYPE_HPP
#   error "Do not include this header file directly! Instead include <up/cctype.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE int isblank(int c) noexcept { return _isctype(c, _BLANK); }
#ifdef UP_HAS_POSIX_LOCALE
    inline UPALWAYSINLINE UPPURE int isalnum_l(int c, _locale_t l) noexcept { return _isalnum_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isalpha_l(int c, _locale_t l) noexcept { return _isalpha_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isblank_l(int c, _locale_t l) noexcept { return _isctype_l(c, _BLANK, l); }
    inline UPALWAYSINLINE UPPURE int iscntrl_l(int c, _locale_t l) noexcept { return _iscntrl_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isdigit_l(int c, _locale_t l) noexcept { return _isdigit_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isgraph_l(int c, _locale_t l) noexcept { return _isgraph_l(c, l); };
    inline UPALWAYSINLINE UPPURE int islower_l(int c, _locale_t l) noexcept { return _islower_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isprint_l(int c, _locale_t l) noexcept { return _isprint_l(c, l); };
    inline UPALWAYSINLINE UPPURE int ispunct_l(int c, _locale_t l) noexcept { return _ispunct_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isspace_l(int c, _locale_t l) noexcept { return _isspace_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isupper_l(int c, _locale_t l) noexcept { return _isupper_l(c, l); };
    inline UPALWAYSINLINE UPPURE int isxdigit_l(int c, _locale_t l) noexcept { return _isxdigit_l(c, l); };
    inline UPALWAYSINLINE UPPURE int tolower_l(int c, _locale_t l) noexcept { return _tolower_l(c, l); };
    inline UPALWAYSINLINE UPPURE int toupper_l(int c, _locale_t l) noexcept { return _toupper_l(c, l); };
#endif    
}

#endif
