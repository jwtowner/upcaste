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

#include <up/test/test_suite.hpp>
#include <up/test/test_case.hpp>
#include <up/test/test_filter.hpp>
#include <up/test/test_listener.hpp>
#include <up/cstring.hpp>
#include <up/memory.hpp>

#if (UP_COMPILER == UP_COMPILER_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4702)
#endif

#include <map>
#include <memory>

#if (UP_COMPILER == UP_COMPILER_MSVC)
#   pragma warning(pop)
#endif

namespace up { namespace test
{
    namespace
    {
        struct test_object_null_delete
        {
            void operator()(test_object*) const noexcept { }
        };

        struct test_compare
        {
            bool operator()(char const* x, char const* y) const noexcept {
                return strcmp(y, x) < 0;
            }
        };
            
        typedef std::map<char const*, std::shared_ptr<test_object>, test_compare> test_map;
    }

    struct UPHIDDEN test_suite_impl
    {
        test_map tests;
    };

    LIBUPTESTAPI
    test_suite& test_suite::instance() {
        static test_suite instance_;
        return instance_;
    }

    LIBUPTESTAPI
    test_suite::test_suite()
    : test_object(""),
    impl_(malloc_construct<test_suite_impl>()) {
    }
        
    LIBUPTESTAPI
    test_suite::test_suite(char const* name)
    : test_object(name),
    impl_(malloc_construct<test_suite_impl>()) {
    }

    LIBUPTESTAPI
    test_suite::~test_suite() {
        destruct_free(impl_);
    }

    LIBUPTESTAPI
    test_suite* test_suite::find_or_create_fixture(char const* file_name) {
        assert(file_name);

        char const* name_start;
        char const* name_end;
        char name[4096];
            
        name_start = strrchr(file_name, '/');
        if (!name_start) {
            name_start = strrchr(file_name, '\\');
            if (!name_start) {
                name_start = file_name;
            }
            else {
                ++name_start;
            }
        }
        else {
            ++name_start;
        }
            
        name_end = strrchr(file_name, '.');
        if (!name_end) {
            name_end = file_name + strlen(file_name);
        }

        strncpy(name, name_start, name_end - name_start);
        name[name_end - name_start] = 0;

        test_map::iterator it = impl_->tests.find(name);
        if (it == impl_->tests.end()) {
            std::shared_ptr<test_object> new_test_suite = std::make_shared<test_suite>(name);
            it = impl_->tests.insert(std::make_pair(new_test_suite->name(), std::move(new_test_suite))).first;
            it->second->parent_ = this;
        }
        
        return static_cast<test_suite*>(it->second.get());
    }
        
    LIBUPTESTAPI
    void test_suite::add_test(test_object* t, bool should_delete) {
        assert(t && (t->parent_ == nullptr));
            
        std::shared_ptr<test_object> new_test;

        if (should_delete) {
            new_test.reset(t);
        }
        else {
            new_test.reset(t, test_object_null_delete());
        }

        impl_->tests.insert(std::make_pair(new_test->name(), std::move(new_test)));
        t->parent_ = this;
    }

    LIBUPTESTAPI
    void test_suite::remove_test(test_object* t) {
        assert(t && (t->parent_ == this));
            
        test_map::iterator it = impl_->tests.find(t->name());
        if (it != impl_->tests.end()) {
            impl_->tests.erase(it);
        }
    }
                
    LIBUPTESTAPI
    unsigned int test_suite::test_count(test_filter& filter) const {
        unsigned int result = 0;
            
        if (filter.matches(*this)) {
            for (test_map::const_iterator it = impl_->tests.begin(), end = impl_->tests.end(); it != end; ++it) {
                result += it->second->test_count(filter);
            }
        }
            
        return result;
    }

    LIBUPTESTAPI
    test_result const& test_suite::run(test_listener& listener, test_filter& filter) {
        result_.reset();
            
        if (!filter.matches(*this)) {
            return result_;
        }
            
        listener.enter_test_suite(*this);
            
        UPTRY {
            for (test_map::const_iterator it = impl_->tests.begin(), end = impl_->tests.end(); it != end; ++it) {
                result_.accumulate(it->second->run(listener, filter));
            }
        }
        UPCATCHANY_RETHROW(
            listener.exit_test_suite(*this);
        );
            
        listener.exit_test_suite(*this);
        return result_;
    }
}}
