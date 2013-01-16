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

#ifndef UP_HASHMAP_HPP
#define UP_HASHMAP_HPP

#include <up/detail/hash.inl>

namespace up
{   
    template <class Key, class Value>
    struct UPVISIBLE hashmap
    {
        static_assert(is_unsigned<Key>::value, "Key parameter must be an unsigned integer type");
        typedef Key key_type;
        typedef Value value_type;
        typedef hashtraits<Key> traits_type;
        Key* keys;
        Value* values;
        size_t capacity;
        size_t used;
        size_t size;
    };

    template <class K, class V>
    UPVISIBLE
    bool hashmap_validate(hashmap<K, V> const& map) noexcept {
        K const* const keys = map.keys;
        size_t const capacity = map.capacity;
        size_t index, key, size_count, use_count;

        if ( (capacity && !keys) || (!capacity && keys)
            || (keys && !map.values) || (!keys && map.values)
            || (capacity < map.used) || (map.used < map.size)
            || ((capacity & (capacity - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, size_count = 0, use_count = 0; index < capacity; ++index) {
            key = keys[index];
            if (key < hashtraits<K>::deleted) {
                ++size_count;
                ++use_count;
            }
            else if (key == hashtraits<K>::deleted) {
                ++use_count;
            }
        }

        if ((size_count != map.size) || (use_count != map.used)) {
            return false;
        }

        return true;
    }

    template <class Key, class Value>
    UPVISIBLE
    ssize_t hashmap_memory_footprint(size_t capacity) noexcept {
        constexpr size_t node_size = sizeof(Key) + sizeof(Value);

        if (!capacity) {
            return 0;
        }
        else if ( ((capacity & (capacity - 1)) != 0)
            || (capacity > (SIZE_MAX / node_size))
        ) {
            return -1;
        }

        size_t const total_size = node_size * capacity;
        if (addition_overflow(total_size, sizeof(Value))) {
            return -1;
        }

        return static_cast<ssize_t>(total_size + sizeof(Value));
    }

    template <class K, class V>
    inline UPALWAYSINLINE
    ssize_t hashmap_memory_footprint(hashmap<K, V> const& map) noexcept {
        return ::up::hashmap_memory_footprint<K, V>(map.capacity);
    }

    template <class K, class V>
    inline UPALWAYSINLINE
    bool hashmap_is_overloaded(hashmap<K, V> const& map, double load_factor) noexcept {
        if (!map.capacity) {
            return true;
        }
        return (static_cast<double>(map.used) / static_cast<double>(map.capacity)) > load_factor;
    }

    template <class K, class V>
    inline UPALWAYSINLINE
    bool hashmap_is_empty(hashmap<K, V> const& map) noexcept {
        return !map.size;
    }

    template <class K, class V>
    inline UPALWAYSINLINE
    size_t hashmap_size(hashmap<K, V> const& map) noexcept {
        return map.size;
    }

    template <class K, class V>
    inline UPALWAYSINLINE
    size_t hashmap_num_buckets(hashmap<K, V> const& map) noexcept {
        return map.capacity;
    }
    
    template <class K, class V>
    UPVISIBLE
    void hashmap_clear(hashmap<K, V>& map) noexcept {
        K* const keys = map.keys;
        V* const values = map.values;
        size_t index, remaining = map.size;
        
        if (remaining) {
            map->size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<K>::deleted) {
                    ::up::destruct(&values[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (map.used) {
            map.used = 0;
            ::up::uninitialized_fill_n(keys, map->capacity, hashtraits<K>::vacant);
        }
    }

    template <class K, class V>
    UPVISIBLE
    void hashmap_swap(hashmap<K, V>& map1, hashmap<K, V>& map2) noexcept {
        if (&map1 != &map2) {
            ::up::swap(map1.keys, map2.keys);
            ::up::swap(map1.values, map2.values);
            ::up::swap(map1.capacity, map2.capacity);
            ::up::swap(map1.used, map2.used);
            ::up::swap(map1.size, map2.size);
        }
    }

    template <class K, class V>
    UPVISIBLE
    void hashmap_construct(hashmap<K, V>& map) noexcept {
        map.keys = nullptr;
        map.values = nullptr;
        map.capacity = 0;
        map.used = 0;
        map.size = 0;
    }

    template <class K, class V>
    UPVISIBLE
    int hashmap_construct(hashmap<K, V>& map, allocator* alloc, size_t capacity) {
        if (!alloc) {
            return hash_badallocator;
        }

        size_t const capacity2 = ::up::ceil2(capacity);
        ssize_t const alloc_capacity = ::up::hashmap_memory_footprint<K, V>(capacity2);
        if (alloc_capacity < 0) {
            return hash_badlength;
        }

        K* keys = nullptr;
        V* values = nullptr;
        
        if (alloc_capacity > 0) {
            size_t space = static_cast<size_t>(alloc_capacity);
            void* ptr = ::up::allocate(alloc, alloc_capacity);
            if (!ptr) {
                return hash_nomem;
            }

            keys = static_cast<K*>(ptr);
            ::up::uninitialized_fill_n(keys, capacity2, hashtraits<K>::vacant);

            ::up::align_advance(sizeof(K) * capacity2, &ptr, &space);
            values = static_cast<V*>(::up::align(alignof(V), sizeof(V) * capacity2, &ptr, &space));
        }
        
        map.keys = keys;
        map.values = values;
        map.capacity = capacity2;
        map.used = 0;
        map.size = 0;
        return hash_success;
    }

    template <class K, class V>
    UPVISIBLE
    int hashmap_destruct(hashmap<K, V>& map, allocator* alloc) {
        if (!alloc) {
            return hash_badallocator;
        }

        K* const keys = map.keys;
        V* const values = map.values;
        size_t const capacity = map.capacity;
        size_t index, remaining = map.size;
        
        if (remaining) {
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<K>::deleted) {
                    ::up::destruct(&values[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (capacity) {
            size_t const buffer_size = ((sizeof(K) + sizeof(V)) * capacity) + sizeof(V);
            ::up::deallocate(alloc, keys, buffer_size);
            map.keys = nullptr;
            map.values = nullptr;
            map.capacity = 0;
            map.used = 0;
            map.size = 0;
        }

        return hash_success;
    }

    template <class K, class V, class Function>
    inline UPHIDDENINLINE
    void hashmap_foreach(hashmap<K, V> const& map, Function func) noexcept {
        K const* const keys = map.keys;
        V* const values = map.values;
        size_t index, remaining = map.size;
        hashrecord<K, V> record;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<K>::deleted) {
                    record.key_ptr = &keys[index];
                    record.value_ptr = &values[index];
                    func(record);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }
    }

    template <class K, class V>
    UPVISIBLE
    hashrecord<K, V> hashmap_find(hashmap<K, V> const& map, typename hashmap<K, V>::key_type const key) {
        hashrecord<K, V> result = { nullptr, nullptr };
        if ((key >= hashtraits<K>::deleted) || !map.size) {
            return result;
        }

        K const* const keys = map.keys;
        V* const values = map.values;
        size_t const mask = map.capacity - 1;
        size_t index, offset, current_key;

        for (offset = 0; offset <= mask; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<K>::vacant) {
                break;
            }
            else if (current_key == key) {
                result.key_ptr = &keys[index];
                result.value_ptr = &values[index];
                break;
            }
        }

        return result;
    }

    template <class K, class V>
    UPVISIBLE
    size_t hashmap_erase(hashmap<K, V>& map, hashrecord<K, V> const& record) {
        if (!record.key_ptr || !map.size) {
            return 0;
        }

        size_t const index = record.key_ptr - map.keys;
        if ((index >= map.capacity) || (*record.key_ptr >= hashtraits<K>::deleted)) {
            return 0;
        }

        --map.size;
        map.keys[index] = hashtraits<K>::deleted;
        ::up::destruct(record.value_ptr);
        return 1;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    size_t hashmap_erase(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key) {
        hashrecord<K, V> record = ::up::hashmap_find(map, key);
        return ::up::hashmap_erase(map, record);
    }

    template <class K, class V>
    UPVISIBLE
    size_t hashmap_multi_erase(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key) {
        if (!map.size) {
            return 0;
        }

        K* const keys = map.keys;
        V* const values = map.values;
        size_t const mask = map.capacity - 1;
        size_t index, offset, current_key, count;

        for (offset = 0, count = 0; offset <= mask; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<K>::vacant) {
                break;
            }
            else if (current_key == key) {
                keys[index] = hashtraits<K>::deleted;
                ::up::destruct(&values[index]);
                ++count;
            }
        }
        
        map.size -= count;
        return count;
    }

    template <class K, class V>
    UPVISIBLE
    hashresult<K, V> hashmap_prepare(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key) {
        hashresult<K, V> result = { { nullptr, nullptr }, false };
        if ((key >= hashtraits<K>::deleted) || (map.size >= map.capacity)) {
            return result;
        }

        K* const keys = map.keys;
        V* const values = map.values;
        size_t const mask = map.capacity - 1;
        size_t index, offset, current_key;
        size_t index2, offset2, current_key2;

        for (offset = 0; ; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key == hashtraits<K>::deleted) {
                for (offset2 = offset + 1; ; ++offset2) {
                    index2 = (key + offset2) & mask;
                    current_key2 = keys[index2];
                    if (current_key2 == hashtraits<K>::vacant) {
                        break;
                    }
                    else if (current_key2 == key) {
                        index = index2;
                        goto yield;
                    }
                }
                break;
            }
            else if (current_key == hashtraits<K>::vacant) {
                ++map.used;
                break;
            }
            else if (current_key == key) {
                goto yield;
            }
        }

        ++map.size;
        keys[index] = key;
        result.success = true;

    yield:
        
        result.record.key_ptr = &keys[index];
        result.record.value_ptr = &values[index];
        return result;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    hashresult<K, V> hashmap_insert(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V const& value) {
        hashresult<K, V> result = ::up::hashmap_prepare(map, key);
        if (result.success) {
            ::new(result.record.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    hashresult<K, V> hashmap_insert(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V&& value) {
        hashresult<K, V> result = ::up::hashmap_prepare(map, key);
        if (result.success) {
            ::new(result.record.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V>
    UPVISIBLE
    hashrecord<K, V> hashmap_multi_prepare(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key) {
        hashrecord<K, V> result;
        if ((key >= hashtraits<K>::deleted) || (map.size >= map.capacity)) {
            result.key_ptr = nullptr;
            result.value_ptr = nullptr;
            return result;
        }

        K* const keys = map.keys;
        V* const values = map.values;
        size_t const mask = map.capacity - 1;
        size_t index, offset, current_key;
        
        for (offset = 0; ; ++offset) {
            index = (key + offset) & mask;
            current_key = keys[index];
            if (current_key >= hashtraits<K>::deleted) {
                map.used += (current_key & 1);
                current_key = hashtraits<K>::vacant;
                break;
            }
        }

        ++map.size;
        keys[index] = key;
        result.key_ptr = &keys[index];
        result.value_ptr = &values[index];
        return result;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    hashrecord<K, V> hashmap_multi_insert(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V const& value) {
        hashrecord<K, V> result = ::up::hashmap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(result.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    hashrecord<K, V> hashmap_multi_insert(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V&& value) {
        hashrecord<K, V> result = ::up::hashmap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(result.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    typename conditional<is_fundamental<V>::value, V, V const&>::type
    hashmap_get(
        hashmap<K, V> const& map,
        typename hashmap<K, V>::key_type const key,
        typename conditional<is_fundamental<V>::value, V, V const&>::type default_value = V()
    ) {
        hashrecord<K, V> record = ::up::hashmap_find(map, key);
        return record.key_ptr ? *record.value_ptr : default_value;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_set(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V const& value) {
        hashresult<K, V> result = ::up::hashmap_prepare(map, key);
        if (!result.record.key_ptr) {
            return hash_nospace;
        }

        if (result.success) {
            ::new(result.record.value_ptr) V(value);
        }
        else {
            *result.record.value_ptr = value;
        }

        return hash_success;
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_set(hashmap<K, V>& map, typename hashmap<K, V>::key_type const key, V&& value) {
        hashresult<K, V> result = ::up::hashmap_prepare(map, key);
        if (!result.record.key_ptr) {
            return hash_nospace;
        }

        if (result.success) {
            ::new(result.record.value_ptr) V(::up::move(value));
        }
        else {
            *result.record.value_ptr = ::up::move(value);
        }

        return hash_success;
    }

    template <class K, class V>
    UPVISIBLE
    void hashmap_move(hashmap<K, V>& dst, hashmap<K, V>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::hashmap_swap(dst, src);
            return;
        }

        K* const keys = src.keys;
        V* const values = src.values;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<K>::deleted) {
                    ::up::hashmap_insert(dst, keys[index], ::up::move(values[index]));
                    ::up::destruct(&values[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(keys, src.capacity, hashtraits<K>::vacant);
        }
    }

    template <class K, class V>
    UPVISIBLE
    void hashmap_multi_move(hashmap<K, V>& dst, hashmap<K, V>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::hashmap_swap(dst, src);
            return;
        }

        K* const keys = src.keys;
        V* const values = src.values;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (keys[index] < hashtraits<K>::deleted) {
                    ::up::hashmap_multi_insert(dst, keys[index], ::up::move(values[index]));
                    ::up::destruct(&values[index]);
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (src.used) {
            src.used = 0;
            ::up::uninitialized_fill_n(keys, src.capacity, hashtraits<K>::vacant);
        }
    }

    template <class K, class V>
    UPVISIBLE
    int hashmap_reserve(hashmap<K, V>& map, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < map.size) || (capacity == map.capacity)) {
            return hash_success;
        }

        hashmap<K, V> temp;
        int retval = ::up::hashmap_construct(temp, alloc, capacity);
        if (retval != hash_success) {
            return retval;
        }

        ::up::hashmap_move(temp, map);
        ::up::hashmap_swap(temp, map);
        return ::up::hashmap_destruct(temp, alloc);
    }

    template <class K, class V>
    UPVISIBLE
    int hashmap_multi_reserve(hashmap<K, V>& map, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < map.size) || (capacity == map.capacity)) {
            return hash_success;
        }

        hashmap<K, V> temp;
        int retval = ::up::hashmap_construct(temp, alloc, capacity);
        if (retval != hash_success) {
            return retval;
        }

        ::up::hashmap_multi_move(temp, map);
        ::up::hashmap_swap(temp, map);
        return ::up::hashmap_destruct(temp, alloc);
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_grow(hashmap<K, V>& map, allocator* alloc) noexcept {
        return ::up::hashmap_reserve(map, alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_multi_grow(hashmap<K, V>& map, allocator* alloc) noexcept {
        return ::up::hashmap_multi_reserve(map, alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_shrink_to_fit(hashmap<K, V>& map, allocator* alloc) noexcept {
        return ::up::hashmap_reserve(map, alloc, map.size);
    }

    template <class K, class V>
    inline UPHIDDENINLINE
    int hashmap_multi_shrink_to_fit(hashmap<K, V>& map, allocator* alloc) noexcept {
        return ::up::hashmap_multi_reserve(map, alloc, map.size);
    }
}

#endif
