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
    UPHIDDEN
    void get_status_info(struct ::stat const*  UPRESTRICT buf, status_info* UPRESTRICT info) noexcept {
        info->type = get_status_type(buf->st_mode);
        info->file_size = static_cast<uintmax_t>(buf->st_size);
        info->hard_link_count = static_cast<uintmax_t>(buf->st_nlink);
        info->last_access_time.tv_sec = buf->st_atime;
        info->last_access_time.tv_nsec = 0;
        info->last_write_time.tv_sec = buf->st_mtime;
        info->last_write_time.tv_nsec = 0;
    }
}}}
