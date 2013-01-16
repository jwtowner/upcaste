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

#include <up/hashset.hpp>
#include <up/test.hpp>

namespace hashset
{
    UP_TEST_CASE(construct_default) {
        up::hashset<unsigned int> set;
        up::hashset_construct(set);
        require(up::hashset_validate(set));
        require(up::hashset_is_empty(set));
        require(up::hashset_size(set) == 0);
        require(up::hashset_num_buckets(set) == 0);
        require(up::hashset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(construct_destruct) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        up::hashset<unsigned int> set;
        int retval;

        retval = up::hashset_construct(set, nullptr, 16);
        require(retval == up::hash_badallocator);

        retval = up::hashset_construct(set, alloc, SIZE_MAX / 2);
        require(retval == up::hash_badlength);

        retval = up::hashset_construct(set, alloc, 14);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require(up::hashset_is_empty(set));
        require(up::hashset_size(set) == 0);
        require(up::hashset_num_buckets(set) == 16);
        require(up::hashset_memory_footprint(set) >= (ssize_t)(16 * sizeof(unsigned int)));

        retval = up::hashset_destruct(set, nullptr);
        require(retval == up::hash_badallocator);

        retval = up::hashset_destruct(set, alloc);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require(up::hashset_num_buckets(set) == 0);
        require(up::hashset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(insert_and_find) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        up::hashset<unsigned int> set;
        up::hashresult<unsigned int> result;
        up::hashrecord<unsigned int> record;
        size_t index2, index34;
        int retval;

        retval = up::hashset_construct(set, alloc, 32);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require(up::hashset_num_buckets(set) == 32);

        result = up::hashset_insert(set, 2);
        require(result.success && result.record.valid() && (result.record.key() == 2));
        require(up::hashset_validate(set) && (up::hashset_size(set) == 1) && !up::hashset_is_empty(set));

        record = up::hashset_find(set, 34);
        require(!record.valid());

        result = up::hashset_insert(set, 2);
        require(!result.success && result.record.valid() && (result.record.key() == 2));
        require(up::hashset_validate(set) && (up::hashset_size(set) == 1) && !up::hashset_is_empty(set));
        index2 = result.record.index();

        result = up::hashset_insert(set, 34);
        require(result.success && result.record.valid() && (result.record.key() == 34));
        require(up::hashset_validate(set) && (up::hashset_size(set) == 2) && !up::hashset_is_empty(set));
        index34 = result.record.index();

        record = up::hashset_find(set, 2);
        require(record.valid() && (record.key() == 2) && (record.index() == index2));
        
        record = up::hashset_find(set, 34);
        require(record.valid() && (record.key() == 34) && (record.index() == index34));
        
        record = up::hashset_find(set, 7);
        require(!record.valid());

        retval = up::hashset_destruct(set, alloc);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require(up::hashset_memory_footprint(set) == 0);
    }

    UP_TEST_CASE(insert_erase) {
        up::allocator* const alloc = up::malloc_allocator::instance();
        up::hashset<unsigned int> set;
        up::hashresult<unsigned int> result;
        up::hashrecord<unsigned int> record;
        size_t count;
        int retval, i;

        retval = up::hashset_construct(set, alloc, 10000);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require((up::hashset_num_buckets(set) == 16384));

        for (i = 0; i < 10000; ++i) {
            result = up::hashset_insert(set, i);
            require(result.success && result.record.valid());
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::hashset_erase(set, i);
            require(count == 1);
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == 5000);

        for (i = 0; i < 10000; i += 2) {
            record = up::hashset_find(set, i);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i)));
            count = up::hashset_erase(set, record);
            require(count == 1);
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == 0);

        for (i = 0; i < 10000; ++i) {
            record = up::hashset_find(set, i);
            require(!record.valid());
        }

        retval = up::hashset_destruct(set, alloc);
        require(retval == up::hash_success);
    }

    UP_TEST_CASE(multi_insert_erase) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashset<unsigned int> set;
        up::hashrecord<unsigned int> record;
        size_t count;
        int retval, i;

        retval = up::hashset_construct(set, alloc, 25600);
        require(retval == up::hash_success);
        require(up::hashset_validate(set));
        require(up::hashset_num_buckets(set) == 32768);

        for (i = 0; i < 25600; ++i) {
            record = up::hashset_multi_insert(set, i & 255);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i & 255)));
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == 25600);

        for (i = 0; i < 256; ++i) {
            count = up::hashset_erase(set, i);
            require(count == 1);
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == (25600 - 256));

        for (i = 0; i < 256; ++i) {
            record = up::hashset_find(set, i);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i)));
            count = up::hashset_multi_erase(set, i);
            require(count == ((25600 / 256) - 1));
        }

        require(up::hashset_validate(set));
        require(up::hashset_size(set) == 0);

        for (i = 0; i < 256; ++i) {
            record = up::hashset_find(set, i);
            require(!record.valid());
        }

        retval = up::hashset_destruct(set, alloc);
        require(retval == up::hash_success);
    }

    UP_TEST_CASE(grow_and_shrink) {
        up::allocator* const alloc = ::up::malloc_allocator::instance();
        up::hashset<unsigned int> set;
        up::hashresult<unsigned int> result;
        up::hashrecord<unsigned int> record;
        size_t count;
        int retval, i;

        up::hashset_construct(set);
        require(up::hashset_validate(set));
        require(up::hashset_num_buckets(set) == 0);

        for (i = 0; i < 10000; ++i) {
            if (up::hashset_is_overloaded(set, 0.75)) {
                retval = up::hashset_grow(set, alloc);
                require(retval == up::hash_success);
            }

            result = up::hashset_insert(set, i);
            require(result.success && result.record.valid());
        }

        require(up::hashset_validate(set));
        require(up::hashset_num_buckets(set) >= 10000);

        for (i = 1; i < 10000; i += 2) {
            count = up::hashset_erase(set, i);
            require(count == 1);
        }

        up::hashset_shrink_to_fit(set, alloc);
        require(up::hashset_validate(set));

        for (i = 0; i < 10000; i += 2) {
            record = up::hashset_find(set, i);
            require(record.valid() && (record.key() == static_cast<unsigned int>(i)));
        }

        retval = up::hashset_destruct(set, alloc);
        require(retval == up::hash_success);
    }
}
