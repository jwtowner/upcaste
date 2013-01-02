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

#if (UP_FILESYSTEM != UP_FILESYSTEM_NONE)

#include <up/filesystem.hpp>

namespace up { namespace filesystem { namespace windows
{
    LIBUPCOREAPI char const path<char>::empty[1] = "";
    LIBUPCOREAPI char const path<char>::curdir[2] = ".";
    LIBUPCOREAPI char const path<char>::pardir[3] = "..";
    LIBUPCOREAPI char const path<char>::sep[2] = "\\";
    LIBUPCOREAPI char const path<char>::altsep[2] = "/";
    LIBUPCOREAPI char const path<char>::sepset[3] = "\\/";
    LIBUPCOREAPI char const path<char>::extsep[2] = ".";
    LIBUPCOREAPI char const path<char>::pathsep[2] = ";";
}}}

#endif
