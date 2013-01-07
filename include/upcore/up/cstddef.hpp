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

#ifndef UP_CSTDDEF_HPP
#define UP_CSTDDEF_HPP

#include <up/prolog.hpp>
#include <stddef.h>

#ifdef UP_HAS_POSIX_SSIZE
#   include <sys/types.h>
#elif UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
    extern "C" { typedef ptrdiff_t ssize_t; }
#else
    extern "C" { typedef long ssize_t; }
#endif

namespace up
{
    using ::ptrdiff_t;
    using ::size_t;
    using ::ssize_t;

#ifndef UP_HAS_STDC_MAX_ALIGN
    union LIBUPCOREAPI max_align_t
    {
        int (max_align_t::*mfp)(int);
        int max_align_t::*mp;
        int (*fp)(int);
        void* p;
        ptrdiff_t pt;
        size_t st;
        long double ld;
        double d;
        float f;
        int i;
        long l;
        long long ll;
        short s;
        bool b;
        char c;
    };
#else
    using ::max_align_t;
#endif

#ifndef UP_NO_NULLPTR
    typedef decltype(nullptr) nullptr_t;
#else
    class LIBUPCOREAPI nullptr_t
    {
        void* p;
        void operator&() const;
    public:
        UPALWAYSINLINE nullptr_t(void* nullptr_t::*) { }
        UPALWAYSINLINE operator void* nullptr_t::*() { return 0; }
        template <class T> UPALWAYSINLINE operator T*() const { return 0; }
        template <class C, class T> UPALWAYSINLINE operator T C::*() const { return 0; }
    };

    inline UPALWAYSINLINE bool operator==(nullptr_t, nullptr_t) { return true; }
    inline UPALWAYSINLINE bool operator!=(nullptr_t, nullptr_t) { return false; }
    inline UPALWAYSINLINE bool operator<(nullptr_t, nullptr_t) { return false; }
    inline UPALWAYSINLINE bool operator<=(nullptr_t, nullptr_t) { return true; }
    inline UPALWAYSINLINE bool operator>(nullptr_t, nullptr_t) { return false; }
    inline UPALWAYSINLINE bool operator>=(nullptr_t, nullptr_t) { return true; }

#   define nullptr ::up::nullptr_t(0)
#endif

#if defined(UP_HAS_STDC_WCHAR) && defined(UP_NO_NATIVE_WCHAR_T)
    using ::wchar_t;
#endif
}

#endif
