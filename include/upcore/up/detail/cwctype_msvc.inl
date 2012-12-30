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

#ifndef UP_DETAIL_CWCTYPE_MSVC_INL
#define UP_DETAIL_CWCTYPE_MSVC_INL

#ifndef UP_CWCTYPE_HPP
#   error "Do not include this header file directly! Instead include <up/cwctype.hpp>"
#endif

namespace up
{
    inline UPALWAYSINLINE UPPURE int iswblank(wint_t wc) noexcept { return ::iswctype(wc, _BLANK); }
}

#ifdef UP_HAS_STDC_LOCALE

namespace up { namespace detail
{
    extern "C" UPIMPORT int UPCDECL _iswalpha_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswupper_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswlower_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswdigit_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswxdigit_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswspace_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswpunct_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswalnum_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswprint_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswgraph_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswcntrl_l(wint_t, _locale_t);
    extern "C" UPIMPORT wint_t UPCDECL _towupper_l(wint_t, _locale_t);
    extern "C" UPIMPORT wint_t UPCDECL _towlower_l(wint_t, _locale_t);
    extern "C" UPIMPORT int UPCDECL _iswctype_l(wint_t, wctype_t, _locale_t);
}}

namespace up
{
    inline UPALWAYSINLINE UPPURE int iswalnum_l(wint_t wc, _locale_t l) noexcept { return detail::_iswalnum_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswalpha_l(wint_t wc, _locale_t l) noexcept { return detail::_iswalpha_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswblank_l(wint_t wc, _locale_t l) noexcept { return detail::_iswctype_l(wc, _BLANK, l); }
    inline UPALWAYSINLINE UPPURE int iswcntrl_l(wint_t wc, _locale_t l) noexcept { return detail::_iswcntrl_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswdigit_l(wint_t wc, _locale_t l) noexcept { return detail::_iswdigit_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswgraph_l(wint_t wc, _locale_t l) noexcept { return detail::_iswgraph_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswlower_l(wint_t wc, _locale_t l) noexcept { return detail::_iswlower_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswprint_l(wint_t wc, _locale_t l) noexcept { return detail::_iswprint_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswpunct_l(wint_t wc, _locale_t l) noexcept { return detail::_iswpunct_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswspace_l(wint_t wc, _locale_t l) noexcept { return detail::_iswspace_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswupper_l(wint_t wc, _locale_t l) noexcept { return detail::_iswupper_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswxdigit_l(wint_t wc, _locale_t l) noexcept { return detail::_iswxdigit_l(wc, l); };
    inline UPALWAYSINLINE UPPURE wint_t towlower_l(wint_t wc, _locale_t l) noexcept { return detail::_towlower_l(wc, l); };
    inline UPALWAYSINLINE UPPURE wint_t towupper_l(wint_t wc, _locale_t l) noexcept { return detail::_towupper_l(wc, l); };
    inline UPALWAYSINLINE UPPURE int iswctype_l(wint_t wc, wctype_t charclass, _locale_t l) noexcept { return detail::_iswctype_l(wc, charclass, l); }
    inline UPALWAYSINLINE UPPURE wint_t towctrans_l(wint_t wc, wctrans_t desc, _locale_t) noexcept { return ::towctrans(wc, desc); }
    inline UPALWAYSINLINE UPPURE wctrans_t wctrans_l(char const* charclass, _locale_t) noexcept { return ::wctrans(charclass); }
    inline UPALWAYSINLINE UPPURE wctype_t wctype_l(char const* prop, _locale_t) noexcept { return ::wctype(prop); }
}

#endif

#endif
