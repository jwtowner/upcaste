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

#include <up/list.hpp>

namespace up
{
    LIBUPCOREAPI
    bool list_validate(list_node const* sentinel, ptrdiff_t expected_size) noexcept {
        if ( !sentinel || !sentinel->next || !sentinel->prev
            || ((sentinel->next == sentinel) && (sentinel->prev != sentinel))
            || ((sentinel->prev == sentinel) && (sentinel->next != sentinel))
        ) {
            return false;
        }

        list_node const* node = sentinel->next;
        list_node const* prev = sentinel;
        ptrdiff_t count = 0;

        while (node != sentinel) {
            if (node->prev != prev) {
                return false;
            }

            prev = node;
            node = node->next;
            ++count;
        }

        if (sentinel->prev != prev) {
            return false;
        }

        if ((expected_size != -1) && (expected_size != count)) {
            return false;
        }

        return true;
    }
}
