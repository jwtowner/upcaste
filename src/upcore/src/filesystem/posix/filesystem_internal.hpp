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

#ifndef UP_FILESYSTEM_POSIX_FILESYSTEM_INTERNAL_HPP
#define UP_FILESYSTEM_POSIX_FILESYSTEM_INTERNAL_HPP

#include <up/filesystem.hpp>
#include <up/cassert.hpp>
#include <up/cerrno.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cthreads.hpp>
#include <up/cuchar.hpp>
#ifdef UP_HAS_STDC_WCHAR
#   include <up/cwchar.hpp>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>

namespace up { namespace filesystem { namespace detail
{
    extern UPHIDDEN once_flag transcode_locale_guard;
    extern UPHIDDEN locale_t transcode_locale;
    extern UPHIDDEN bool transcode_locale_is_utf8;

    extern UPHIDDEN
    void UPCDECL transcode_locale_init() noexcept;

    extern UPHIDDEN
    void get_space_info(struct ::statvfs const& buf, space_info& info) noexcept;

    extern UPHIDDEN
    void get_status_info(struct ::stat const& buf, status_info& info) noexcept;

    extern UPHIDDEN UPPURE
    status_type get_status_type(::mode_t mode) noexcept;
}}}

#endif
