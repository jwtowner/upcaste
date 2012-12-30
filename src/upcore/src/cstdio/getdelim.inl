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

#include <up/cerrno.hpp>
#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cwchar.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULL(4)
    ssize_t NAME(CHAR** UPRESTRICT lineptr, size_t* UPRESTRICT n, CHAR_INT delimiter, FILE* UPRESTRICT stream) {
        constexpr size_t work_length = 8192 - 1;
        size_t length = 0, new_length, work_read;
        bool done = false;
        CHAR_INT ch;
        CHAR* work;
        CHAR work_buffer[work_length];

        if (UPUNLIKELY(!lineptr || !n)) {
            errno = EINVAL;
            return -1;
        }

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
        _lock_file(stream);
        __try {
#else
        flockfile(stream);
#endif

        do {
            for (work = work_buffer; work < &work_buffer[work_length]; ++work) {
#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
                ch = UPCONCATENATE(_, UPCONCATENATE(CHAR_GET, _nolock))(stream);
#else
                ch = UPCONCATENATE(CHAR_GET, _unlocked)(stream);
#endif
                if (ch == CHAR_EOF) {
                    if (!length) {
                        return -1;
                    }
                    done = true;
                    break;
                }

                *work++ = static_cast<CHAR>(ch);
                if (ch == delimiter) {
                    done = true;
                    break;
                }
            }

            work_read = &work_buffer[work_length] - work;
            new_length = length + work_read;
            if (UPUNLIKELY(new_length > (SSIZE_MAX / sizeof(CHAR)))) {
                errno = EOVERFLOW;
                return -1;
            }

            if ((new_length >= *n) || !*lineptr) {
                work = static_cast<CHAR*>(realloc(*lineptr, sizeof(CHAR) * (new_length + 1)));
                if (UPUNLIKELY(!work)) {
                    errno = ENOMEM;
                    return -1;
                }
                *lineptr = work;
                *n = new_length + 1;
            }

            *static_cast<CHAR*>(MEMPCPY(*lineptr + length, work_buffer, work_read)) = CHAR_NIL;
            length = new_length;
        }
        while (!done);

#if UP_STDC_EXTENSIONS == UP_STDC_EXTENSIONS_MSVC
        }
        __finally {
            _unlock_file(stream);
        }
#else
        funlockfile(stream);
#endif

        return static_cast<ssize_t>(length);
    }
}
