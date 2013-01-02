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

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>
#include <up/cwchar.hpp>
#include <up/test.hpp>

namespace filesystem_filepath_windows
{
    UP_TEST_CASE(windows_basename) {
        char* path;

        path = up::filesystem::windows::basename((char const*)nullptr);
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename((char const*)nullptr, 12);
        require(!path && (errno == EINVAL));

        path = up::filesystem::windows::basename("");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename("/");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename("\\");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename("c:/");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename("c:\\/\\");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::windows::basename("hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("/hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("c:\\hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("hello/");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("hello\\");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("/hello///");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("c:\\hello\\\\\\");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::windows::basename("/hello/world.py");
        require(path && !up::strcmp(path, "world.py"));
        free(path);

        path = up::filesystem::windows::basename("c:\\hello\\world.py");
        require(path && !up::strcmp(path, "world.py"));
        free(path);

        path = up::filesystem::windows::basename("/hello//world///");
        require(path && !up::strcmp(path, "world"));
        free(path);

        path = up::filesystem::windows::basename("c:\\hello\\\\world\\//");
        require(path && !up::strcmp(path, "world"));
        free(path);

        path = up::filesystem::windows::basename("hello/world/foo\\baz.py");
        require(path && !up::strcmp(path, "baz.py"));
        free(path);
    }

    UP_TEST_CASE(windows_dirname) {
        char* path;

        path = up::filesystem::windows::dirname((char const*)nullptr);
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::dirname((char const*)nullptr, 12);
        require(!path && (errno == EINVAL));

        path = up::filesystem::windows::dirname("");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::dirname("/");
        require(path && !up::strcmp(path, "\\"));
        free(path);

        path = up::filesystem::windows::dirname("\\");
        require(path && !up::strcmp(path, "\\"));
        free(path);

        path = up::filesystem::windows::dirname("c:/");
        require(path && !up::strcmp(path, "c:/"));
        free(path);

        path = up::filesystem::windows::dirname("c:\\/\\");
        require(path && !up::strcmp(path, "c:\\"));
        free(path);

        path = up::filesystem::windows::dirname("hello");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::dirname("/hello");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::windows::dirname("\\hello");
        require(path && !up::strcmp(path, "\\"));
        free(path);

        path = up::filesystem::windows::dirname("hello/");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::dirname("/hello///");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::windows::dirname("c:\\hello\\\\");
        require(path && !up::strcmp(path, "c:\\"));
        free(path);

        path = up::filesystem::windows::dirname("/hello/world.py");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::windows::dirname("c:\\hello\\world.py");
        require(path && !up::strcmp(path, "c:\\hello"));
        free(path);

        path = up::filesystem::windows::dirname("/hello//world///");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::windows::dirname("c:\\hello\\\\world\\//");
        require(path && !up::strcmp(path, "c:\\hello"));
        free(path);

        path = up::filesystem::windows::dirname("hello/world/foo\\baz.py");
        require(path && !up::strcmp(path, "hello/world/foo"));
        free(path);
    }

    UP_TEST_CASE(windows_is_absolute_path) {
        bool result;
        require(-1 == up::filesystem::windows::is_absolute_path((char const*)nullptr, &result) && !result);
        require(-1 == up::filesystem::windows::is_absolute_path((char const*)nullptr, 1, &result) && !result);
        require(!up::filesystem::windows::is_absolute_path((char const*)nullptr, 0, &result) && !result);
        require(!up::filesystem::windows::is_absolute_path("", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path("foo", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path("\\", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("\\foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("/", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("/foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("C:", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path("C:foo", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path("C:\\", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("C:\\foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("C:/", &result) && result);
        require(!up::filesystem::windows::is_absolute_path("C:/foo", &result) && result);
    }

    UP_TEST_CASE(windows_join_path) {
        char const* const parts1[] = { "hello", "world", "/foo\\", "\\bar.baz" };
        char const* const parts2[] = { "c:\\hello", "\\/", "\\world", "foo", nullptr, "/bar.baz" };
        char buffer[10];
        char* path;
        size_t sz;
        
        path = up::filesystem::windows::join_path(parts1, 4);
        require(path && !up::strcmp(path, "hello\\world\\foo\\bar.baz"));
        free(path);

        path = up::filesystem::windows::join_path(parts2, 6);
        require(path && !up::strcmp(path, "c:\\hello\\world\\foo\\bar.baz"));
        free(path);

        sz = up::filesystem::windows::join_path(buffer, sizeof(buffer), nullptr);
        require(!sz && !up::strcmp(buffer, ""));

        sz = up::filesystem::windows::join_path(buffer, sizeof(buffer), "/hi", "you", nullptr, "/bar.baz", nullptr);
        require((sz == 7) && !up::strcmp(buffer, "/hi\\you"));

        sz = up::filesystem::windows::join_path(buffer, sizeof(buffer), "hello", "world", "/foo/", "/bar.baz", nullptr);
        require((sz == 23) && !up::strcmp(buffer, "hello\\wor"));
    }

    UP_TEST_CASE(windows_normalize_path) {
        char* path;
        
        path = up::filesystem::windows::normalize_path((char const*)nullptr);
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::normalize_path("");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::normalize_path(".");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::normalize_path("c:");
        require(path && !up::strcmp(path, "C:"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:/");
        require(path && !up::strcmp(path, "C:\\"));
        free(path);

        path = up::filesystem::windows::normalize_path("hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);
        
        path = up::filesystem::windows::normalize_path("/hello");
        require(path && !up::strcmp(path, "\\hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("//hello");
        require(path && !up::strcmp(path, "\\\\hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("///hello");
        require(path && !up::strcmp(path, "\\\\hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:hello");
        require(path && !up::strcmp(path, "C:hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("c://hello");
        require(path && !up::strcmp(path, "C:\\hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:///hello");
        require(path && !up::strcmp(path, "C:\\hello"));
        free(path);

        path = up::filesystem::windows::normalize_path("hello/sussman/../../");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::windows::normalize_path("hello/sussman/../../..");
        require(path && !up::strcmp(path, ".."));
        free(path);

        path = up::filesystem::windows::normalize_path("/hello/sussman/../../../");
        require(path && !up::strcmp(path, "\\"));
        free(path);

        path = up::filesystem::windows::normalize_path(".//hello//sussman//");
        require(path && !up::strcmp(path, "hello\\sussman"));
        free(path);

        path = up::filesystem::windows::normalize_path("hello\\sussman/sicp.pdf");
        require(path && !up::strcmp(path, "hello\\sussman\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("hello/../sussman/sicp.pdf");
        require(path && !up::strcmp(path, "sussman\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("hello/../../sussman/sicp.pdf");
        require(path && !up::strcmp(path, "..\\sussman\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("/hello/../../sussman\\sicp.pdf");
        require(path && !up::strcmp(path, "\\sussman\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("./hello//sussman/sicp.pdf");
        require(path && !up::strcmp(path, "hello\\sussman\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("//hello/./sussman/..///sicp.pdf");
        require(path && !up::strcmp(path, "\\\\hello\\sicp.pdf"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:hello/sussman/../../");
        require(path && !up::strcmp(path, "C:"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:\\hello/sussman/../../");
        require(path && !up::strcmp(path, "C:\\"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:hello/sussman/../../..");
        require(path && !up::strcmp(path, "C:.."));
        free(path);

        path = up::filesystem::windows::normalize_path("c:\\hello/sussman/../../..");
        require(path && !up::strcmp(path, "C:\\"));
        free(path);

        path = up::filesystem::windows::normalize_path("c:\\\\hello/./sussman/..///sicp.pdf");
        require(path && !up::strcmp(path, "C:\\hello\\sicp.pdf"));
        free(path);
    }
}
