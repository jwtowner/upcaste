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

#include <up/test/fixture_name_filter.hpp>
#include <up/test/test_suite.hpp>
#include <up/cstring.hpp>

namespace up { namespace test
{
    LIBUPTESTAPI
    fixture_name_filter::fixture_name_filter(char const* name)
    : name_(name) {
    }
        
    LIBUPTESTAPI
    bool fixture_name_filter::matches(test_object const& t) {
        if (name_) {
            char const* name = t.name();
            if (!t.parent()) {
                return true;
            }
            if (strstr(name, name_) == name) {
                return true;
            }
                
            test_suite* suite = t.parent();
            while (suite) {
                name = suite->name();
                if (strstr(name, name_) == name) {
                    return true;
                }
                
                suite = suite->parent();
            }
        }
            
        return false;
    }
}}
