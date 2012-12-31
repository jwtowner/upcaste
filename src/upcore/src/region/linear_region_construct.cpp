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

#include "linear_region_internal.hpp"
#include <up/cerrno.hpp>
#include <new>

namespace up
{
    LIBUPCOREAPI UPWARNRESULT
    linear_region* linear_region_construct(void* base, size_t space, size_t alignment, size_t offset) noexcept {
        size_t const original_space = space;
        linear_region* region;
        void* ptr = base;
        
        region = static_cast<linear_region*>(align(alignof(linear_region), sizeof(linear_region), &ptr, &space));
        if (UPUNLIKELY(!region)) {
            return nullptr;
        }
        else if (UPUNLIKELY(region != base)) {
            errno = EINVAL;
            return nullptr;
        }

        align_advance(sizeof(linear_region), &ptr, &space);
        if (UPUNLIKELY(!align_bidirectional(alignment, offset, alignment, &ptr, &space))) {
            return nullptr;
        }

        return ::new(region) linear_region(ptr, space, alignment, original_space);
    }
}
