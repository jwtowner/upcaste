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

#include <up/clocale.hpp>
#include <up/cstring.hpp>
#include <up/test.hpp>

namespace clocale
{
    UP_TEST_CASE(newlocale_all) {
        up::locale_t c_locale = up::newlocale(LC_ALL_MASK, "C", nullptr);
        require(c_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", c_locale));

        up::locale_t posix_locale = up::newlocale(LC_ALL_MASK, "POSIX", nullptr);
        require(posix_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", posix_locale));

        up::locale_t system_locale = up::newlocale(LC_ALL_MASK, "", nullptr);
        require(system_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", system_locale));

        up::freelocale(c_locale);
        up::freelocale(posix_locale);
        up::freelocale(system_locale);
    }

    UP_TEST_CASE(uselocale_all) {
        up::locale_t c_locale = up::newlocale(LC_ALL_MASK, "C", nullptr);
        up::locale_t old_locale = up::uselocale(c_locale);

        require(old_locale);
        require(!up::strcasecmp("HeLLo", "hELlO"));

        up::freelocale(c_locale);
        up::freelocale(old_locale);
    }

    UP_TEST_CASE(newlocale_ctype) {
        up::locale_t c_locale = up::newlocale(LC_CTYPE_MASK, "C", nullptr);
        require(c_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", c_locale));

        up::locale_t posix_locale = up::newlocale(LC_CTYPE_MASK, "POSIX", nullptr);
        require(posix_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", posix_locale));

        up::locale_t system_locale = up::newlocale(LC_CTYPE_MASK, "", nullptr);
        require(system_locale);
        require(!up::strcasecmp_l("HeLLo", "hELlO", system_locale));

        up::freelocale(c_locale);
        up::freelocale(posix_locale);
        up::freelocale(system_locale);
    }
}
