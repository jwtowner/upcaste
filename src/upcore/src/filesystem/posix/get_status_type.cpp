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

#include "filesystem_internal.hpp"

namespace up { namespace filesystem { namespace detail
{
    UPHIDDEN UPPURE
    status_type get_status_type(::mode_t mode) noexcept {
        if (S_ISREG(mode)) {
            return status_regular;
        }
        else if (S_ISDIR(mode)) {
            return status_directory;
        }
        else if (S_ISLNK(mode)) {
            return status_symlink;
        }
        else if (S_ISBLK(mode)) {
            return status_block;
        }
        else if (S_ISCHR(mode)) {
            return status_character;
        }
        else if (S_ISFIFO(mode)) {
            return status_fifo;
        }
        else if (S_ISSOCK(mode)) {
            return status_socket;
        }
        else {
            return status_unknown;
        }
    }
}}}
