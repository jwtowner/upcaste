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

#include <up/prolog.hpp>

#ifdef UP_HAS_STDC_WCHAR

#include <up/filesystem.hpp>
#include <up/cerrno.hpp>
#include <up/cwchar.hpp>
#include <up/test.hpp>

namespace filesystem_woperations
{
    UP_TEST_CASE(current_directory_path) {
        bool result;
        wchar_t* path = up::filesystem::current_directory_path<wchar_t>();
        require(path && (up::wcslen(path) > 0));
        require(!up::filesystem::is_absolute_path(path, &result) && result);
        require(!up::filesystem::exists(path, &result) && result);
        free(path);
    }

    UP_TEST_CASE(temp_directory_path) {
        bool result;
        wchar_t* wpath = up::filesystem::temp_directory_path<wchar_t>();
        require(wpath && (up::wcslen(wpath) > 0));
        require(!up::filesystem::is_absolute_path(wpath, &result) && result);
        require(!up::filesystem::exists(wpath, &result) && result);
        free(wpath);
    }

    UP_TEST_CASE(user_directory_path) {
        bool result;
        wchar_t* wpath = up::filesystem::user_directory_path<wchar_t>();
        require(wpath && (up::wcslen(wpath) > 0));
        require(!up::filesystem::is_absolute_path(wpath, &result) && result);
        require(!up::filesystem::exists(wpath, &result) && result);
        free(wpath);
    }

    UP_TEST_CASE(absolute_path) {
        wchar_t* wpath;
        
#if UP_FILESYSTEM == UP_FILESYSTEM_POSIX

        wpath = up::filesystem::absolute_path(L"/hello//world");
        require(path && !up::wcscmp(wpath, L"/hello/world"));
        free(wpath);

        wpath = up::filesystem::absolute_path(L"//hello//world");
        require(wpath && !up::wcscmp(wpath, L"//hello/world"));
        free(wpath);

#elif UP_FILESYSTEM == UP_FILESYSTEM_WINDOWS

        wpath = up::filesystem::absolute_path(L"/hello/world");
        require(wpath && !up::wcscmp(wpath, L"\\hello\\world"));
        free(wpath);

        wpath = up::filesystem::absolute_path(L"C:\\hello\\world");
        require(wpath && !up::wcscmp(wpath, L"C:\\hello\\world"));
        free(wpath);

#endif
    }
}

#endif
