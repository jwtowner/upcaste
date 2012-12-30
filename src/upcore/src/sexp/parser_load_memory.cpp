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

#include <up/sexp.hpp>
#include <up/cassert.hpp>
#include <up/cstring.hpp>

namespace up { namespace sexp
{
    LIBUPCOREAPI
    int parser_load_memory(parser* UPRESTRICT par, char const* UPRESTRICT filename, char const* UPRESTRICT text, size_t length) noexcept {
        if (!par) {
            return sexp_badarg;
        }

        allocator* const alloc = par->alloc;
        assert(alloc);

        lexer lex;
        char* filename_copy = nullptr;
        size_t filename_length = 0;
        int retval;

        retval = lexer_construct(&lex, text, length);
        if (retval != sexp_success) {
            return retval;
        }

        if (filename) {
            filename_length = strlen(filename);
            if (filename_length > 0) {
                filename_copy = static_cast<char*>(alloc->allocate(filename_length + 1));
                if (!filename_copy) {
                    return sexp_nomem;
                }
                memcpy(filename_copy, filename, filename_length + 1);
            }
        }

        if (par->loaded) {
            retval = parser_unload(par);
            if (retval != sexp_success) {
                if (filename_copy) {
                    alloc->deallocate(filename_copy, filename_length + 1);
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
        par->buffer_allocated = false;
        par->loaded = true;
        return sexp_success;
    }
}}
