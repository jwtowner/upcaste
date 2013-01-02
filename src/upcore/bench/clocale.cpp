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

#include <up/cctype.hpp>
#include <up/cwctype.hpp>
#include <up/clocale.hpp>
#include <up/cstdatomic.hpp>
#include <up/test.hpp>
#include <locale>

namespace clocale
{
    struct fast_isspace
    {
        fast_isspace() {
        }
        
        UPNOINLINE void operator()() const {
            for (int i = 0; i < 1024; ++i) {
                int value = up::fast_isspace(i);
                up::sink_dependency(value);
            }
        }
    };

    struct c_locale_isspace
    {
        c_locale_isspace(char const* name) {
            up::setlocale(LC_CTYPE, name);
        }
        
        UPNOINLINE void operator()() const {
            for (up::wint_t i = 0; i < 1024; ++i) {
                int value = up::iswspace(i);
                up::sink_dependency(value);
            }
        }
    };
    
    struct c_locale_isspace_l
    {
        up::locale_t locale_;
        c_locale_isspace_l(char const* name) { locale_ = up::newlocale(LC_CTYPE_MASK, name, nullptr); }
        c_locale_isspace_l(c_locale_isspace_l const& x) { locale_ = up::duplocale(x.locale_); }
        c_locale_isspace_l& operator=(c_locale_isspace_l const& x) { if (this != &x) { locale_ = up::duplocale(x.locale_); } return *this; }
        ~c_locale_isspace_l() { up::freelocale(locale_); }
        
        UPNOINLINE void operator()() const {
            for (up::wint_t i = 0; i < 1024; ++i) {
                int value = up::iswspace_l(i, locale_);
                up::sink_dependency(value);
            }
        }
    };
    
    struct cxx_locale_isspace
    {
        std::locale locale_;
        cxx_locale_isspace(char const* name) : locale_(name) { }
        
        UPNOINLINE void operator()() const {
            for (int i = 0; i < 1024; ++i) {
                bool value = std::isspace<wchar_t>((wchar_t)i, locale_);
                up::sink_dependency(value);
            }
        }
    };

    UP_TEST_BENCHMARK(isspace) {
        unsigned int const iterations = 100000;

        fast_isspace z;
        UP_TEST_RUN_BENCHMARK("fast_isspace", 1, iterations, z);

        c_locale_isspace a("C");
        UP_TEST_RUN_BENCHMARK("isspace - 'C'", 1, iterations, a);
        
        c_locale_isspace_l b("C");
        UP_TEST_RUN_BENCHMARK("isspace_l - 'C'", 1, iterations, b);
    
        cxx_locale_isspace c("C");
        UP_TEST_RUN_BENCHMARK("std::isspace - 'C'", 1, iterations, c);

#if UP_BASESYSTEM != UP_BASESYSTEM_WINDOWS

        c_locale_isspace a2("en_US.utf8");
        UP_TEST_RUN_BENCHMARK("isspace - 'UTF-8'", 1, iterations, a2);
        
        c_locale_l_isspace b2("en_US.utf8");
        UP_TEST_RUN_BENCHMARK("isspace_l - 'UTF-8'", 1, iterations, b2);
    
        cxx_locale_isspace c2("en_US.utf8");
        UP_TEST_RUN_BENCHMARK("std::isspace - 'UTF-8'", 1, iterations, c2);

#endif
    }
}
