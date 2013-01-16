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

#include <up/densemap.hpp>
#include <up/test.hpp>

namespace densemap
{
    struct int_hash
    {
        size_t operator()(int k) const noexcept { return k; }
    };

    struct int_equals
    {
        bool operator()(int k1, int k2) const noexcept { return k1 == k2; }
    };
    
    UP_TEST_CASE(construct_default) {
        up::densemap<int, float, int_hash, int_equals> map;
        up::densemap_construct(map);
        require(up::densemap_validate(map));
        require(up::densemap_is_empty(map));
        require(up::densemap_size(map) == 0);
        require(up::densemap_num_buckets(map) == 0);
        require(up::densemap_memory_footprint(map) == 0);
    }

    UP_TEST_CASE(construct_destruct) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;
        int retval;

        retval = up::densemap_construct(map, nullptr, 32);
        require(retval == up::dense_badallocator);

        retval = up::densemap_construct(map, alloc, SIZE_MAX / 2);
        require(retval == up::dense_badlength);

        retval = up::densemap_construct(map, alloc, 28);
        require(retval == up::dense_success);
        require(up::densemap_validate(map));
        require(up::densemap_is_empty(map));
        require(up::densemap_size(map) == 0);
        require(up::densemap_num_buckets(map) == 32);
        require(up::densemap_memory_footprint(map) >= (ssize_t)(32 * (sizeof(int) + sizeof(float) + sizeof(size_t))));

        retval = up::densemap_destruct(map, nullptr);
        require(retval == up::dense_badallocator);

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
        require(up::densemap_validate(map));
        require(up::densemap_num_buckets(map) == 0);
        require(up::densemap_memory_footprint(map) == 0);
    }

    UP_TEST_CASE(insert_and_find) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;

        int retval = up::densemap_construct(map, alloc, 24);
        require(retval == up::dense_success);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) == 32));
        require(up::densemap_is_empty(map) && (up::densemap_size(map) == 0));

        up::denseresult<int, float> result = up::densemap_insert(map, 2, 3.5f);
        require(result.success && result.record.valid() && (result.record.key() == 2) && (result.record.value() == 3.5f));
        require((up::densemap_size(map) == 1) && up::densemap_validate(map));

        result = up::densemap_insert(map, 2, 6.5f);
        require(!result.success && result.record.valid() && (result.record.key() == 2) && (result.record.value() == 3.5f));
        require((up::densemap_size(map) == 1) && up::densemap_validate(map));

        result = up::densemap_insert(map, 34, 6.5f);
        require(result.success && result.record.valid() && (result.record.key() == 34) && (result.record.value() == 6.5f));
        require((up::densemap_size(map) == 2) && up::densemap_validate(map));

        up::denserecord<int, float> record = up::densemap_find(map, 2);
        require(record.valid() && (record.key() == 2) && (record.value() == 3.5f));
        require((up::densemap_size(map) == 2) && up::densemap_validate(map));

        record = up::densemap_find(map, 34);
        require(record.valid() && (record.key() == 34) && (record.value() == 6.5f));
        require((up::densemap_size(map) == 2) && up::densemap_validate(map));

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) == 0));
        require(up::densemap_is_empty(map) && (up::densemap_size(map) == 0));
    }

    UP_TEST_CASE(insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;
        up::denserecord<int, float> record;
        up::denseresult<int, float> result;
        size_t count;
        int retval, i;

        retval = up::densemap_construct(map, alloc, 10000);
        require(retval == up::dense_success);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) >= 10000));

        for (i = 0; i < 10000; ++i) {
            result = up::densemap_insert(map, i, i * 2.0f);
            require(result.success && result.record.valid());
        }

        for (i = 1; i < 10000; i += 2) {
            count = up::densemap_erase(map, i);
            require(count == 1);
        }

        require(up::densemap_validate(map) && (up::densemap_size(map) == 5000));

        for (i = 0; i < 10000; i += 2) {
            record = up::densemap_find(map, i);
            require(record.valid() && (record.key() == i) && (record.value() == (i * 2.0f)));
            count = up::densemap_erase(map, record);
            require(count == 1);
        }

        require(up::densemap_validate(map) && (up::densemap_size(map) == 0));

        for (i = 0; i < 10000; ++i) {
            record = up::densemap_find(map, i);
            require(!record.valid());
        }

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
    }

    UP_TEST_CASE(multi_insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;
        up::denserecord<int, float> record;
        size_t count;
        int retval, i;

        retval = up::densemap_construct(map, alloc, 25600);
        require(retval == up::dense_success);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) >= 25600));

        for (i = 0; i < 25600; ++i) {
            record = up::densemap_multi_insert(map, i & 255, i * 2.0f);
            require(record.valid() && (record.value() == (i * 2.0f)));
        }

        for (i = 0; i < 256; ++i) {
            count = up::densemap_erase(map, i);
            require(count == 1);
        }

        require(up::densemap_validate(map) && (up::densemap_size(map) == (25600 - 256)));

        for (i = 0; i < 256; ++i) {
            record = up::densemap_find(map, i);
            require(record.valid() && (record.key() == i));
            count = up::densemap_multi_erase(map, i);
            require(count == ((25600 / 256) - 1));
        }

        require(up::densemap_validate(map) && (up::densemap_size(map) == 0));

        for (i = 0; i < 256; ++i) {
            record = up::densemap_find(map, i);
            require(!record.valid());
        }

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
    }

    UP_TEST_CASE(set_and_get) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;
        int retval, i;
        float value;

        retval = up::densemap_construct(map, alloc, 10000);
        require(retval == up::dense_success);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) >= 10000));

        for (i = 0; i < 10000; ++i) {
            retval = up::densemap_set(map, i, i * 2.0f);
            require(retval == up::dense_success);
        }

        require(up::densemap_validate(map) && (up::densemap_size(map) == 10000));

        for (i = 0; i < 10000; ++i) {
            value = up::densemap_get(map, i, -2.0f);
            require(value == (i * 2.0f));
        }

        value = up::densemap_get(map, -1, -1.0f);
        require(value == -1.0f);

        value = up::densemap_get(map, 100000, -1.0f);
        require(value == -1.0f);

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
    }

    UP_TEST_CASE(grow_and_shrink) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::densemap<int, float, int_hash, int_equals> map;
        size_t count;
        int retval, i;
        float value;

        up::densemap_construct(map);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) == 0));

        for (i = 0; i < 10000; ++i) {
            if (up::densemap_is_overloaded(map, 0.75)) {
                retval = up::densemap_grow(map, alloc);
                require(retval == up::dense_success);
            }

            retval = up::densemap_set(map, i, i * 2.0f);
            require(retval == up::dense_success);
        }

        require(up::densemap_validate(map));
        require(up::densemap_num_buckets(map) >= 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::densemap_erase(map, i);
            require(count == 1);
        }

        up::densemap_shrink_to_fit(map, alloc);
        require(up::densemap_validate(map) && (up::densemap_num_buckets(map) >= 5000) && (up::densemap_num_buckets(map) < 10000));

        for (i = 0; i < 10000; i += 2) {
            value = up::densemap_get(map, i, -2.0f);
            require(value == (i * 2.0f));
        }

        retval = up::densemap_destruct(map, alloc);
        require(retval == up::dense_success);
    }
}
