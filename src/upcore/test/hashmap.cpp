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

#include <up/hashmap.hpp>
#include <up/test.hpp>

namespace hashmap
{
    UP_TEST_CASE(construct_default) {
        up::hashmap<unsigned int, float> map;
        up::hashmap_construct(map);
        require(up::hashmap_validate(map));
        require(up::hashmap_is_empty(map));
        require(up::hashmap_size(map) == 0);
        require(up::hashmap_num_buckets(map) == 0);
        require(up::hashmap_memory_footprint(map) == 0);
    }

    UP_TEST_CASE(construct_destruct) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        int retval;

        retval = up::hashmap_construct(map, nullptr, 16);
        require(retval == up::hash_badallocator);

        retval = up::hashmap_construct(map, alloc, SIZE_MAX / 2);
        require(retval == up::hash_badlength);

        retval = up::hashmap_construct(map, alloc, 14);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map));
        require(up::hashmap_is_empty(map));
        require(up::hashmap_size(map) == 0);
        require(up::hashmap_num_buckets(map) == 16);
        require(up::hashmap_memory_footprint(map) >= (ssize_t)(16 * (sizeof(unsigned int) + sizeof(float))));

        retval = up::hashmap_destruct(map, nullptr);
        require(retval == up::hash_badallocator);

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map));
        require(up::hashmap_num_buckets(map) == 0);
        require(up::hashmap_memory_footprint(map) == 0);
    }

    UP_TEST_CASE(insert_and_find) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        up::hashresult<unsigned int, float> result;
        up::hashrecord<unsigned int, float> record;

        int retval = up::hashmap_construct(map, alloc, 24);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) == 32));
        require(up::hashmap_is_empty(map) && (up::hashmap_size(map) == 0));

        result = up::hashmap_insert(map, 2, 3.5f);
        require(result.success && result.record.valid() && (result.record.key() == 2) && (result.record.value() == 3.5f));
        require((up::hashmap_size(map) == 1) && up::hashmap_validate(map));

        result = up::hashmap_insert(map, 2, 6.5f);
        require(!result.success && result.record.valid() && (result.record.key() == 2) && (result.record.value() == 3.5f));
        require((up::hashmap_size(map) == 1) && up::hashmap_validate(map));

        result = up::hashmap_insert(map, 34, 6.5f);
        require(result.success && result.record.valid() && (result.record.key() == 34) && (result.record.value() == 6.5f));
        require((up::hashmap_size(map) == 2) && up::hashmap_validate(map));

        record = up::hashmap_find(map, 2);
        require(record.valid() && (record.key() == 2) && (record.value() == 3.5f));
        require((up::hashmap_size(map) == 2) && up::hashmap_validate(map));

        record = up::hashmap_find(map, 34);
        require(record.valid() && (record.key() == 34) && (record.value() == 6.5f));
        require((up::hashmap_size(map) == 2) && up::hashmap_validate(map));

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) == 0));
        require(up::hashmap_is_empty(map) && (up::hashmap_size(map) == 0));
    }

    UP_TEST_CASE(insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        up::hashrecord<unsigned int, float> record;
        up::hashresult<unsigned int, float> result;
        size_t count;
        int retval, i;

        retval = up::hashmap_construct(map, alloc, 10000);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) >= 10000));

        for (i = 0; i < 10000; ++i) {
            result = up::hashmap_insert(map, i, i * 2.0f);
            require(result.success && result.record.valid());
        }

        for (i = 1; i < 10000; i += 2) {
            count = up::hashmap_erase(map, i);
            require(count == 1);
        }

        require(up::hashmap_validate(map) && (up::hashmap_size(map) == 5000));

        for (i = 0; i < 10000; i += 2) {
            record = up::hashmap_find(map, i);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i)) && (record.value() == (i * 2.0f)));
            count = up::hashmap_erase(map, record);
            require(count == 1);
        }

        require(up::hashmap_validate(map) && (up::hashmap_size(map) == 0));

        for (i = 0; i < 10000; ++i) {
            record = up::hashmap_find(map, i);
            require(!record.valid());
        }

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
    }

    UP_TEST_CASE(multi_insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        up::hashrecord<unsigned int, float> record;
        size_t count;
        int retval, i;

        retval = up::hashmap_construct(map, alloc, 25600);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) >= 25600));

        for (i = 0; i < 25600; ++i) {
            record = up::hashmap_multi_insert(map, i & 255, i * 2.0f);
            require(record.valid() && (record.value() == (i * 2.0f)));
        }

        for (i = 0; i < 256; ++i) {
            count = up::hashmap_erase(map, i);
            require(count == 1);
        }

        require(up::hashmap_validate(map) && (up::hashmap_size(map) == (25600 - 256)));

        for (i = 0; i < 256; ++i) {
            record = up::hashmap_find(map, i);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i)));
            count = up::hashmap_multi_erase(map, i);
            require(count == ((25600 / 256) - 1));
        }

        require(up::hashmap_validate(map) && (up::hashmap_size(map) == 0));

        for (i = 0; i < 256; ++i) {
            record = up::hashmap_find(map, i);
            require(!record.valid());
        }

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
    }

    UP_TEST_CASE(set_and_get) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        int retval, i;
        float value;

        retval = up::hashmap_construct(map, alloc, 10000);
        require(retval == up::hash_success);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) >= 10000));

        for (i = 0; i < 10000; ++i) {
            retval = up::hashmap_set(map, i, i * 2.0f);
            require(retval == up::hash_success);
        }

        require(up::hashmap_validate(map) && (up::hashmap_size(map) == 10000));

        for (i = 0; i < 10000; ++i) {
            value = up::hashmap_get(map, i, -2.0f);
            require(value == (i * 2.0f));
        }

        value = up::hashmap_get(map, 100000, -1.0f);
        require(value == -1.0f);

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
    }

    UP_TEST_CASE(grow_and_shrink) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashmap<unsigned int, float> map;
        size_t count;
        int retval, i;
        float value;

        up::hashmap_construct(map);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) == 0));

        for (i = 0; i < 10000; ++i) {
            if (up::hashmap_is_overloaded(map, 0.75)) {
                retval = up::hashmap_grow(map, alloc);
                require(retval == up::hash_success);
            }

            retval = up::hashmap_set(map, i, i * 2.0f);
            require(retval == up::hash_success);
        }

        require(up::hashmap_validate(map));
        require(up::hashmap_num_buckets(map) >= 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::hashmap_erase(map, i);
            require(count == 1);
        }

        up::hashmap_shrink_to_fit(map, alloc);
        require(up::hashmap_validate(map) && (up::hashmap_num_buckets(map) >= 5000) && (up::hashmap_num_buckets(map) < 10000));

        for (i = 0; i < 10000; i += 2) {
            value = up::hashmap_get(map, i, -2.0f);
            require(value == (i * 2.0f));
        }

        retval = up::hashmap_destruct(map, alloc);
        require(retval == up::hash_success);
    }
}
