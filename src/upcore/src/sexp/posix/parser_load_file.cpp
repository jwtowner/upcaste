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

#include <up/sexp.hpp>
#include <up/cassert.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/filesystem.hpp>
#include <fcntl.h>
#include <unistd.h>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, char const* UPRESTRICT filename) noexcept {
        if (!par || !filename) {
            return sexp_badarg;
        }
        
        allocator* const alloc = par->alloc;
        int fd, retval;
        off64_t file_length;
        ssize_t length_read;
        size_t length, length_remaining;
        char* text;
        char* filename_copy;
        size_t filename_length;
        lexer lex;

        // open the file
        fd = ::open(filename, O_RDONLY);
        if (fd == -1) {
            return sexp_ioerror;
        }

        // determine length of file and 
        file_length = ::lseek64(fd, 0, SEEK_END);
        if (file_length == static_cast<off64_t>(-1)) {
            ::close(fd);
            return sexp_ioerror;
        }
        else if (file_length > static_cast<off64_t>(SSIZE_MAX)) {
            ::close(fd);
            return sexp_nomem;
        }

        if (::lseek64(fd, 0, SEEK_SET) != 0) {
            ::close(fd);
            return sexp_ioerror;
        }

        // allocate text buffer large enough to hold file contents
        length = static_cast<size_t>(file_length);
        text = static_cast<char*>(alloc->allocate(length));
        if (!text) {
            ::close(fd);
            return sexp_nomem;
        }

        // read in the total file, handling interruptions
        length_remaining = length;
        do {
            length_read = ::read(fd, text + (length - length_remaining), length_remaining);
            if (length_read == -1) {
                alloc->deallocate(text, length);
                ::close(fd);
                return sexp_ioerror;
            }
            length_remaining -= length_read;
        }
        while (length_remaining > 0);

        ::close(fd);

        // initialize the lexer, copy filename, and unload the old state.
        // once this is successful, we can save the new state for the
        // freshly loaded file.
        retval = lexer_construct(&lex, text, length);
        if (retval != sexp_success) {
            return retval;
        }

        if (filename) {
            filename_length = strlen(filename);
            filename_copy = strndup(filename, filename_length, alloc);
            if (!filename_copy) {
                alloc->deallocate(text, length);
                return sexp_nomem;
            }
        }

        if (par->loaded) {
            retval = parser_unload(par);
            if (retval != sexp_success) {
                if (filename_copy) {
                    alloc->deallocate(filename_copy, filename_length + 1);
                    alloc->deallocate(text, length);
                }
                return retval;
            }
        }

        par->lex = lex;
        par->prev_column = 0;
        par->prev_line = 0;
        par->filename = filename_copy;
        par->filename_length = filename_length;
        par->buffer = const_cast<char*>(text);
        par->buffer_length = length;
        par->buffer_allocated = true;
        par->loaded = true;
        return sexp_success;
    }

#ifdef UP_HAS_STDC_WCHAR
    LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, wchar_t const* UPRESTRICT filename) noexcept {
        if (!par || !filename) {
            return sexp_badarg;
        }
        char* native_filename = filesystem::transcode(filename);
        int retval = parser_load_file(par, native_filename);
        free(native_filename);
        return retval;
    }
#endif
}}
