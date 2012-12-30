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

#include <up/sparseset.hpp>
#include <up/test.hpp>

namespace sparseset
{
    struct int_float_pair
    {
        up::slist_node node;
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

    typedef up::sparseset<int_float_pair, &int_float_pair::node, int_float_pair_hash, int_float_pair_equals> int_float_set;

    UP_TEST_CASE(construct_default) {
        int_float_set set;
        up::sparseset_construct(set);
        require(up::sparseset_validate(set));
        require(up::sparseset_is_empty(set));
        require(up::sparseset_size(set) == 0);
        require(up::sparseset_num_buckets(set) == 0);
        require(up::sparseset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(construct_destruct) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        int_float_set set;
        int retval;

        retval = up::sparseset_construct(set, nullptr, 32);
        require(retval == up::sparse_badallocator);

        retval = up::sparseset_construct(set, alloc, SIZE_MAX / 2);
        require(retval == up::sparse_badlength);

        retval = up::sparseset_construct(set, alloc, 28);
        require(retval == up::sparse_success);
        require(up::sparseset_validate(set));
        require(up::sparseset_is_empty(set));
        require(up::sparseset_size(set) == 0);
        require(up::sparseset_num_buckets(set) == 32);
        require(up::sparseset_memory_footprint(set) == (32 * sizeof(up::slist_node)));

        retval = up::sparseset_destruct(set, (up::allocator*)nullptr, alloc);
        require(retval == up::sparse_badallocator);

        retval = up::sparseset_destruct(set, alloc, (up::allocator*)nullptr);
        require(retval == up::sparse_badallocator);

        retval = up::sparseset_destruct(set, alloc, alloc);
        require(retval == up::sparse_success);
        require(up::sparseset_validate(set));
        require(up::sparseset_num_buckets(set) == 0);
        require(up::sparseset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(insert_and_find) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::sparseresult<int_float_pair> result;
        int_float_set set;
        int_float_pair* record;
        int retval;

        retval = up::sparseset_construct(set, alloc, 32);
        require(retval == up::sparse_success);
        require(up::sparseset_validate(set));
        require(up::sparseset_num_buckets(set) == 32);

        result = up::sparseset_insert(set, alloc, int_float_pair(2, 3.5f));
        require(result.success && result.record && (result.record->key == 2) && (result.record->value == 3.5f));
        require(up::sparseset_validate(set) && (up::sparseset_size(set) == 1) && !up::sparseset_is_empty(set));

        record = up::sparseset_find(set, 34);
        require(!record);

        result = up::sparseset_insert(set, alloc, int_float_pair(2, 6.5f));
        require(!result.success && result.record && (result.record->key == 2) && (result.record->value == 3.5f));
        require(up::sparseset_validate(set) && (up::sparseset_size(set) == 1) && !up::sparseset_is_empty(set));

        result = up::sparseset_insert(set, alloc, int_float_pair(34, 6.5f));
        require(result.success && result.record && (result.record->key == 34) && (result.record->value == 6.5f));
        require(up::sparseset_validate(set) && (up::sparseset_size(set) == 2) && !up::sparseset_is_empty(set));

        record = up::sparseset_find(set, 2);
        require(record && (record->key == 2) && (record->value == 3.5f));
        
        record = up::sparseset_find(set, 34);
        require(record && (record->key == 34) && (record->value == 6.5f));
        
        record = up::sparseset_find(set, 7);
        require(!record);

        retval = up::sparseset_destruct(set, alloc, alloc);
        require(retval == up::sparse_success);
        require(up::sparseset_validate(set));
        require(up::sparseset_memory_footprint(set) == 0);
    }
    
    /*
    UP_TEST_CASE(insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::sparsemap<int, float, int_hash, int_equals> map;
        up::sparserecord<int, float>* record;
        up::sparseresult<int, float> result;
        size_t count;
        int retval, i;

        retval = up::sparsemap_construct(map, alloc, 256);
        require(retval == up::sparse_success);
        require(up::sparsemap_validate(map) && (up::sparsemap_num_buckets(map) == 256));

        for (i = 0; i < 10000; ++i) {
            result = up::sparsemap_insert(map, alloc, i, i * 2.0f);
            require(result.success && result.record);
        }

        for (i = 1; i < 10000; i += 2) {
            count = up::sparsemap_erase(map, alloc, i);
            require(count == 1);
        }

        require(up::sparsemap_validate(map) && (up::sparsemap_size(map) == 5000));

        for (i = 0; i < 10000; i += 2) {
            record = up::sparsemap_find(map, i);
            require(record && (record->key == i) && (record->value == (i * 2.0f)));
            count = up::sparsemap_erase(map, alloc, record);
            require(count == 1);
        }

        require(up::sparsemap_validate(map) && (up::sparsemap_size(map) == 0));

        for (i = 0; i < 10000; ++i) {
            record = up::sparsemap_find(map, i);
            require(!record);
        }

        retval = up::sparsemap_destruct(map, alloc, alloc);
        require(retval == up::sparse_success);
    }
    
    UP_TEST_CASE(multi_insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::sparsemap<int, float, int_hash, int_equals> map;
        up::sparserecord<int, float>* record;
        size_t count;
        int retval, i;

        retval = up::sparsemap_construct(map, alloc, 256);
        require(retval == up::sparse_success);
        require(up::sparsemap_validate(map) && (up::sparsemap_num_buckets(map) == 256));

        for (i = 0; i < 25600; ++i) {
            record = up::sparsemap_multi_insert(map, alloc, i & 255, i * 2.0f);
            require(record && (record->value == (i * 2.0f)));
        }

        for (i = 0; i < 256; ++i) {
            count = up::sparsemap_erase(map, alloc, i);
            require(count == 1);
        }

        require(up::sparsemap_validate(map) && (up::sparsemap_size(map) == (25600 - 256)));

        for (i = 0; i < 256; ++i) {
            record = up::sparsemap_find(map, i);
            require(record && (record->key == i));
            count = up::sparsemap_multi_erase(map, alloc, i);
            require(count == ((25600 / 256) - 1));
        }

        require(up::sparsemap_validate(map) && (up::sparsemap_size(map) == 0));

        for (i = 0; i < 256; ++i) {
            record = up::sparsemap_find(map, i);
            require(!record);
        }

        retval = up::sparsemap_destruct(map, alloc, alloc);
        require(retval == up::sparse_success);
    }

    UP_TEST_CASE(grow_and_shrink) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::sparsemap<int, float, int_hash, int_equals> map;
        size_t count;
        int retval, i;
        float value;

        up::sparsemap_construct(map);
        require(up::sparsemap_validate(map) && (up::sparsemap_num_buckets(map) == 0));

        for (i = 0; i < 10000; ++i) {
            if (up::sparsemap_is_overloaded(map, 8.0)) {
                retval = up::sparsemap_grow(map, alloc);
                require(retval == up::sparse_success);
            }

            retval = up::sparsemap_set(map, alloc, i, i * 2.0f);
            require(retval == up::sparse_success);
        }

        require(up::sparsemap_validate(map) && (up::sparsemap_num_buckets(map) >= 1250));

        for (i = 1; i < 10000; i += 2) {
            count = up::sparsemap_erase(map, alloc, i);
            require(count == 1);
        }

        up::sparsemap_shrink_to_fit(map, alloc);
        require(up::sparsemap_validate(map));

        for (i = 0; i < 10000; i += 2) {
            value = up::sparsemap_get(map, i, -2.0f);
            require(value == (i * 2.0f));
        }

        retval = up::sparsemap_destruct(map, alloc, alloc);
        require(retval == up::sparse_success);
    }*/
}
