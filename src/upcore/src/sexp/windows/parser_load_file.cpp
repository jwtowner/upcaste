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
#include <up/cuchar.hpp>
#include <up/filesystem.hpp>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

namespace up { namespace sexp { namespace
{
    int parser_load_file(
        parser* UPRESTRICT par,
        char const* UPRESTRICT filename,
        wchar_t const* UPRESTRICT wfilename
    )
    noexcept {
        allocator* const alloc = par->alloc;
        HANDLE file = INVALID_HANDLE_VALUE;
        LARGE_INTEGER file_length;
        DWORD length = 0, length_read;
        char* text = nullptr;
        char* filename_copy = nullptr;
        size_t filename_length = 0;
        int retval;
        lexer lex;

        // open the file
        file = ::CreateFileW(wfilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (file == INVALID_HANDLE_VALUE) {
            return sexp_ioerror;
        }

        // determine length of file
        file_length.QuadPart = 0;
        if (::SetFilePointerEx(file, file_length, &file_length, FILE_END) == FALSE) {
            retval = sexp_ioerror;
            goto error;
        }
        
        if (file_length.QuadPart > MAXDWORD) {
            retval = sexp_nomem;
            goto error;
        }

        // allocate text buffer large enough to hold file contents
        length = static_cast<DWORD>(file_length.QuadPart);
        text = static_cast<char*>(alloc->allocate(length));
        if (!text) {
            retval = sexp_nomem;
            goto error;
        }

        // reset file pointer, read in the contents of the file, and close it
        file_length.QuadPart = 0;
        if (::SetFilePointerEx(file, file_length, &file_length, FILE_BEGIN) == FALSE) {
            retval = sexp_ioerror;
            goto error;
        }

        if (::ReadFile(file, text, length, &length_read, NULL) == FALSE) {
            retval = sexp_ioerror;
            goto error;
        }

        if (length != length_read) {
            retval = sexp_ioerror;
            goto error;
        }

        ::CloseHandle(file);
        file = INVALID_HANDLE_VALUE;

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
                retval = sexp_nomem;
                goto error;
            }
        }

        if (par->loaded) {
            retval = parser_unload(par);
            if (retval != sexp_success) {
                goto error;
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

    error:

        if (filename_copy) {
            alloc->deallocate(filename_copy, filename_length + 1);
        }
        if (text) {
            alloc->deallocate(text, length);
        }
        if (file != INVALID_HANDLE_VALUE) {
            ::CloseHandle(file);
        }
        return retval;
    }
}}}

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, char const* UPRESTRICT filename) noexcept {
        if (!par || !filename) {
            return sexp_badarg;
        }
        wchar_t* native_filename = filesystem::transcode(filename);
        int retval = parser_load_file(par, filename, native_filename);
        free(native_filename);
        return retval;
    }

#ifdef UP_HAS_STDC_WCHAR
    LIBUPCOREAPI
    int parser_load_file(parser* UPRESTRICT par, wchar_t const* UPRESTRICT filename) noexcept {
        if (!par || !filename) {
            return sexp_badarg;
        }
        char* ascii_filename = filesystem::transcode(filename);
        int retval = parser_load_file(par, ascii_filename, filename);
        free(ascii_filename);
        return retval;
    }
#endif
}}
