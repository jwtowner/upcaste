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

#ifndef UP_HASHSET_HPP
#define UP_HASHSET_HPP

#include <up/detail/hash.inl>

namespace up
{   
    template <class Key>
    struct UPVISIBLE hashset
    {
        static_assert(is_unsigned<Key>::value, "Key parameter must be an unsigned integer type");
        typedef Key key_type;
        typedef hashtraits<Key> traits_type;
        Key* keys;
        size_t capacity;
        size_t used;
        size_t size;
    };

    template <class Key>
    UPVISIBLE
    bool hashset_validate(hashset<Key> const& set) noexcept {
        Key const* const keys = set.keys;
        size_t const capacity = set.capacity;
        size_t index, key, size_count, use_count;

        if ( (capacity && !keys) || (!capacity && keys)
            || (capacity < set.used) || (set.used < set.size)
            || ((capacity & (capacity - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, size_count = 0, use_count = 0; index < capacity; ++index) {
            key = keys[index];
            if (key < hashtraits<Key>::deleted) {
                ++size_count;
                ++use_count;
            }
            else if (key == hashtraits<Key>::deleted) {
                ++use_count;
            }
        }

        if ((size_count != set.size) || (use_count != set.used)) {
            return false;
        }

        return true;
    }

    template <class Key>
    UPVISIBLE
    ssize_t hashset_memory_footprint(size_t capacity) noexcept {
        if (!capacity) {
            return 0;
        }
        else if ( ((capacity & (capacity - 1)) != 0)
            || (capacity > (SIZE_MAX / sizeof(Key)))
        ) {
            return -1;
        }

        size_t const total_size = sizeof(Key) * capacity;
        return static_cast<ssize_t>(total_size);
    }

    template <class Key>
    inline UPALWAYSINLINE
    ssize_t hashset_memory_footprint(hashset<Key> const& set) noexcept {
        return ::up::hashset_memory_footprint<Key>(set.capacity);
    }

    template <class Key>
    inline UPALWAYSINLINE
    bool hashset_is_overloaded(hashset<Key> const& set, double load_factor) noexcept {
        if (!set.capacity) {
            return true;
        }
        return (static_cast<double>(set.used) / static_cast<double>(set.capacity)) > load_factor;
    }

    template <class Key>
    inline UPALWAYSINLINE
    bool hashset_is_empty(hashset<Key> const& set) noexcept {
        return !set.size;
    }

    template <class Key>
    inline UPALWAYSINLINE
    size_t hashset_size(hashset<Key> const& set) noexcept {
        return set.size;
    }

    template <class Key>
    inline UPALWAYSINLINE
    size_t hashset_num_buckets(hashset<Key> const& set) noexcept {
        return set.capacity;
    }
    
    template <class Key>
    UPVISIBLE
    void hashset_clear(hashset<Key>& set) noexcept {
        Key* const keys = set.keys;
        size_t index, remaining = set.size;
        
        if (remaining) {
            set->size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<Key>::deleted) {
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (set.used) {
            set.used = 0;
            ::up::uninitialized_fill_n(keys, set->capacity, hashtraits<Key>::vacant);
        }
    }

    template <class Key>
    UPVISIBLE
    void hashset_swap(hashset<Key>& map1, hashset<Key>& map2) noexcept {
        if (&map1 != &map2) {
            ::up::swap(map1.keys, map2.keys);
            ::up::swap(map1.capacity, map2.capacity);
            ::up::swap(map1.used, map2.used);
            ::up::swap(map1.size, map2.size);
        }
    }

    template <class Key>
    UPVISIBLE
    void hashset_construct(hashset<Key>& set) noexcept {
        set.keys = nullptr;
        set.capacity = 0;
        set.used = 0;
        set.size = 0;
    }

    template <class Key>
    UPVISIBLE
    int hashset_construct(hashset<Key>& set, allocator* alloc, size_t capacity) {
        if (!alloc) {
            return hash_badallocator;
        }

        size_t const capacity2 = ::up::ceil2(capacity);
        ssize_t const alloc_capacity = ::up::hashset_memory_footprint<Key>(capacity2);
        if (alloc_capacity < 0) {
            return hash_badlength;
        }

        Key* keys = nullptr;
        if (alloc_capacity > 0) {
            keys = static_cast<Key*>(::up::allocate(alloc, alloc_capacity));
            if (!keys) {
                return hash_nomem;
            }
            ::up::uninitialized_fill_n(keys, capacity2, hashtraits<Key>::vacant);
        }
        
        set.keys = keys;
        set.capacity = capacity2;
        set.used = 0;
        set.size = 0;
        return hash_success;
    }

    template <class Key>
    UPVISIBLE
    int hashset_destruct(hashset<Key>& set, allocator* alloc) {
        if (!alloc) {
            return hash_badallocator;
        }

        Key* const keys = set.keys;
        size_t const capacity = set.capacity;
        size_t index, remaining = set.size;
        
        if (remaining) {
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<Key>::deleted) {
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (capacity) {
            ::up::deallocate(alloc, keys, sizeof(Key) * capacity);
            set.keys = nullptr;
            set.capacity = 0;
            set.used = 0;
            set.size = 0;
        }

        return hash_success;
    }

    template <class Key, class Function>
    inline UPHIDDENINLINE
    void hashset_foreach(hashset<Key> const& set, Function func) noexcept {
        Key const* const keys = set.keys;
        size_t index, remaining = set.size;
        hashrecord<Key> record;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<Key>::deleted) {
                    record.key_ptr = &keys[index];
                    record.key_index = index;
                    func(record);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }
    }

    template <class Key>
    UPVISIBLE
    hashrecord<Key> hashset_find(hashset<Key> const& set, typename hashset<Key>::key_type const key) {
        hashrecord<Key> result = { nullptr, SIZE_MAX };
        if ((key >= hashtraits<Key>::deleted) || !set.size) {
            return result;
        }

        Key const* const keys = set.keys;
        size_t const mask = set.capacity - 1;
        size_t index, offset, current_key;

        for (offset = 0; offset <= mask; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<Key>::vacant) {
                break;
            }
            else if (current_key == key) {
                result.key_ptr = &keys[index];
                result.key_index = index;
                break;
            }
        }

        return result;
    }

    template <class Key>
    UPVISIBLE
    size_t hashset_erase(hashset<Key>& set, hashrecord<Key> const& record) {
        if (!record.key_ptr || !set.size) {
            return 0;
        }

        size_t const index = record.key_ptr - set.keys;
        if ((index >= set.capacity) || (*record.key_ptr >= hashtraits<Key>::deleted)) {
            return 0;
        }

        --set.size;
        set.keys[index] = hashtraits<Key>::deleted;
        return 1;
    }

    template <class Key>
    inline UPHIDDENINLINE
    size_t hashset_erase(hashset<Key>& set, typename hashset<Key>::key_type const key) {
        hashrecord<Key> record = ::up::hashset_find(set, key);
        return ::up::hashset_erase(set, record);
    }

    template <class Key>
    UPVISIBLE
    size_t hashset_multi_erase(hashset<Key>& set, typename hashset<Key>::key_type const key) {
        if (!set.size) {
            return 0;
        }

        Key* const keys = set.keys;
        size_t const mask = set.capacity - 1;
        size_t index, offset, current_key, count;

        for (offset = 0, count = 0; offset <= mask; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<Key>::vacant) {
                break;
            }
            else if (current_key == key) {
                keys[index] = hashtraits<Key>::deleted;
                ++count;
            }
        }
        
        set.size -= count;
        return count;
    }

    template <class Key>
    UPVISIBLE
    hashresult<Key> hashset_insert(hashset<Key>& set, typename hashset<Key>::key_type const key) {
        hashresult<Key> result = { { nullptr, SIZE_MAX }, false };
        if ((key >= hashtraits<Key>::deleted) || (set.size >= set.capacity)) {
            return result;
        }

        Key* const keys = set.keys;
        size_t const mask = set.capacity - 1;
        size_t index, offset, current_key;
        size_t index2, offset2, current_key2;

        for (offset = 0; ; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<Key>::deleted) {
                for (offset2 = offset + 1; ; ++offset2) {
                    index2 = (key + offset2) & mask;
                    current_key2 = keys[index2];
                    if (current_key2 == hashtraits<Key>::vacant) {
                        break;
                    }
                    else if (current_key2 == key) {
                        index = index2;
                        goto yield;
                    }
                }
                break;
            }
            else if (current_key == hashtraits<Key>::vacant) {
                ++set.used;
                break;
            }
            else if (current_key == key) {
                goto yield;
            }
        }

        ++set.size;
        keys[index] = key;
        result.success = true;

    yield:
        
        result.record.key_ptr = &keys[index];
        result.record.key_index = index;
        return result;
    }

    template <class Key>
    UPVISIBLE
    hashrecord<Key> hashset_multi_insert(hashset<Key>& set, typename hashset<Key>::key_type const key) {
        hashrecord<Key> result;
        if ((key >= hashtraits<Key>::deleted) || (set.size >= set.capacity)) {
            result.key_ptr = nullptr;
            result.key_index = 0;
            return result;
        }

        Key* const keys = set.keys;
        size_t const mask = set.capacity - 1;
        size_t index, offset, current_key;
        
        for (offset = 0; ; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key >= hashtraits<Key>::deleted) {
                set.used += (current_key & 1);
                current_key = hashtraits<Key>::vacant;
                break;
            }
        }

        ++set.size;
        keys[index] = key;
        result.key_ptr = &keys[index];
        result.key_index = index;
        return result;
    }

    template <class Key>
    UPVISIBLE
    void hashset_move(hashset<Key>& dst, hashset<Key>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::hashset_swap(dst, src);
            return;
        }

        Key* const keys = src.keys;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<Key>::deleted) {
                    ::up::hashset_insert(dst, keys[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(keys, src.capacity, hashtraits<Key>::vacant);
        }
    }

    template <class Key>
    UPVISIBLE
    void hashset_multi_move(hashset<Key>& dst, hashset<Key>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::hashset_swap(dst, src);
            return;
        }

        Key* const keys = src.keys;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<Key>::deleted) {
                    ::up::hashset_multi_insert(dst, keys[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(keys, src.capacity, hashtraits<Key>::vacant);
        }
    }

    template <class Key>
    UPVISIBLE
    int hashset_reserve(hashset<Key>& set, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < set.size) || (capacity == set.capacity)) {
            return hash_success;
        }

        hashset<Key> temp;
        int retval = ::up::hashset_construct(temp, alloc, capacity);
        if (retval != hash_success) {
            return retval;
        }

        ::up::hashset_move(temp, set);
        ::up::hashset_swap(temp, set);
        return ::up::hashset_destruct(temp, alloc);
    }

    template <class Key>
    UPVISIBLE
    int hashset_multi_reserve(hashset<Key>& set, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < set.size) || (capacity == set.capacity)) {
            return hash_success;
        }

        hashset<Key> temp;
        int retval = ::up::hashset_construct(temp, alloc, capacity);
        if (retval != hash_success) {
            return retval;
        }

        ::up::hashset_multi_move(temp, set);
        ::up::hashset_swap(temp, set);
        return ::up::hashset_destruct(temp, alloc);
    }

    template <class Key>
    inline UPHIDDENINLINE
    int hashset_grow(hashset<Key>& set, allocator* alloc) noexcept {
        return ::up::hashset_reserve(set, alloc, set.size ? (set.size << 1) : 8);
    }

    template <class Key>
    inline UPHIDDENINLINE
    int hashset_multi_grow(hashset<Key>& set, allocator* alloc) noexcept {
        return ::up::hashset_multi_reserve(set, alloc, set.size ? (set.size << 1) : 8);
    }

    template <class Key>
    inline UPHIDDENINLINE
    int hashset_shrink_to_fit(hashset<Key>& set, allocator* alloc) noexcept {
        return ::up::hashset_reserve(set, alloc, set.size);
    }

    template <class Key>
    inline UPHIDDENINLINE
    int hashset_multi_shrink_to_fit(hashset<Key>& set, allocator* alloc) noexcept {
        return ::up::hashset_multi_reserve(set, alloc, set.size);
    }
}

#endif
