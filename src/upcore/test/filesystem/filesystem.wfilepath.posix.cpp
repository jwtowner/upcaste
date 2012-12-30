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

#include <up/prolog.hpp>

#ifdef UP_HAS_STDC_WCHAR

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>
#include <up/cwchar.hpp>
#include <up/test.hpp>

namespace filesystem_wfilepath_posix
{
    UP_TEST_CASE(posix_basename) {
        wchar_t* wpath;

        wpath = up::filesystem::posix::basename((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::posix::basename((wchar_t const*)nullptr, 12);
        require(!wpath && (errno == EINVAL));

        wpath = up::filesystem::posix::basename(L"");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"/");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"hello/");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"/hello///");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"/hello/world.py");
        require(wpath && !up::wcscmp(wpath, L"world.py"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"/hello//world///");
        require(wpath && !up::wcscmp(wpath, L"world"));
        free(wpath);

        wpath = up::filesystem::posix::basename(L"hello/world/foo\\baz.py");
        require(wpath && !up::wcscmp(wpath, L"foo\\baz.py"));
        free(wpath);
    }

    UP_TEST_CASE(posix_dirname) {
        wchar_t* wpath;

        wpath = up::filesystem::posix::dirname((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::dirname((wchar_t const*)nullptr, 12);
        require(!wpath && (errno == EINVAL));

        wpath = up::filesystem::posix::dirname(L"");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"/");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"hello");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"hello/");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"/hello///");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"/hello/world.py");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"/hello//world///");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::posix::dirname(L"hello/world/foo\\baz.py");
        require(wpath && !up::wcscmp(wpath, L"hello/world"));
        free(wpath);
    }

    UP_TEST_CASE(posix_is_absolute_path) {
        bool result;
        require(-1 == up::filesystem::posix::is_absolute_path((wchar_t const*)nullptr, &result) && !result);
        require(-1 == up::filesystem::posix::is_absolute_path((wchar_t const*)nullptr, 1, &result) && !result);
        require(!up::filesystem::posix::is_absolute_path((wchar_t const*)nullptr, 0, &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"\\", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"\\foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"/", &result) && result);
        require(!up::filesystem::posix::is_absolute_path(L"/foo", &result) && result);
        require(!up::filesystem::posix::is_absolute_path(L"C:", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"C:foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"C:\\", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"C:\\foo", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"C:/", &result) && !result);
        require(!up::filesystem::posix::is_absolute_path(L"C:/foo", &result) && !result);
    }

    UP_TEST_CASE(posix_join_path) {
        wchar_t const* const wparts1[] = { L"hello", L"world", L"/foo/", L"/bar.baz" };
        wchar_t const* const wparts2[] = { L"/hello", L"///", L"/world", L"foo", nullptr, L"/bar.baz" };
        wchar_t const* const wparts3[] = { L"//", L"hello", L"world" };
        wchar_t wbuffer[10];
        wchar_t* wpath;
        ssize_t sz;

        wpath = up::filesystem::posix::join_path(wparts1, 4);
        require(wpath && !up::wcscmp(wpath, L"hello/world/foo/bar.baz"));
        free(wpath);

        wpath = up::filesystem::posix::join_path(wparts2, 6);
        require(wpath && !up::wcscmp(wpath, L"/hello/world/foo/bar.baz"));
        free(wpath);

        wpath = up::filesystem::posix::join_path(wparts3, 3);
        require(wpath && !up::wcscmp(wpath, L"//hello/world"));
        free(wpath);

        sz = up::filesystem::posix::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), nullptr);
        require(!sz && !up::wcscmp(wbuffer, L""));

        sz = up::filesystem::posix::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), L"/hi", L"you", nullptr, L"/bar.baz", nullptr);
        require((sz == 7) && !up::wcscmp(wbuffer, L"/hi/you"));

        sz = up::filesystem::posix::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), L"hello", L"world", L"/foo/", L"/bar.baz", nullptr);
        require((sz == 23) && !up::wcscmp(wbuffer, L"hello/wor"));

        sz = up::filesystem::posix::join_path(wbuffer, sizeof(wbuffer), L"///", L"hello", nullptr);
        require((sz == 8) && !up::wcscmp(wbuffer, L"///hello"));

        sz = up::filesystem::posix::join_path(wbuffer, sizeof(wbuffer), L"/", L"world", nullptr);
        require((sz == 6) && !up::wcscmp(wbuffer, L"/world"));
    }

    UP_TEST_CASE(posix_normalize_path) {
        wchar_t* wpath;

        wpath = up::filesystem::posix::normalize_path((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L".");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);
        
        wpath = up::filesystem::posix::normalize_path(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"//hello");
        require(wpath && !up::wcscmp(wpath, L"//hello"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"///hello");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello/sussman/../../");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello/sussman/../../..");
        require(wpath && !up::wcscmp(wpath, L".."));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"/hello/sussman/../../../");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L".//hello//sussman//");
        require(wpath && !up::wcscmp(wpath, L"hello/sussman"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello/sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"hello/sussman/sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello/../sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"sussman/sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"hello/../../sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"../sussman/sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"/hello/../../sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"/sussman/sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"./hello//sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"hello/sussman/sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::posix::normalize_path(L"//hello/./sussman/..///sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"//hello/sicp.pdf"));
        free(wpath);
    }
}

#endif
