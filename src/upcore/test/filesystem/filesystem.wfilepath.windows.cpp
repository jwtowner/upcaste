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

namespace filesystem_wfilepath_windows
{
    UP_TEST_CASE(windows_basename) {
        wchar_t* wpath;

        wpath = up::filesystem::windows::basename((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename((wchar_t const*)nullptr, 12);
        require(!wpath && (errno == EINVAL));

        wpath = up::filesystem::windows::basename(L"");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"/");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"\\");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:/");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:\\/\\");
        require(wpath && !up::wcscmp(wpath, L""));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:\\hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"hello/");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"hello\\");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"/hello///");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:\\hello\\\\\\");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"/hello/world.py");
        require(wpath && !up::wcscmp(wpath, L"world.py"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:\\hello\\world.py");
        require(wpath && !up::wcscmp(wpath, L"world.py"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"/hello//world///");
        require(wpath && !up::wcscmp(wpath, L"world"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"c:\\hello\\\\world\\//");
        require(wpath && !up::wcscmp(wpath, L"world"));
        free(wpath);

        wpath = up::filesystem::windows::basename(L"hello/world/foo\\baz.py");
        require(wpath && !up::wcscmp(wpath, L"baz.py"));
        free(wpath);
    }

    UP_TEST_CASE(windows_dirname) {
        wchar_t* wpath;

        wpath = up::filesystem::windows::dirname((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::dirname((wchar_t const*)nullptr, 12);
        require(!wpath && (errno == EINVAL));

        wpath = up::filesystem::windows::dirname(L"");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"/");
        require(wpath && !up::wcscmp(wpath, L"\\"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"\\");
        require(wpath && !up::wcscmp(wpath, L"\\"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"c:/");
        require(wpath && !up::wcscmp(wpath, L"c:/"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"c:\\/\\");
        require(wpath && !up::wcscmp(wpath, L"c:\\"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"hello");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"\\hello");
        require(wpath && !up::wcscmp(wpath, L"\\"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"hello/");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"/hello///");
        require(wpath && !up::wcscmp(wpath, L"/"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"c:\\hello\\\\");
        require(wpath && !up::wcscmp(wpath, L"c:\\"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"/hello/world.py");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"c:\\hello\\world.py");
        require(wpath && !up::wcscmp(wpath, L"c:\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"/hello//world///");
        require(wpath && !up::wcscmp(wpath, L"/hello"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"c:\\hello\\\\world\\//");
        require(wpath && !up::wcscmp(wpath, L"c:\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::dirname(L"hello/world/foo\\baz.py");
        require(wpath && !up::wcscmp(wpath, L"hello/world/foo"));
        free(wpath);
    }

    UP_TEST_CASE(windows_is_absolute_path) {
        bool result;
        require(-1 == up::filesystem::windows::is_absolute_path((wchar_t const*)nullptr, &result) && !result);
        require(-1 == up::filesystem::windows::is_absolute_path((wchar_t const*)nullptr, 1, &result) && !result);
        require(!up::filesystem::windows::is_absolute_path((wchar_t const*)nullptr, 0, &result) && !result);
        require(!up::filesystem::windows::is_absolute_path(L"", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path(L"foo", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path(L"\\", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"\\foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"/", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"/foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"C:", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path(L"C:foo", &result) && !result);
        require(!up::filesystem::windows::is_absolute_path(L"C:\\", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"C:\\foo", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"C:/", &result) && result);
        require(!up::filesystem::windows::is_absolute_path(L"C:/foo", &result) && result);
    }

    UP_TEST_CASE(windows_join_path) {
        wchar_t const* const wparts1[] = { L"hello", L"world", L"/foo\\", L"\\bar.baz" };
        wchar_t const* const wparts2[] = { L"c:\\hello", L"\\/", L"\\world", L"foo", nullptr, L"/bar.baz" };
        wchar_t wbuffer[10];
        wchar_t* wpath;
        ssize_t sz;

        wpath = up::filesystem::windows::join_path(wparts1, 4);
        require(wpath && !up::wcscmp(wpath, L"hello\\world\\foo\\bar.baz"));
        free(wpath);

        wpath = up::filesystem::windows::join_path(wparts2, 6);
        require(wpath && !up::wcscmp(wpath, L"c:\\hello\\world\\foo\\bar.baz"));
        free(wpath);

        sz = up::filesystem::windows::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), nullptr);
        require(!sz && !up::wcscmp(wbuffer, L""));

        sz = up::filesystem::windows::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), L"/hi", L"you", nullptr, L"/bar.baz", nullptr);
        require((sz == 7) && !up::wcscmp(wbuffer, L"/hi\\you"));

        sz = up::filesystem::windows::join_path(wbuffer, sizeof(wbuffer) / sizeof(wchar_t), L"hello", L"world", L"/foo/", L"/bar.baz", nullptr);
        require((sz == 23) && !up::wcscmp(wbuffer, L"hello\\wor"));
    }

    UP_TEST_CASE(windows_normalize_path) {
        wchar_t* wpath;
        
        wpath = up::filesystem::windows::normalize_path((wchar_t const*)nullptr);
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L".");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:");
        require(wpath && !up::wcscmp(wpath, L"C:"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:/");
        require(wpath && !up::wcscmp(wpath, L"C:\\"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello");
        require(wpath && !up::wcscmp(wpath, L"hello"));
        free(wpath);
        
        wpath = up::filesystem::windows::normalize_path(L"/hello");
        require(wpath && !up::wcscmp(wpath, L"\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"//hello");
        require(wpath && !up::wcscmp(wpath, L"\\\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"///hello");
        require(wpath && !up::wcscmp(wpath, L"\\\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:hello");
        require(wpath && !up::wcscmp(wpath, L"C:hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c://hello");
        require(wpath && !up::wcscmp(wpath, L"C:\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:///hello");
        require(wpath && !up::wcscmp(wpath, L"C:\\hello"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello/sussman/../../");
        require(wpath && !up::wcscmp(wpath, L"."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello/sussman/../../..");
        require(wpath && !up::wcscmp(wpath, L".."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"/hello/sussman/../../../");
        require(wpath && !up::wcscmp(wpath, L"\\"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L".//hello//sussman//");
        require(wpath && !up::wcscmp(wpath, L"hello\\sussman"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello\\sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"hello\\sussman\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello/../sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"sussman\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"hello/../../sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"..\\sussman\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"/hello/../../sussman\\sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"\\sussman\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"./hello//sussman/sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"hello\\sussman\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"//hello/./sussman/..///sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"\\\\hello\\sicp.pdf"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:hello/sussman/../../");
        require(wpath && !up::wcscmp(wpath, L"C:"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:\\hello/sussman/../../");
        require(wpath && !up::wcscmp(wpath, L"C:\\"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:hello/sussman/../../..");
        require(wpath && !up::wcscmp(wpath, L"C:.."));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:\\hello/sussman/../../..");
        require(wpath && !up::wcscmp(wpath, L"C:\\"));
        free(wpath);

        wpath = up::filesystem::windows::normalize_path(L"c:\\\\hello/./sussman/..///sicp.pdf");
        require(wpath && !up::wcscmp(wpath, L"C:\\hello\\sicp.pdf"));
        free(wpath);
    }
}

#endif
