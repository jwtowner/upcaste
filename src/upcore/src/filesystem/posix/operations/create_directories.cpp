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

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    int create_directories(char const* p) noexcept {
        size_t length;
        char* dirpath, * dirpath_end;
        struct ::stat stat_buf;
        int result = 0;

        if (!p) {
            errno = EINVAL;
            return -1;
        }

        // create local copy of directory path that we can manipulate
        length = strlen(p);
        dirpath = static_cast<char*>(malloc(length + 1));
        if (!dirpath) {
            return -1;
        }

        dirpath_end = static_cast<char*>(memcpy(dirpath, p, length + 1));

        // create parent directories or ensure they exist
        while ((dirpath_end = strchr(dirpath_end, '/'))) {
            *dirpath_end = '\0';

            if ((::mkdir(dirpath, S_IRWXU | S_IRWXG | S_IRWXO) != 0) && (errno != EEXIST)) {
                result = -1;
                goto done;
            }

            *dirpath_end = '/';
        }

        // create final leaf directory or ensure it exists as a directory
        if (::mkdir(dirpath, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            if (errno != EEXIST) {
                result = -1;
                goto done;
            }

            if (::stat(dirpath, &stat_buf) != 0) {
                result = -1;
                goto done;
            }

            if (!S_ISDIR(stat_buf.st_mode)) {
                errno = ENOTDIR;
                result = -1;
                goto done;
            }
        }

    done:

        free(dirpath);
        return result;
    }
}}
