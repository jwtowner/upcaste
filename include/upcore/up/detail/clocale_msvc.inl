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

#ifndef UP_DETAIL_CLOCALE_MSVC_INL
#define UP_DETAIL_CLOCALE_MSVC_INL

#ifndef UP_CLOCALE_HPP
#   error "Do not include this header file directly! Instead include <up/clocale.hpp>"
#endif

#define LC_GLOBAL_LOCALE    ((::up::locale_t)-1)
#define LC_COLLATE_MASK     0x01
#define LC_CTYPE_MASK       0x02
#define LC_MONETARY_MASK    0x04
#define LC_NUMERIC_MASK     0x08
#define LC_TIME_MASK        0x10
#define LC_MESSAGES_MASK    0x20
#define LC_ALL_MASK         (LC_COLLATE_MASK | LC_CTYPE_MASK | LC_MONETARY_MASK | LC_NUMERIC_MASK | LC_TIME_MASK | LC_MESSAGES_MASK)

namespace up
{
    typedef ::_locale_t locale_t;
    
    extern LIBUPCOREAPI
    char* setlocale(int category, char const* locale) noexcept;
    
    extern LIBUPCOREAPI UPWARNRESULT
    locale_t newlocale(int category, char const* locale, locale_t base) noexcept;
    
    extern LIBUPCOREAPI UPWARNRESULT
    locale_t duplocale(locale_t locobj) noexcept;
    
    extern LIBUPCOREAPI UPWARNRESULT
    locale_t uselocale(locale_t locobj) noexcept;
    
    extern LIBUPCOREAPI
    void freelocale(locale_t locobj) noexcept;

    extern LIBUPCOREAPI
    struct lconv* localeconv_l(locale_t locale) noexcept;
}

#endif
