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

#ifndef UP_DENSESET_HPP
#define UP_DENSESET_HPP

#include <up/detail/dense.inl>

namespace up
{   
    template <class Record, class Hasher, class Equals>
    struct UPVISIBLE denseset
    {
        typedef Record record_type;
        typedef Hasher hasher_type;
        typedef Equals equals_type;
        compressed_pair<Record*, compressed_pair<Hasher, Equals> > records_and_functors;
        size_t* hashcodes;
        size_t capacity;
        size_t used;
        size_t size;
        UPALWAYSINLINE Record* records() const noexcept { return records_and_functors.first(); }
        UPALWAYSINLINE Hasher const& hasher() const noexcept { return records_and_functors.second().first(); }
        UPALWAYSINLINE Equals const& equals() const noexcept { return records_and_functors.second().second(); }
    };

    template <class R, class H, class E>
    UPVISIBLE
    bool denseset_validate(denseset<R, H, E> const& set) noexcept {
        H const& hasher = set.hasher();
        R* const records = set.records();
        size_t const* const hashcodes = set.hashcodes;
        size_t const capacity = set.capacity;
        size_t index, hashcode, size_count, use_count;

        if ( (capacity && !hashcodes) || (!capacity && hashcodes)
            || (hashcodes && !records) || (!hashcodes && records)
            || (capacity < set.used) || (set.used < set.size)
            || ((capacity & (capacity - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, size_count = 0, use_count = 0; index < capacity; ++index) {
            hashcode = hashcodes[index];
            if (hashcode < dense_deleted) {
                if (::up::dense_resolve(hasher(records[index])) != hashcode) {
                    return false;
                }
                ++size_count;
                ++use_count;
            }
            else if (hashcode == dense_deleted) {
                ++use_count;
            }
        }

        if ((size_count != set.size) || (use_count != set.used)) {
            return false;
        }

        return true;
    }

    template <class Record>
    UPVISIBLE
    ssize_t denseset_memory_footprint(size_t capacity) noexcept {
        constexpr size_t pad_size = sizeof(Record);
        constexpr size_t node_size = sizeof(size_t) + pad_size;

        if (!capacity) {
            return 0;
        }
        else if (((capacity & (capacity - 1)) != 0) || (capacity > (SIZE_MAX / node_size))) {
            return -1;
        }

        size_t const total_size = node_size * capacity;
        if (addition_overflow(total_size, pad_size)) {
            return -1;
        }

        return static_cast<ssize_t>(total_size + pad_size);
    }

    template <class R, class H, class E>
    inline UPALWAYSINLINE
    ssize_t denseset_memory_footprint(denseset<R, H, E> const& set) noexcept {
        return ::up::denseset_memory_footprint<R>(set.capacity);
    }

    template <class R, class H, class E>
    inline UPALWAYSINLINE
    bool denseset_is_overloaded(denseset<R, H, E> const& set, double load_factor) noexcept {
        return (set.capacity > 0) ? ((static_cast<double>(set.used) / static_cast<double>(set.capacity)) > load_factor) : true;
    }

    template <class R, class H, class E>
    inline UPALWAYSINLINE
    bool denseset_is_empty(denseset<R, H, E> const& set) noexcept {
        return !set.size;
    }

    template <class R, class H, class E>
    inline UPALWAYSINLINE
    size_t denseset_size(denseset<R, H, E> const& set) noexcept {
        return set.size;
    }

    template <class R, class H, class E>
    inline UPALWAYSINLINE
    size_t denseset_num_buckets(denseset<R, H, E> const& set) noexcept {
        return set.capacity;
    }
    
    template <class R, class H, class E>
    UPVISIBLE
    void denseset_clear(denseset<R, H, E>& set) noexcept {
        R* const records = set.records();
        size_t* const hashcodes = set.hashcodes;
        size_t index, remaining = set.size;
        
        if (remaining) {
            set->size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::destruct(&records[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (set.used) {
            set.used = 0;
            ::up::uninitialized_fill_n(hashcodes, set->capacity, dense_vacant);
        }
    }

    template <class R, class H, class E>
    UPVISIBLE
    void denseset_swap(denseset<R, H, E>& set1, denseset<R, H, E>& set2) noexcept {
        if (&set1 != &set2) {
            ::up::swap(set1.records_and_functors, set2.records_and_functors);
            ::up::swap(set1.hashcodes, set2.hashcodes);
            ::up::swap(set1.capacity, set2.capacity);
            ::up::swap(set1.used, set2.used);
            ::up::swap(set1.size, set2.size);
        }
    }

    template <class R, class H, class E>
    UPVISIBLE
    void denseset_construct(denseset<R, H, E>& set, H const& hasher = H(), E const& equals = E()) noexcept {
        set.records_and_functors.first(nullptr);
        set.records_and_functors.second().first(hasher);
        set.records_and_functors.second().second(equals);
        set.hashcodes = nullptr;
        set.capacity = 0;
        set.used = 0;
        set.size = 0;
    }

    template <class R, class H, class E>
    UPVISIBLE
    int denseset_construct(denseset<R, H, E>& set, allocator* alloc, size_t capacity, H const& hasher = H(), E const& equals = E()) {
        if (!alloc) {
            return dense_badallocator;
        }

        size_t const capacity2 = ::up::ceil2(capacity);
        ssize_t const alloc_capacity = ::up::denseset_memory_footprint<R>(capacity2);
        if (alloc_capacity < 0) {
            return dense_badlength;
        }

        R* records = nullptr;
        size_t* hashcodes = nullptr;

        if (alloc_capacity > 0) {
            size_t space = static_cast<size_t>(alloc_capacity);
            void* ptr = ::up::allocate(alloc, alloc_capacity);
            if (!ptr) {
                return dense_nomem;
            }

            hashcodes = static_cast<size_t*>(ptr);
            ::up::uninitialized_fill_n(hashcodes, capacity2, dense_vacant);

            ::up::align_advance(sizeof(size_t) * capacity2, &ptr, &space);
            records = static_cast<R*>(::up::align(alignof(R), sizeof(R) * capacity2, &ptr, &space));
        }
        
        set.records_and_functors.first(records);
        set.records_and_functors.second().first(hasher);
        set.records_and_functors.second().second(equals);
        set.hashcodes = hashcodes;
        set.capacity = capacity2;
        set.used = 0;
        set.size = 0;
        return dense_success;
    }

    template <class R, class H, class E>
    UPVISIBLE
    int denseset_destruct(denseset<R, H, E>& set, allocator* alloc) {
        if (!alloc) {
            return dense_badallocator;
        }

        R* const records = set.records();
        size_t* const hashcodes = set.hashcodes;
        size_t const capacity = set.capacity;
        size_t index, remaining = set.size;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::destruct(&records[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (capacity) {
            size_t const pad_size = sizeof(R);
            size_t const node_size = sizeof(size_t) + pad_size;
            size_t const buffer_size = ((capacity > 32) ? (node_size *capacity) : (node_size * 32)) + pad_size;
            ::up::deallocate(alloc, hashcodes, buffer_size);
            set.records_and_functors.first(nullptr);
            set.hashcodes = nullptr;
            set.capacity = 0;
            set.used = 0;
            set.size = 0;
        }

        return dense_success;
    }

    template <class R, class H, class E, class Function>
    inline UPHIDDENINLINE
    void denseset_foreach(denseset<R, H, E> const& set, Function func) noexcept {
        R* const records = set.records();
        size_t const* const hashcodes = set.hashcodes;
        size_t index, remaining = set.size;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    func(&records[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }
    }

    template <class R, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    R* denseset_find(denseset<R, H, E> const& set, Key const& key, Hasher const& hasher, Equals const& equals) {
        R* result = nullptr;
        if (!set.size) {
            return result;
        }

        R* const records = set.records();
        size_t const* const hashcodes = set.hashcodes;
        size_t const mask = set.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode;

        for (offset = 0; offset <= mask; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode == dense_vacant) {
                break;
            }
            else if ((current_hashcode == hashcode) && equals(records[index], key)) {
                result = &records[index];
                break;
            }
        }

        return result;
    }

    template <class R, class H, class E, class Key>
    inline UPHIDDENINLINE
    R* denseset_find(denseset<R, H, E> const& set, Key const& key) {
        return ::up::denseset_find(set, key, set.hasher(), set.equals());
    }

    template <class R, class H, class E>
    UPVISIBLE
    size_t denseset_erase(denseset<R, H, E>& set, R const* record) {
        if (!record || !set.size) {
            return 0;
        }

        size_t const index = record - set.records();
        if ((index >= set.capacity) || (set.hashcodes[index] >= dense_deleted)) {
            return 0;
        }

        --set.size;
        set.hashcodes[index] = dense_deleted;
        record->~R();
        return 1;
    }

    template <class R, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t denseset_erase(denseset<R, H, E>& set, Key const& key, Hasher const& hasher, Equals const& equals) {
        R* record = ::up::denseset_find(set, key, hasher, equals);
        return ::up::denseset_erase(set, record);
    }

    template <class R, class H, class E, class Key>
    inline UPHIDDENINLINE
    typename enable_if<!is_same<R*, Key>::value && !is_same<R const*, Key>::value, size_t>::type
    denseset_erase(denseset<R, H, E>& set, Key const& key) {
        return ::up::denseset_erase(set, key, set.hasher(), set.equals());
    }

    template <class R, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    size_t denseset_multi_erase(denseset<R, H, E>& set, Key const& key, Hasher const& hasher, Equals const& equals) {
        if (!set.size) {
            return 0;
        }

        R* const records = set.records();
        size_t * const hashcodes = set.hashcodes;
        size_t const mask = set.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode, count;

        for (offset = 0, count = 0; offset <= mask; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode == dense_vacant) {
                break;
            }
            else if ((current_hashcode == hashcode) && equals(records[index], key)) {
                hashcodes[index] = dense_deleted;
                (&records[index])->~R();
                ++count;
            }
        }
        
        set.size -= count;
        return count;
    }

    template <class R, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t denseset_multi_erase(denseset<R, H, E>& set, Key const& key) {
        return ::up::denseset_multi_erase(set, key, set.hasher(), set.equals());
    }

    template <class R, class H, class E, class Key>
    UPVISIBLE
    denseresult<R> denseset_prepare(denseset<R, H, E>& set, Key const& key) {
        denseresult<R> result = { nullptr, false };
        if (set.size >= set.capacity) {
            return result;
        }

        H const& hasher = set.hasher();
        E const& equals = set.equals();
        R* const records = set.records();
        size_t* const hashcodes = set.hashcodes;
        size_t const mask = set.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode;
        size_t index2, offset2, current_hashcode2;

        for (offset = 0; ; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode == dense_deleted) {
                for (offset2 = offset + 1; ; ++offset2) {
                    index2 = (hashcode + offset2) & mask;
                    current_hashcode2 = hashcodes[index2];
                    if (current_hashcode2 == dense_vacant) {
                        break;
                    }
                    else if ((current_hashcode2 == hashcode) && equals(records[index2], key)) {
                        index = index2;
                        goto yield;
                    }
                }
                break;
            }
            else if (current_hashcode == dense_vacant) {
                ++set.used;
                break;
            }
            else if ((current_hashcode == hashcode) && equals(records[index], key)) {
                goto yield;
            }
        }

        ++set.size;
        hashcodes[index] = hashcode;
        result.success = true;

    yield:
        
        result.record = &records[index];
        return result;
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    denseresult<R> denseset_insert(denseset<R, H, E>& set, R const& record) {
        denseresult<R> result = ::up::denseset_prepare(set, record);
        if (result.success) {
            ::new(result.record) R(record);
        }
        return result;
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    denseresult<R> denseset_insert(denseset<R, H, E>& set, R&& record) {
        denseresult<R> result = ::up::denseset_prepare(set, record);
        if (result.success) {
            ::new(result.record) R(::up::move(record));
        }
        return result;
    }

    template <class R, class H, class E, class Key>
    UPVISIBLE
    R* denseset_multi_prepare(denseset<R, H, E>& set, Key const& key) {
        if (set.size >= set.capacity) {
            return nullptr;
        }

        H const& hasher = set.hasher();
        R* const records = set.records();
        size_t* const hashcodes = set.hashcodes;
        size_t const mask = set.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode;
        
        for (offset = 0; ; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode >= dense_deleted) {
                set.used += (current_hashcode & 1);
                current_hashcode = dense_vacant;
                break;
            }
        }

        ++set.size;
        hashcodes[index] = hashcode;
        return &records[index];
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    R* denseset_multi_insert(denseset<R, H, E>& set, R const& record) {
        R* result = ::up::denseset_multi_prepare(set, record);
        if (result) {
            ::new(result) R(record);
        }
        return result;
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    R* denseset_multi_insert(denseset<R, H, E>& set, R&& record) {
        R* result = ::up::denseset_multi_prepare(set, record);
        if (result) {
            ::new(result) R(::up::move(record));
        }
        return result;
    }

    template <class R, class H, class E>
    UPVISIBLE
    void denseset_move(denseset<R, H, E>& dst, denseset<R, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::denseset_swap(dst, src);
            return;
        }

        R* const records = src.records();
        size_t* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::denseset_insert(dst, ::up::move(records[index]));
                    ::up::destruct(&records[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(hashcodes, src.capacity, dense_vacant);
        }
    }

    template <class R, class H, class E>
    UPVISIBLE
    void denseset_multi_move(denseset<R, H, E>& dst, denseset<R, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::denseset_swap(dst, src);
            return;
        }

        R* const records = src.records();
        size_t* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::denseset_multi_insert(dst, ::up::move(records[index]));
                    ::up::destruct(&records[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(hashcodes, src.capacity, dense_vacant);
        }
    }

    template <class R, class H, class E>
    UPVISIBLE
    int denseset_reserve(denseset<R, H, E>& set, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < set.size) || (capacity == set.capacity)) {
            return dense_success;
        }

        denseset<R, H, E> temp;
        int retval = ::up::denseset_construct(temp, alloc, capacity, set.hasher(), set.equals());
        if (retval != dense_success) {
            return retval;
        }

        ::up::denseset_move(temp, set);
        ::up::denseset_swap(temp, set);
        return ::up::denseset_destruct(temp, alloc);
    }

    template <class R, class H, class E>
    UPVISIBLE
    int denseset_multi_reserve(denseset<R, H, E>& set, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < set.size) || (capacity == set.capacity)) {
            return dense_success;
        }

        denseset<R, H, E> temp;
        int retval = ::up::denseset_construct(temp, alloc, capacity, set.hasher(), set.equals());
        if (retval != dense_success) {
            return retval;
        }

        ::up::denseset_multi_move(temp, set);
        ::up::denseset_swap(temp, set);
        return ::up::denseset_destruct(temp, alloc);
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    int denseset_grow(denseset<R, H, E>& set, allocator* alloc) noexcept {
        return ::up::denseset_reserve(set, alloc, set.size ? (set.size << 1) : 8);
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    int denseset_multi_grow(denseset<R, H, E>& set, allocator* alloc) noexcept {
        return ::up::denseset_multi_reserve(set, alloc, set.size ? (set.size << 1) : 8);
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    int denseset_shrink_to_fit(denseset<R, H, E>& set, allocator* alloc) noexcept {
        return ::up::denseset_reserve(set, alloc, set.size);
    }

    template <class R, class H, class E>
    inline UPHIDDENINLINE
    int denseset_multi_shrink_to_fit(denseset<R, H, E>& set, allocator* alloc) noexcept {
        return ::up::denseset_multi_reserve(set, alloc, set.size);
    }
}

#endif
