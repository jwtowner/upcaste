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

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>
#include <up/cstring.hpp>
#include <up/test.hpp>

namespace filesystem_operations
{
    UP_TEST_CASE(current_directory_path) {
        bool result;
        char* path = up::filesystem::current_directory_path<char>();
        require(path && (up::strlen(path) > 0));
        require(!up::filesystem::is_absolute_path(path, &result) && result);
        require(!up::filesystem::exists(path, &result) && result);
        free(path);
    }

    UP_TEST_CASE(temp_directory_path) {
        bool result;
        char* path = up::filesystem::temp_directory_path<char>();
        require(path && (up::strlen(path) > 0));
        require(!up::filesystem::is_absolute_path(path, &result) && result);
        require(!up::filesystem::exists(path, &result) && result);
        free(path);
    }

    UP_TEST_CASE(user_directory_path) {
        bool result;
        char* path = up::filesystem::user_directory_path<char>();
        require(path && (up::strlen(path) > 0));
        require(!up::filesystem::is_absolute_path(path, &result) && result);
        require(!up::filesystem::exists(path, &result) && result);
        free(path);
    }

    UP_TEST_CASE(absolute_path) {
        char* path;
        
#if UP_FILESYSTEM == UP_FILESYSTEM_POSIX

        path = up::filesystem::absolute_path("/hello//world");
        require(path && !up::strcmp(path, "/hello/world"));
        free(path);

        path = up::filesystem::absolute_path("//hello//world");
        require(path && !up::strcmp(path, "//hello/world"));
        free(path);

#elif UP_FILESYSTEM == UP_FILESYSTEM_WINDOWS

        path = up::filesystem::absolute_path("/hello/world");
        require(path && !up::strcmp(path, "\\hello\\world"));
        free(path);

        path = up::filesystem::absolute_path("C:\\hello\\world");
        require(path && !up::strcmp(path, "C:\\hello\\world"));
        free(path);

#endif
    }
}
