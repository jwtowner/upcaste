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

#include <up/prolog.hpp>

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE) && defined(UP_HAS_STDC_WCHAR)

#include <up/filesystem.hpp>

namespace up { namespace filesystem { namespace windows
{
    LIBUPCOREAPI wchar_t const path<wchar_t>::empty[1] = L"";
    LIBUPCOREAPI wchar_t const path<wchar_t>::curdir[2] = L".";
    LIBUPCOREAPI wchar_t const path<wchar_t>::pardir[3] = L"..";
    LIBUPCOREAPI wchar_t const path<wchar_t>::sep[2] = L"\\";
    LIBUPCOREAPI wchar_t const path<wchar_t>::altsep[2] = L"/";
    LIBUPCOREAPI wchar_t const path<wchar_t>::sepset[3] = L"\\/";
    LIBUPCOREAPI wchar_t const path<wchar_t>::extsep[2] = L".";
    LIBUPCOREAPI wchar_t const path<wchar_t>::pathsep[2] = L";";
}}}

#endif
