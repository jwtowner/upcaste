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

#include <up/slist.hpp>

namespace up
{   
    LIBUPCOREAPI
    bool slist_validate(slist_node const* head, ptrdiff_t expected_size) noexcept {
        if (!head) {
            return false;
        }

        for (slist_node const* slow = head->next, * fast1 = slow, * fast2 = slow; slow; slow = slow->next) {
            fast1 = fast2->next;
            if (!fast1) {
                break;
            }

            fast2 = fast1->next;
            if (!fast2) {
                break;
            }

            if ((slow == fast1) || (slow == fast2)) {
                return false;
            }
        }

        ptrdiff_t size = 0;

        for (slist_node const* node = head->next; node; node = node->next) {
            ++size;
        }
        
        return (expected_size < 0) ? true : (expected_size == size);
    }
}
