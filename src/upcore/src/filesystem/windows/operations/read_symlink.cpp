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

namespace up { namespace filesystem
{
    LIBUPCOREAPI UPALLOC UPWARNRESULT UPNONNULLALL
    char* read_symlink(char const* p) noexcept {
        char* new_buffer, * buffer = nullptr, * result = nullptr;
        size_t buffer_size = 4096;
        ssize_t result_size;

        for (;;) {
            new_buffer = static_cast<char*>(realloc(buffer, buffer_size));
            if (!new_buffer) {
                goto handle_error;
            }

            buffer = new_buffer;
            result_size = ::readlink(p, buffer, buffer_size);
            if (result_size <= -1) {
                goto handle_error;
            }
            else if (static_cast<size_t>(result_size) < buffer_size) {
                break;
            }

            buffer_size <<= 1;
        }

        result = static_cast<char*>(malloc(result_size + 1));
        if (!result) {
            goto handle_error;
        }

        memcpy(result, buffer, result_size);
        result[result_size] = 0;

    handle_error:

        free(buffer);
        return result;
    }
}}
*/
