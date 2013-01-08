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

#include <up/test/console/console_test_listener.hpp>
#include <up/test/default_filter.hpp>
#include <up/test/test_benchmark_result.hpp>
#include <up/test/test_case.hpp>
#include <up/test/test_error.hpp>
#include <up/test/test_suite.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/memory.hpp>
#include <string>

namespace up { namespace test
{
    namespace
    {
        char const* const section_separator = "==========================================================================";
        char const* const group_separator   = "--------------------------------------------------------------------------";
    }

    struct UPHIDDEN console_test_listener_impl
    {
        std::string log_buffer;
    };
        
    LIBUPTESTAPI
    console_test_listener::console_test_listener()
    : error_buffer_(static_cast<char*>(malloc(16384))),
    error_buffer_size_(16384),
    error_buffer_index_(0),
    impl_(malloc_construct<console_test_listener_impl>()) {
        sink_ = add_log_sink(&logger_callback, this);
    }
        
    LIBUPTESTAPI
    console_test_listener::~console_test_listener() {
        remove_log_sink(sink_);
        destruct_free(impl_);
        free(error_buffer_);
    }

    LIBUPTESTAPI
    void console_test_listener::started(test_suite const&, unsigned int total_tests, unsigned int total_tests_to_run) {
        std::string empty;
        impl_->log_buffer.swap(empty);
        error_buffer_index_ = 0;
        error_buffer_[0] = '\0';
    
        printf("Running %d out of %d test cases ...\n", total_tests_to_run, total_tests);
        printf("%s\n\n", section_separator);
    }
        
    LIBUPTESTAPI
    void console_test_listener::stopped(test_suite const&, test_result const& result) {
        if (result.failed()) {
            printf("\n%s\n\n%s", section_separator, error_buffer_);
        }
            
        printf("\n%s\n", section_separator);
        printf("        Total Tests: %5d\n", result.test_count());
        printf("    Total Tests Run: %5d\n", result.tests_run());
        printf("Total Ignored Tests: %5d\n", result.tests_ignored());
        printf(" Total Failed Tests: %5d\n", result.tests_failed());
        printf(" Total Passed Tests: %5d\n", result.tests_passed());
    }
        
    LIBUPTESTAPI
    void console_test_listener::enter_test_suite(test_suite const& suite) {
        if (suite.parent() != nullptr) {
            printf("%s\n", suite.name());
            printf("%s\n", group_separator);
        }
    }
        
    LIBUPTESTAPI
    void console_test_listener::exit_test_suite(test_suite const& suite) {
        if (suite.parent() != nullptr) {
            putchar('\n');
        }
    }
        
    LIBUPTESTAPI
    void console_test_listener::test_case_passed(test_case const& t) {
        printf("  %s... SUCCESS\n", t.name());
            
        if (!impl_->log_buffer.empty()) {
            printf("\n    Log:\n--------\n%s\n--------\n\n", impl_->log_buffer.c_str());
        }

        std::string empty;
        impl_->log_buffer.swap(empty);
    }
        
    LIBUPTESTAPI
    void console_test_listener::test_case_ignored(test_case const& t) {
        printf("  %s... IGNORED\n", t.name());
    }
        
    LIBUPTESTAPI
    void console_test_listener::test_case_failed(test_case const& t, test_error const& error) {
        printf("  %s... FAILURE\n", t.name());
            
        if (!impl_->log_buffer.empty()) {
            printf("\n    Log:\n--------\n%s\n--------\n\n", impl_->log_buffer.c_str());
        }

        std::string empty;
        impl_->log_buffer.swap(empty);

        if (error_buffer_index_ > 0) {
            error_printf("%s\n\n", group_separator);
        }
            
        if (strcmp(t.file_name(), error.file_name()) == 0) { 
            error_printf("Failed test case '%s' [%s(%Ld)]:\n\n", t.name(), error.file_name(), error.line_number());
        }
        else {
            error_printf("Error in test case '%s' [%s(%Ld)]:\n\n", t.name(), error.file_name(), error.line_number());
        }
            
        error_printf("Type: %s\n", error.what());
            
        if (error.condition() != 0) {
            error_printf("Condition: %s\n", error.condition());
        }
            
        if (error.message() != 0) {
            error_printf("Message: %s\n", error.message());
        }
            
        error_printf("\n");
    }
        
    LIBUPTESTAPI
    void console_test_listener::benchmark_results(
        test_case const&,
        test_benchmark_result const* first,
        test_benchmark_result const* last
    ) {
        char number_buffer[32];
        int max_category_length = 8;
        int max_complexity_length = 1;
            
        for (test_benchmark_result const* result = first; result != last; ++result) {
            int length = static_cast<int>(strlen(result->category()));
            if (length > max_category_length) {
                max_category_length = length;
            }
                
            length = snprintf(number_buffer, 31, "%u", result->complexity());
            if (length > max_complexity_length) {
                max_complexity_length = length;
            }
        }
            
        putchar('\n');
            
        for (test_benchmark_result const* result = first; result != last; ++result) {
            printf(
                "    %*s : %*u : %.9fs [%.6Es/%u]\n",
                max_category_length,
                result->category(),
                max_complexity_length,
                result->complexity(),
                result->average_time(),
                result->total_time(),
                result->iterations()
            );
        }
            
        putchar('\n');
    }

    UPHIDDEN
    void UPCDECL console_test_listener::logger_callback(log_record const* record, void* user_data) {
        console_test_listener* listener = static_cast<console_test_listener*>(user_data);
        listener->impl_->log_buffer.append(record->message);
    }

    UPHIDDEN
    void console_test_listener::error_printf(char const* format, ...) {
        char error_message[1024];
        int error_message_size;
        char* new_buffer;
        va_list args;

        va_start(args, format);
        error_message_size = vsnprintf(error_message, sizeof(error_message), format, args);
        va_end(args);

        if (error_message_size >= static_cast<int>(error_buffer_size_ - error_buffer_index_)) {
            error_buffer_size_ = error_buffer_size_ * 2;
            new_buffer = static_cast<char*>(realloc(error_buffer_, error_buffer_size_));
            if (!new_buffer) {
                return;
            }

            error_buffer_ = new_buffer;
        }

        strcpy(error_buffer_ + error_buffer_index_, error_message);
        error_buffer_index_ += error_message_size;
    }
}}
