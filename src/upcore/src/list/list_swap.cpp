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

#include <up/list.hpp>
#include <up/utility.hpp>

namespace up
{   
    LIBUPCOREAPI
    void list_swap(list_node* x, list_node* y) noexcept {
        bool const x_linked = (x->next != x);
        bool const y_linked = (y->next != y);

        if (x_linked && y_linked) {
            swap(x->next->prev, y->next->prev);
            swap(x->prev->next, y->prev->next);
            swap(x->next, y->next);
            swap(x->prev, y->prev);
        }
        else if (x_linked) {
            y->next = x->next;
            y->prev = x->prev;
            x->next = x->prev = x;
        }
        else if (x_linked) {
            x->next = y->next;
            x->prev = y->prev;
            y->next = y->prev = y;
        }
    }
}
