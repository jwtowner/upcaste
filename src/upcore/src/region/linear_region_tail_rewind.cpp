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

#include "linear_region_internal.hpp"
#include <up/cassert.hpp>
#include <up/cstdalign.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    void linear_region_tail_rewind(linear_region* r) noexcept {
        assert(linear_region_validate(r));

        size_t const alignment = r->alignment;
        size_t const align_mask = alignment - 1;
        size_t const offset = (alignment - (reinterpret_cast<uintptr_t>(r->head) & align_mask)) & align_mask;
        size_t space = r->space;
        void* ptr = r;

        verify(r == static_cast<linear_region*>(align(alignof(linear_region), sizeof(linear_region), &ptr, &space)));
        align_advance(sizeof(linear_region), &ptr, &space);

        char* rewound_head = static_cast<char*>(align_bidirectional(alignment, offset, alignment, &ptr, &space));
        char* rewound_tail = rewound_head + space;
        assert(rewound_head && (rewound_head <= r->head) && (rewound_tail >= r->tail));

        r->tail = rewound_tail;
    }

    LIBUPCOREAPI UPNONNULLALL
    void linear_region_tail_rewind(linear_region* r, void* p) noexcept {
        assert(linear_region_validate(r));

        char* const position = static_cast<char*>(p);
        if (!p || (position <= r->head)) {
            return;
        }

        size_t const alignment = r->alignment;
        size_t const align_mask = alignment - 1;
        size_t const offset = (alignment - (reinterpret_cast<uintptr_t>(r->head) & align_mask)) & align_mask;
        size_t space = r->space;
        void* ptr = r;

        verify(r == static_cast<linear_region*>(align(alignof(linear_region), sizeof(linear_region), &ptr, &space)));
        align_advance(sizeof(linear_region), &ptr, &space);

        char* const rewound_tail = static_cast<char*>(align_bidirectional(alignment, offset, alignment, &ptr, &space)) + space;
        assert(rewound_tail >= r->tail);

        size_t tail_space = position - r->head;
        char* const new_tail = static_cast<char*>(align_bidirectional(alignment, offset, alignment, &p, &tail_space));
        if (new_tail && (r->head <= new_tail) && (new_tail <= rewound_tail)) {
            r->tail = new_tail;
        }
    }
}
