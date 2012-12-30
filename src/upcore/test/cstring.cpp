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

#include <up/cstring.hpp>
#include <up/test.hpp>

namespace cstring
{
    UP_TEST_CASE(strcasecmp) {
        require(!up::strcasecmp("", ""));
        require(!up::strcasecmp("awesome", "awesome"));
        require(!up::strcasecmp("awesome", "AWESOME"));
        require(!up::strcasecmp("aWeSoME", "AwEsOme"));
        require(0 != up::strcasecmp("@aWeSoME[", "`AwEsOme{"));
    }

    UP_TEST_CASE(fast_strcasecmp) {
        require(!up::fast_strcasecmp("", ""));
        require(!up::fast_strcasecmp("awesome", "awesome"));
        require(!up::fast_strcasecmp("awesome", "AWESOME"));
        require(!up::fast_strcasecmp("aWeSoME", "AwEsOme"));
        require(0 != up::fast_strcasecmp("@aWeSoME[", "`AwEsOme{"));
    }
}
