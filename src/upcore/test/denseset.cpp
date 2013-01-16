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

#include <up/denseset.hpp>
#include <up/test.hpp>

namespace denseset
{
    struct int_float_pair
    {
        int key;
        float value;
        int_float_pair() { }
        int_float_pair(int k, float v) : key(k), value(v) { }
    };

    struct int_float_pair_hash
    {
        size_t operator()(int_float_pair const& p) const noexcept { return p.key; }
        size_t operator()(int k) const noexcept { return k; }
    };

    struct int_float_pair_equals
    {
        bool operator()(int_float_pair const& p1, int_float_pair const& p2) const noexcept { return p1.key == p2.key; }
        bool operator()(int_float_pair const& p1, int k2) const noexcept { return p1.key == k2; }
        bool operator()(int k1, int_float_pair const& p2) const noexcept { return k1 == p2.key; }
    };

    typedef up::denseset<int_float_pair,int_float_pair_hash, int_float_pair_equals> int_float_set;

    UP_TEST_CASE(construct_default) {
        int_float_set set;
        up::denseset_construct(set);
        require(up::denseset_validate(set));
        require(up::denseset_is_empty(set));
        require(up::denseset_size(set) == 0);
        require(up::denseset_num_buckets(set) == 0);
        require(up::denseset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(construct_destruct) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        int_float_set set;
        int retval;

        retval = up::denseset_construct(set, nullptr, 32);
        require(retval == up::dense_badallocator);

        retval = up::denseset_construct(set, alloc, SIZE_MAX / 2);
        require(retval == up::dense_badlength);

        retval = up::denseset_construct(set, alloc, 28);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require(up::denseset_is_empty(set));
        require(up::denseset_size(set) == 0);
        require(up::denseset_num_buckets(set) == 32);
        require(up::denseset_memory_footprint(set) >= (ssize_t)(32 * (sizeof(int) + sizeof(float) + sizeof(size_t))));

        retval = up::denseset_destruct(set, nullptr);
        require(retval == up::dense_badallocator);

        retval = up::denseset_destruct(set, alloc);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require(up::denseset_num_buckets(set) == 0);
        require(up::denseset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(insert_and_find) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        up::denseresult<int_float_pair> result;
        int_float_set set;
        int_float_pair* record;
        int retval;

        retval = up::denseset_construct(set, alloc, 32);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require(up::denseset_num_buckets(set) == 32);

        result = up::denseset_insert(set, int_float_pair(2, 3.5f));
        require(result.success && result.record && (result.record->key == 2) && (result.record->value == 3.5f));
        require(up::denseset_validate(set) && (up::denseset_size(set) == 1) && !up::denseset_is_empty(set));

        record = up::denseset_find(set, 34);
        require(!record);

        result = up::denseset_insert(set, int_float_pair(2, 6.5f));
        require(!result.success && result.record && (result.record->key == 2) && (result.record->value == 3.5f));
        require(up::denseset_validate(set) && (up::denseset_size(set) == 1) && !up::denseset_is_empty(set));

        result = up::denseset_insert(set, int_float_pair(34, 6.5f));
        require(result.success && result.record && (result.record->key == 34) && (result.record->value == 6.5f));
        require(up::denseset_validate(set) && (up::denseset_size(set) == 2) && !up::denseset_is_empty(set));

        record = up::denseset_find(set, 2);
        require(record && (record->key == 2) && (record->value == 3.5f));
        
        record = up::denseset_find(set, 34);
        require(record && (record->key == 34) && (record->value == 6.5f));
        
        record = up::denseset_find(set, 7);
        require(!record);

        retval = up::denseset_destruct(set, alloc);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require(up::denseset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(insert_erase) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        up::denseresult<int_float_pair> result;
        int_float_set set;
        int_float_pair* record;
        size_t count;
        int retval, i;

        retval = up::denseset_construct(set, alloc, 10000);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require((up::denseset_num_buckets(set) == 16384));

        for (i = 0; i < 10000; ++i) {
            result = up::denseset_insert(set, int_float_pair(i, i * 2.0f));
            require(result.success && result.record);
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::denseset_erase(set, i);
            require(count == 1);
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == 5000);

        for (i = 0; i < 10000; i += 2) {
            record = up::denseset_find(set, i);
            require(record && (record->key == i) && (record->value == (i * 2.0f)));
            count = up::denseset_erase(set, record);
            require(count == 1);
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == 0);

        for (i = 0; i < 10000; ++i) {
            record = up::denseset_find(set, i);
            require(!record);
        }

        retval = up::denseset_destruct(set, alloc);
        require(retval == up::dense_success);
    }

    UP_TEST_CASE(multi_insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        int_float_set set;
        int_float_pair* record;
        size_t count;
        int retval, i;

        retval = up::denseset_construct(set, alloc, 25600);
        require(retval == up::dense_success);
        require(up::denseset_validate(set));
        require(up::denseset_num_buckets(set) == 32768);

        for (i = 0; i < 25600; ++i) {
            record = up::denseset_multi_insert(set, int_float_pair(i & 255, i * 2.0f));
            require(record && (record->value == (i * 2.0f)));
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == 25600);

        for (i = 0; i < 256; ++i) {
            count = up::denseset_erase(set, i);
            require(count == 1);
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == (25600 - 256));

        for (i = 0; i < 256; ++i) {
            record = up::denseset_find(set, i);
            require(record && (record->key == i));
            count = up::denseset_multi_erase(set, i);
            require(count == ((25600 / 256) - 1));
        }

        require(up::denseset_validate(set));
        require(up::denseset_size(set) == 0);

        for (i = 0; i < 256; ++i) {
            record = up::denseset_find(set, i);
            require(!record);
        }

        retval = up::denseset_destruct(set, alloc);
        require(retval == up::dense_success);
    }

    UP_TEST_CASE(grow_and_shrink) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::denseresult<int_float_pair> result;
        int_float_set set;
        int_float_pair* record;
        size_t count;
        int retval, i;

        up::denseset_construct(set);
        require(up::denseset_validate(set));
        require(up::denseset_num_buckets(set) == 0);

        for (i = 0; i < 10000; ++i) {
            if (up::denseset_is_overloaded(set, 0.75)) {
                retval = up::denseset_grow(set, alloc);
                require(retval == up::dense_success);
            }

            result = up::denseset_insert(set, int_float_pair(i, i * 2.0f));
            require(result.success && result.record);
        }

        require(up::denseset_validate(set));
        require(up::denseset_num_buckets(set) >= 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::denseset_erase(set, i);
            require(count == 1);
        }

        up::denseset_shrink_to_fit(set, alloc);
        require(up::denseset_validate(set));

        for (i = 0; i < 10000; i += 2) {
            record = up::denseset_find(set, i);
            require(record && (record->value == (i * 2.0f)));
        }

        retval = up::denseset_destruct(set, alloc);
        require(retval == up::dense_success);
    }
}
