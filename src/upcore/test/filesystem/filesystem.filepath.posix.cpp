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
#include <up/cstring.hpp>
#include <up/test.hpp>

namespace filesystem_filepath_posix
{
    UP_TEST_CASE(posix_basename) {
        char* path;

        path = up::filesystem::posix::basename((char const*)nullptr);
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::posix::basename((char const*)nullptr, 12);
        require(!path && (errno == EINVAL));

        path = up::filesystem::posix::basename("");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::posix::basename("/");
        require(path && !up::strcmp(path, ""));
        free(path);

        path = up::filesystem::posix::basename("hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::posix::basename("/hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::posix::basename("hello/");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::posix::basename("/hello///");
        require(path && !up::strcmp(path, "hello"));
        free(path);

        path = up::filesystem::posix::basename("/hello/world.py");
        require(path && !up::strcmp(path, "world.py"));
        free(path);

        path = up::filesystem::posix::basename("/hello//world///");
        require(path && !up::strcmp(path, "world"));
        free(path);

        path = up::filesystem::posix::basename("hello/world/foo\\baz.py");
        require(path && !up::strcmp(path, "foo\\baz.py"));
        free(path);
    }

    UP_TEST_CASE(posix_dirname) {
        char* path;

        path = up::filesystem::posix::dirname((char const*)nullptr);
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::dirname((char const*)nullptr, 12);
        require(!path && (errno == EINVAL));

        path = up::filesystem::posix::dirname("");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::dirname("/");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::posix::dirname("hello");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::dirname("/hello");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::posix::dirname("hello/");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::dirname("/hello///");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::posix::dirname("/hello/world.py");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::posix::dirname("/hello//world///");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::posix::dirname("hello/world/foo\\baz.py");
        require(path && !up::strcmp(path, "hello/world"));
        free(path);
    }

    UP_TEST_CASE(posix_is_absolute_path) {
        bool result;

        require(-1 == up::filesystem::posix::is_absolute_path((char const*)nullptr, &result) && !result);
        require(-1 == up::filesystem::posix::is_absolute_path((char const*)nullptr, 1, &result) && !result);
        require(!up::filesystem::posix::is_absolute_path((char const*)nullptr, 0, &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("\\", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("\\foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("/", &result) && result);
        require(!up::filesystem::posix::is_absolute_path("/foo", &result) && result);
        require(!up::filesystem::posix::is_absolute_path("C:", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("C:foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("C:\\", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("C:\\foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("C:/", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path("C:/foo", &result) && !result);
    }

    UP_TEST_CASE(posix_join_path) {
        char const* const parts1[] = { "hello", "world", "/foo/", "/bar.baz" };
        char const* const parts2[] = { "/hello", "///", "/world", "foo", (char const*)nullptr, "/bar.baz" };
        char const* const parts3[] = { "//", "hello", "world" };
        char buffer[10];
        char* path;
        size_t sz;
        
        path = up::filesystem::posix::join_path(parts1, 4);
        require(path && !up::strcmp(path, "hello/world/foo/bar.baz"));
        free(path);

        path = up::filesystem::posix::join_path(parts2, 6);
        require(path && !up::strcmp(path, "/hello/world/foo/bar.baz"));
        free(path);

        path = up::filesystem::posix::join_path(parts3, 3);
        require(path && !up::strcmp(path, "//hello/world"));
        free(path);

        sz = up::filesystem::posix::join_path(buffer, sizeof(buffer), (char const*)nullptr);
        require(!sz && !up::strcmp(buffer, ""));

        sz = up::filesystem::posix::join_path(buffer, sizeof(buffer), "/hi", "you", (char const*)nullptr, "/bar.baz", (char const*)nullptr);
        require((sz == 7) && !up::strcmp(buffer, "/hi/you"));

        sz = up::filesystem::posix::join_path(buffer, sizeof(buffer), "hello", "world", "/foo/", "/bar.baz", (char const*)nullptr);
        require((sz == 23) && !up::strcmp(buffer, "hello/wor"));

        sz = up::filesystem::posix::join_path(buffer, sizeof(buffer), "///", "hello", (char const*)nullptr);
        require((sz == 8) && !up::strcmp(buffer, "///hello"));

        sz = up::filesystem::posix::join_path(buffer, sizeof(buffer), "/", "world", (char const*)nullptr);
        require((sz == 6) && !up::strcmp(buffer, "/world"));
    }

    UP_TEST_CASE(posix_normalize_path) {
        char* path;
        
        path = up::filesystem::posix::normalize_path((char const*)nullptr);
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::normalize_path("");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::normalize_path(".");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::normalize_path("hello");
        require(path && !up::strcmp(path, "hello"));
        free(path);
        
        path = up::filesystem::posix::normalize_path("/hello");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::posix::normalize_path("//hello");
        require(path && !up::strcmp(path, "//hello"));
        free(path);

        path = up::filesystem::posix::normalize_path("///hello");
        require(path && !up::strcmp(path, "/hello"));
        free(path);

        path = up::filesystem::posix::normalize_path("hello/sussman/../../");
        require(path && !up::strcmp(path, "."));
        free(path);

        path = up::filesystem::posix::normalize_path("hello/sussman/../../..");
        require(path && !up::strcmp(path, ".."));
        free(path);

        path = up::filesystem::posix::normalize_path("/hello/sussman/../../../");
        require(path && !up::strcmp(path, "/"));
        free(path);

        path = up::filesystem::posix::normalize_path(".//hello//sussman//");
        require(path && !up::strcmp(path, "hello/sussman"));
        free(path);

        path = up::filesystem::posix::normalize_path("hello/sussman/sicp.pdf");
        require(path && !up::strcmp(path, "hello/sussman/sicp.pdf"));
        free(path);

        path = up::filesystem::posix::normalize_path("hello/../sussman/sicp.pdf");
        require(path && !up::strcmp(path, "sussman/sicp.pdf"));
        free(path);

        path = up::filesystem::posix::normalize_path("hello/../../sussman/sicp.pdf");
        require(path && !up::strcmp(path, "../sussman/sicp.pdf"));
        free(path);

        path = up::filesystem::posix::normalize_path("/hello/../../sussman/sicp.pdf");
        require(path && !up::strcmp(path, "/sussman/sicp.pdf"));
        free(path);

        path = up::filesystem::posix::normalize_path("./hello//sussman/sicp.pdf");
        require(path && !up::strcmp(path, "hello/sussman/sicp.pdf"));
        free(path);

        path = up::filesystem::posix::normalize_path("//hello/./sussman/..///sicp.pdf");
        require(path && !up::strcmp(path, "//hello/sicp.pdf"));
        free(path);
    }
}
