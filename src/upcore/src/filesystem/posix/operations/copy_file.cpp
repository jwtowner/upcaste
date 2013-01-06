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
        constexpr size_t max_buffer_size = 1024 * 1024;
        size_t buffer_size = 0;
        char* buffer = nullptr;
        
        int from_fd = -1, to_fd = -1;
        struct ::stat from_attributes;
        ssize_t bytes_read, bytes_written;

        // open both files
        from_fd = ::open(from, O_RDONLY, 0);
        if (from_fd == -1) {
            goto handle_error;
        }

        if (::fstat(from_fd, &from_attributes) == -1) {
            goto handle_error;
        }

        to_fd = ::open(to, O_CREAT | O_WRONLY | O_CREAT | ((option == copy_overwrite) ? O_EXCL : O_TRUNC), S_IRUSR | S_IWUSR);
        if (to_fd == -1) {
            goto handle_error;
        }

        // allocate temporary buffer
        buffer_size = max_buffer_size;
        for (;;) {
            buffer = static_cast<char*>(malloc(buffer_size));
            if (buffer) {
                break;
            }
            buffer_size >>= 1;
            if (!buffer_size) {
                errno = ENOMEM;
                goto handle_error;
            }
        }

        // copy file from source to destination in buffer-sized chunks
        do
        {
            bytes_read = ::read(from_fd, buffer, buffer_size);
            if (bytes_read < 0) {
                goto handle_error;
            }

            bytes_written = ::write(to_fd, buffer, bytes_read);
            if (bytes_written < bytes_read) {
                if (bytes_written >= 0) {
                    errno = ENOSPC;
                }
                goto handle_error;
            }
        }
        while (bytes_read == static_cast<ssize_t>(buffer_size));

        // set parent file attributes
        if (::fchmod(to_fd, from_attributes.st_mode) == -1) {
            goto handle_error;
        }

        // cleanup
        free(buffer);
        ::close(to_fd);
        ::close(from_fd);
        return true;

    handle_error:
        
        if (buffer) {
            free(buffer);
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
