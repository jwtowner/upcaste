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

#include "../filesystem_internal.hpp"

namespace up { namespace filesystem { namespace detail
{
    namespace
    {
        constexpr size_t temp_directory_envvar_count = 4;
        char const* const temp_directory_envvar[temp_directory_envvar_count] = { "TMPDIR", "TEMPDIR", "TMP", "TEMP" };
        char const* const default_temp_directory_path = "/tmp";
    }

    LIBUPCOREAPI UPALLOC UPWARNRESULT
    char* observer_operations<char>::temp_directory_path() noexcept {
        char const* path = nullptr;

        for (size_t i = 0; (i < temp_directory_envvar_count) && path; ++i) {
            path = getenv(temp_directory_envvar[i]);
        }

        if (!path) {
            path = default_temp_directory_path;
        }

        size_t const path_length = strlen(path);
        char* result = static_cast<char*>(malloc(path_length + 1));
        if (!result) {
            return nullptr;
        }

        memcpy(result, path, path_length + 1);
        return result;
    }
}}}
