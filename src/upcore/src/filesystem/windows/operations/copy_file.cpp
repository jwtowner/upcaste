/*//
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

#if UP_BASESYSTEM == UP_BASESYSTEM_DARWIN
#   include <copyfile.h>
#endif

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPNONNULLALL
    int copy_file(char const* from, char const* to, copy_option option) noexcept {
#if UP_BASESYSTEM == UP_BASESYSTEM_DARWIN
        ::copyfile_state_t state = ::copyfile_state_alloc();
        if (!state) {
            return false;
        }

        ::copyfile_flags_t flags = COPYFILE_ALL;                     
        if (option == copy_option::overwrite) {
            flags |= COPYFILE_UNLINK;
        }
        else {
            flags |= COPYFILE_EXCL;
        }

        int result = ::copyfile(from, to, state, flags);
        if (result != 0) {
            ::copyfile_state_free(state);
            return false;
        }
        
        ec.clear();
        ::copyfile_state_free(state);
        return true;
#else
        constexpr size_t max_buffer_size = 1048576;
        std::pair<unsigned char*, ptrdiff_t> buffer(nullptr, 0);
        int from_fd = -1, to_fd = -1;
        struct ::stat from_attributes;
        ssize_t bytes_read, bytes_written;

        from_fd = ::open(from, O_RDONLY, 0);
        if (from_fd == -1) {
            goto handle_error;
        }

        if (::fstat(from_fd, &from_attributes) == -1) {
            goto handle_error;
        }

        to_fd = ::open(to, O_CREAT | O_WRONLY | O_CREAT | ((option == copy_option::overwrite) ? O_EXCL : O_TRUNC), S_IRUSR | S_IWUSR);
        if (to_fd == -1) {
            goto handle_error;
        }

        buffer = ::std::get_temporary_buffer<unsigned char>(max_buffer_size);
        if (!buffer.first) {
            errno = ENOMEM;
            goto handle_error;
        }

        do
        {
            bytes_read = ::read(from_fd, buffer.first, buffer.second);
            if (bytes_read < 0) {
                goto handle_error;
            }

            bytes_written = ::write(to_fd, buffer.first, bytes_read);
            if (bytes_written < bytes_read) {
                if (bytes_written >= 0) {
                    errno = ENOSPC;
                }
                goto handle_error;
            }
        }
        while (bytes_read == buffer.second);

        if (::fchmod(to_fd, from_attributes.st_mode) == -1) {
            goto handle_error;
        }

        ::std::return_temporary_buffer(buffer.first);
        ::close(to_fd);
        ::close(from_fd);
        ec.clear();
        return true;

    handle_error:
        
        if (buffer.first) {
            ::std::return_temporary_buffer(buffer.first);
        }

        if (from_fd != -1) {
            ::close(from_fd);
        }

        if (to_fd != -1) {
            ::close(to_fd);
            ::unlink(to);
        }

        return false;
#endif
    }
}}
*/
