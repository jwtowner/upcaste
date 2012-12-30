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

#ifndef UP_TCHAR_HPP
#define UP_TCHAR_HPP

#include <up/prolog.hpp>
#if UP_TCHAR == UP_TCHAR_CHAR
#   include <up/detail/tchar_char.inl>
#elif UP_TCHAR == UP_TCHAR_WCHAR
#   include <up/detail/tchar_wchar.inl>
#endif

namespace up
{
    extern LIBUPCOREAPI UPNONNULL(1,2)
    int ftprintf(FILE* UPRESTRICT stream, tchar_t const* UPRESTRICT format, ...) noexcept;
    
    extern LIBUPCOREAPI UPNONNULL(1,2)
    int ftscanf(FILE* UPRESTRICT stream, tchar_t const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1,3)
    int stprintf(tchar_t* UPRESTRICT s, size_t n, tchar_t const* UPRESTRICT format, ...) noexcept;

    extern LIBUPCOREAPI UPNONNULL(1,2)
    int stscanf(tchar_t const* UPRESTRICT s, tchar_t const* UPRESTRICT format, ...) noexcept;
}

#endif
