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

#include <up/sparsemap.hpp>
#include <up/sparseset.hpp>

namespace up
{
    LIBUPCOREAPI UPPURE
    ssize_t sparse_memory_footprint(size_t size, size_t num_buckets, size_t sizeof_record) noexcept {
        if (!num_buckets && !size) {
            return 0;
        }
        else if ( (sizeof_record < sizeof(slist_node))
            || ((num_buckets & (num_buckets - 1)) != 0)
            || ((num_buckets == 0) && (size > 0))
            || (num_buckets > (SIZE_MAX / sizeof(slist_node)))
            || (size > (SIZE_MAX / sizeof_record))
        ) {
            return -1;
        }
        
        size_t const bucket_size = (num_buckets * sizeof(slist_node));
        size_t const entry_size = (size * sizeof_record);
        if (addition_overflow(bucket_size, entry_size)) {
            return -1;
        }

        size_t const total_size = bucket_size + entry_size;
        if (total_size > (SIZE_MAX / 2)) {
            return -1;
        }

        return static_cast<ssize_t>(total_size);
    }
}
