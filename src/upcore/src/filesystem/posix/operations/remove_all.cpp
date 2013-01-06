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

#include "../filesystem_internal.hpp"

namespace up { namespace filesystem { namespace
{
    uintmax_t remove_all_recursive(char const* UPRESTRICT p, bool* UPRESTRICT error) noexcept {
        uintmax_t result = 0;
        struct ::dirent* entry;
        DIR* dir;

        assert(error && !*error);

        dir = ::opendir(p);
        if (!dir) {
            goto handle_error;
        }

        errno = 0;
            
        while ((entry = ::readdir(dir))) {
            if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
                continue;
            }

            if (entry->d_type == DT_DIR) {
                result += remove_all_recursive(entry->d_name, error);
                if (*error) {
                    goto cleanup;
                }
                continue;
            }
                
            if (entry->d_type == DT_UNKNOWN) {
                result += remove_all(entry->d_name);
                if (errno != 0) {
                    goto cleanup;
                }
                continue;
            }

            if (::unlink(entry->d_name) != 0) {
                goto handle_error;
            }

            ++result;
        }

        if ((errno != 0) || (::closedir(dir) != 0)) {
            goto handle_error;
        }
        
        dir = nullptr;

        if (::rmdir(p) != 0) {
            goto handle_error;
        }

        return result + 1;

    handle_error:

        *error = true;

    cleanup:

        if (dir) {
            ::closedir(dir);
        }

        return result;
    }
}}}

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    uintmax_t remove_all(char const* p) noexcept {
        struct ::stat buf;
        if (::lstat(p, &buf) != 0) {
            return 0;
        }

        if (S_ISDIR(buf.st_mode)) {
            bool error = false;
            return remove_all_recursive(p, &error);
        }

        if (::unlink(p) != 0) {
            return 0;
        }

        return 1;
    }
}}
