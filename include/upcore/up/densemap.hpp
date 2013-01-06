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

#ifndef UP_DENSEMAP_HPP
#define UP_DENSEMAP_HPP

#include <up/detail/dense.inl>

namespace up
{   
    template <class Key, class Value, class Hasher, class Equals>
    struct UPVISIBLE densemap
    {
        typedef Key key_type;
        typedef Value value_type;
        typedef Hasher hasher_type;
        typedef Equals equals_type;
        compressed_pair<Key const*, Hasher> keys_and_hasher;
        compressed_pair<Value*, Equals> values_and_equals;
        size_t* hashcodes;
        size_t capacity;
        size_t used;
        size_t size;
        UPALWAYSINLINE Key const* keys() const noexcept { return keys_and_hasher.first(); }
        UPALWAYSINLINE Value* values() const noexcept { return values_and_equals.first(); }
        UPALWAYSINLINE Hasher const& hasher() const noexcept { return keys_and_hasher.second(); }
        UPALWAYSINLINE Equals const& equals() const noexcept { return values_and_equals.second(); }
    };

    template <class K, class V, class H, class E>
    UPVISIBLE
    bool densemap_validate(densemap<K, V, H, E> const& map) noexcept {
        H const& hasher = map.hasher();
        K const* const keys = map.keys();
        size_t const* const hashcodes = map.hashcodes;
        size_t const capacity = map.capacity;
        size_t index, hashcode, size_count, use_count;

        if ( (capacity && !hashcodes) || (!capacity && hashcodes)
            || (hashcodes && !keys) || (!hashcodes && keys)
            || (hashcodes && !map.values()) || (!hashcodes && map.values())
            || (capacity < map.used) || (map.used < map.size)
            || ((capacity & (capacity - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, size_count = 0, use_count = 0; index < capacity; ++index) {
            hashcode = hashcodes[index];
            if (hashcode < dense_deleted) {
                if (::up::dense_resolve(hasher(keys[index])) != hashcode) {
                    return false;
                }
                ++size_count;
                ++use_count;
            }
            else if (hashcode == dense_deleted) {
                ++use_count;
            }
        }

        if ((size_count != map.size) || (use_count != map.used)) {
            return false;
        }

        return true;
    }

    template <class K, class V>
    UPVISIBLE
    ssize_t densemap_memory_footprint(size_t capacity) noexcept {
        size_t const pad_size = sizeof(K) + sizeof(V);
        size_t const node_size = sizeof(size_t) + pad_size;

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

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    ssize_t densemap_memory_footprint(densemap<K, V, H, E> const& map) noexcept {
        return ::up::densemap_memory_footprint<K, V>(map.capacity);
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    bool densemap_is_overloaded(densemap<K, V, H, E> const& map, double load_factor) noexcept {
        return (map.capacity > 0) ? ((static_cast<double>(map.used) / static_cast<double>(map.capacity)) > load_factor) : true;
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    bool densemap_is_empty(densemap<K, V, H, E> const& map) noexcept {
        return !map.size;
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    size_t densemap_size(densemap<K, V, H, E> const& map) noexcept {
        return map.size;
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    size_t densemap_num_buckets(densemap<K, V, H, E> const& map) noexcept {
        return map.capacity;
    }
    
    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_clear(densemap<K, V, H, E>& map) noexcept {
        K const* const keys = map.keys();
        V* const values = map.values();
        size_t* const hashcodes = map.hashcodes;
        size_t index, remaining = map.size;
        
        if (remaining) {
            map->size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    const_cast<V*>(&values[index])->~V();
                    const_cast<K*>(&keys[index])->~K();
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (map.used) {
            map.used = 0;
            ::up::uninitialized_fill_n(hashcodes, map->capacity, dense_vacant);
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_swap(densemap<K, V, H, E>& map1, densemap<K, V, H, E>& map2) noexcept {
        if (&map1 != &map2) {
            ::up::swap(map1.keys_and_hasher, map2.keys_and_hasher);
            ::up::swap(map1.values_and_equals, map2.values_and_equals);
            ::up::swap(map1.hashcodes, map2.hashcodes);
            ::up::swap(map1.capacity, map2.capacity);
            ::up::swap(map1.used, map2.used);
            ::up::swap(map1.size, map2.size);
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_construct(densemap<K, V, H, E>& map, H const& hasher = H(), E const& equals = E()) noexcept {
        map.keys_and_hasher.first(nullptr);
        map.keys_and_hasher.second(hasher);
        map.values_and_equals.first(nullptr);
        map.values_and_equals.second(equals);
        map.hashcodes = nullptr;
        map.capacity = 0;
        map.used = 0;
        map.size = 0;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int densemap_construct(densemap<K, V, H, E>& map, allocator* alloc, size_t capacity, H const& hasher = H(), E const& equals = E()) {
        if (!alloc) {
            return dense_badallocator;
        }

        size_t const capacity2 = ::up::ceil2(capacity);
        ssize_t const alloc_capacity = ::up::densemap_memory_footprint<K, V>(capacity2);
        if (alloc_capacity < 0) {
            return dense_badlength;
        }

        K const* keys = nullptr;
        V* values = nullptr;
        size_t* hashcodes = nullptr;

        if (alloc_capacity > 0) {
            size_t space = static_cast<size_t>(alloc_capacity);
            void* ptr = ::up::allocate(alloc, alloc_capacity);
            if (!ptr) {
                return dense_nomem;
            }

            hashcodes = static_cast<size_t*>(ptr);
            
            ::up::align_advance(sizeof(size_t) * capacity2, &ptr, &space);
            keys = static_cast<K const*>(::up::align(alignof(K), sizeof(K) * capacity2, &ptr, &space));
            
            ::up::align_advance(sizeof(K) * capacity2, &ptr, &space);
            values = static_cast<V*>(::up::align(alignof(V), sizeof(V) * capacity2, &ptr, &space));

            ::up::uninitialized_fill_n(hashcodes, capacity2, dense_vacant);
        }
        
        map.keys_and_hasher.first(keys);
        map.keys_and_hasher.second(hasher);
        map.values_and_equals.first(values);
        map.values_and_equals.second(equals);
        map.hashcodes = hashcodes;
        map.capacity = capacity2;
        map.used = 0;
        map.size = 0;

        return dense_success;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int densemap_destruct(densemap<K, V, H, E>& map, allocator* alloc) {
        if (!alloc) {
            return dense_badallocator;
        }

        K const* const keys = const_cast<K*>(map.keys());
        V* const values = map.values();
        size_t* const hashcodes = map.hashcodes;
        size_t const capacity = map.capacity;
        size_t index, remaining = map.size;
        
        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    const_cast<V*>(&values[index])->~V();
                    const_cast<K*>(&keys[index])->~K();
                    if (!(--remaining)) {
                        break;
                    }
                }
            }
        }

        if (capacity) {
            size_t const pad_size = sizeof(K) + sizeof(V);
            size_t const node_size = sizeof(size_t) + pad_size;
            size_t const buffer_size = ((capacity > 32) ? (node_size *capacity) : (node_size * 32)) + pad_size;
            ::up::deallocate(alloc, hashcodes, buffer_size);
            map.keys_and_hasher.first(nullptr);
            map.values_and_equals.first(nullptr);
            map.hashcodes = nullptr;
            map.capacity = 0;
            map.used = 0;
            map.size = 0;
        }

        return dense_success;
    }

    template <class K, class V, class H, class E, class Function>
    inline UPHIDDENINLINE
    void densemap_foreach(densemap<K, V, H, E> const& map, Function func) noexcept {
        K const* const keys = map.keys();
        V* const values = map.values();
        size_t const* const hashcodes = map.hashcodes;
        size_t index, remaining = map.size;
        denserecord<K, V> record;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
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

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    denserecord<K, V> densemap_find(densemap<K, V, H, E> const& map, Key const& key, Hasher const& hasher, Equals const& equals) {
        denserecord<K, V> result = { nullptr, nullptr };
        if (!map.size) {
            return result;
        }

        K const* const keys = map.keys();
        V* const values = map.values();
        size_t const* const hashcodes = map.hashcodes;
        size_t const mask = map.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode;

        for (offset = 0; offset <= mask; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode == dense_vacant) {
                break;
            }
            else if ((current_hashcode == hashcode) && equals(keys[index], key)) {
                result.key_ptr = &keys[index];
                result.value_ptr = &values[index];
                break;
            }
        }

        return result;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    denserecord<K, V> densemap_find(densemap<K, V, H, E> const& map, Key const& k) {
        return ::up::densemap_find(map, k, map.hasher(), map.equals());
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    size_t densemap_erase(densemap<K, V, H, E>& map, denserecord<K, V> const& record) {
        if (!record.key_ptr || !map.size) {
            return 0;
        }

        size_t const index = record.key_ptr - map.keys();
        if (index >= map.capacity) {
            return 0;
        }

        --map.size;
        map.hashcodes[index] = dense_deleted;
        const_cast<V*>(record.value_ptr)->~V();
        const_cast<K*>(record.key_ptr)->~K();
        return 1;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t densemap_erase(densemap<K, V, H, E>& map, Key const& key, Hasher const& hasher, Equals const& equals) {
        denserecord<K, V> record = ::up::densemap_find(map, key, hasher, equals);
        return ::up::densemap_erase(map, record);
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t densemap_erase(densemap<K, V, H, E>& map, Key const& key) {
        return ::up::densemap_erase(map, key, map.hasher(), map.equals());
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    size_t densemap_multi_erase(densemap<K, V, H, E>& map, Key const& key, Hasher const& hasher, Equals const& equals) {
        if (!map.size) {
            return 0;
        }

        K const* const keys = map.keys();
        V* const values = map.values();
        size_t * const hashcodes = map.hashcodes;
        size_t const mask = map.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode, count;

        for (offset = 0, count = 0; offset <= mask; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode == dense_vacant) {
                break;
            }
            else if ((current_hashcode == hashcode) && equals(keys[index], key)) {
                hashcodes[index] = dense_deleted;
                const_cast<V*>(&values[index])->~V();
                const_cast<K*>(&keys[index])->~K();
                ++count;
            }
        }
        
        map.size -= count;
        return count;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t densemap_multi_erase(densemap<K, V, H, E>& map, Key const& key) {
        return ::up::densemap_multi_erase(map, key, map.hasher(), map.equals());
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    denseresult<K, V> densemap_prepare(densemap<K, V, H, E>& map, K const& key) {
        denseresult<K, V> result = { { nullptr, nullptr }, false };

        if (map.size >= map.capacity) {
            return result;
        }

        H const& hasher = map.hasher();
        E const& equals = map.equals();
        K const* const keys = map.keys();
        V* const values = map.values();
        size_t* const hashcodes = map.hashcodes;
        size_t const mask = map.capacity - 1;
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
                    else if ((current_hashcode2 == hashcode) && equals(keys[index2], key)) {
                        index = index2;
                        goto yield;
                    }
                }
                break;
            }
            else if (current_hashcode == dense_vacant) {
                ++map.used;
                break;
            }
            else if ((current_hashcode == hashcode) && equals(keys[index], key)) {
                goto yield;
            }
        }

        ++map.size;
        hashcodes[index] = hashcode;
        result.success = true;

    yield:
        
        result.record.key_ptr = &keys[index];
        result.record.value_ptr = &values[index];
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denseresult<K, V> densemap_insert(densemap<K, V, H, E>& map, K const& key, V const& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(key);
            ::new(result.record.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denseresult<K, V> densemap_insert(densemap<K, V, H, E>& map, K const& key, V&& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(key);
            ::new(result.record.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denseresult<K, V> densemap_insert(densemap<K, V, H, E>& map, K&& key, V const& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(::up::move(key));
            ::new(result.record.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denseresult<K, V> densemap_insert(densemap<K, V, H, E>& map, K&& key, V&& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(::up::move(key));
            ::new(result.record.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    denserecord<K, V> densemap_multi_prepare(densemap<K, V, H, E>& map, K const& key) {
        denserecord<K, V> result;
        if (map.size >= map.capacity) {
            result.key_ptr = nullptr;
            result.value_ptr = nullptr;
            return result;
        }

        H const& hasher = map.hasher();
        K const* const keys = map.keys();
        V* const values = map.values();
        size_t* const hashcodes = map.hashcodes;
        size_t const mask = map.capacity - 1;
        size_t const hashcode = ::up::dense_resolve(hasher(key));
        size_t index, offset, current_hashcode;
        
        for (offset = 0; ; ++offset) {
            index = (hashcode + offset) & mask;
            current_hashcode = hashcodes[index];
            if (current_hashcode >= dense_deleted) {
                map.used += (current_hashcode & 1);
                current_hashcode = dense_vacant;
                break;
            }
        }

        ++map.size;
        hashcodes[index] = hashcode;
        result.key_ptr = &keys[index];
        result.value_ptr = &values[index];
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denserecord<K, V> densemap_multi_insert(densemap<K, V, H, E>& map, K const& key, V const& value) {
        denserecord<K, V> result = ::up::densemap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(const_cast<K*>(result.key_ptr)) K(key);
            ::new(result.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denserecord<K, V> densemap_multi_insert(densemap<K, V, H, E>& map, K const& key, V&& value) {
        denserecord<K, V> result = ::up::densemap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(const_cast<K*>(result.key_ptr)) K(key);
            ::new(result.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denserecord<K, V> densemap_multi_insert(densemap<K, V, H, E>& map, K&& key, V const& value) {
        denserecord<K, V> result = ::up::densemap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(const_cast<K*>(result.key_ptr)) K(::up::move(key));
            ::new(result.value_ptr) V(value);
        }
        return result;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    denserecord<K, V> densemap_multi_insert(densemap<K, V, H, E>& map, K&& key, V&& value) {
        denserecord<K, V> result = ::up::densemap_multi_prepare(map, key);
        if (result.key_ptr) {
            ::new(const_cast<K*>(result.key_ptr)) K(::up::move(key));
            ::new(result.value_ptr) V(::up::move(value));
        }
        return result;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    typename conditional<is_fundamental<V>::value, V, V const&>::type
    densemap_get(
        densemap<K, V, H, E> const& map,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals,
        typename conditional<is_fundamental<V>::value, V, V const&>::type default_value = V()
    ) {
        denserecord<K, V> record = ::up::densemap_find(map, key, hasher, equals);
        return record.key_ptr ? *record.value_ptr : default_value;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    typename conditional<is_fundamental<V>::value, V, V const&>::type
    densemap_get(
        densemap<K, V, H, E> const& map,
        Key const& key,
        typename conditional<is_fundamental<V>::value, V, V const&>::type default_value = V()
    ) {
        denserecord<K, V> record = ::up::densemap_find(map, key);
        return record.key_ptr ? *record.value_ptr : default_value;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_set(densemap<K, V, H, E>& map, K const& key, V const& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (!result.record.key_ptr) {
            return dense_nospace;
        }

        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(key);
            ::new(result.record.value_ptr) V(value);
        }
        else {
            *result.record.value_ptr = value;
        }

        return dense_success;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_set(densemap<K, V, H, E>& map, K const& key, V&& value) {
        denseresult<K, V> result = ::up::densemap_prepare(map, key);
        if (!result.record.key_ptr) {
            return dense_nospace;
        }

        if (result.success) {
            ::new(const_cast<K*>(result.record.key_ptr)) K(key);
            ::new(result.record.value_ptr) V(::up::move(value));
        }
        else {
            *result.record.value_ptr = ::up::move(value);
        }

        return dense_success;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_copy(densemap<K, V, H, E>& dst, densemap<K, V, H, E> const& src) {
        if (&dst == &src) {
            return;
        }

        K const* const keys = src.keys();
        V* const values = src.values();
        size_t const* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;
        denseresult<K, V> result;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    result = ::up::densemap_insert(dst, keys[index], values[index]);
                    --remaining;
                    if (!result.record.key_ptr || !remaining) {
                        break;
                    }
                }
            }
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_multi_copy(densemap<K, V, H, E>& dst, densemap<K, V, H, E>& src) {
        if (&dst == &src) {
            return;
        }

        K const* const keys = src.keys();
        V* const values = src.values();
        size_t const* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;
        denserecord<K, V> record;

        if (remaining) {
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    record = ::up::densemap_multi_insert(dst, keys[index], values[index]);
                    --remaining;
                    if (!record.key_ptr || !remaining) {
                        break;
                    }
                }
            }
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_move(densemap<K, V, H, E>& dst, densemap<K, V, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::densemap_swap(dst, src);
            return;
        }

        K const* const keys = src.keys();
        V* const values = src.values();
        size_t* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::densemap_insert(dst, ::up::move(keys[index]), ::up::move(values[index]));
                    const_cast<V*>(&values[index])->~V();
                    const_cast<K*>(&keys[index])->~K();
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

    template <class K, class V, class H, class E>
    UPVISIBLE
    void densemap_multi_move(densemap<K, V, H, E>& dst, densemap<K, V, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.capacity) {
            ::up::densemap_swap(dst, src);
            return;
        }

        K const* const keys = src.keys();
        V* const values = src.values();
        size_t* const hashcodes = src.hashcodes;
        size_t index, remaining = src.size;

        if (remaining) {
            src.size = 0;
            for (index = 0; ; ++index) {
                if (hashcodes[index] < dense_deleted) {
                    ::up::densemap_multi_insert(dst, ::up::move(keys[index]), ::up::move(values[index]));
                    const_cast<V*>(&values[index])->~V();
                    const_cast<K*>(&keys[index])->~K();
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

    template <class K, class V, class H, class E>
    UPVISIBLE
    int densemap_reserve(densemap<K, V, H, E>& map, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < map.size) || (capacity == map.capacity)) {
            return dense_success;
        }

        densemap<K, V, H, E> temp;
        int retval = ::up::densemap_construct(temp, alloc, capacity, map.hasher(), map.equals());
        if (retval != dense_success) {
            return retval;
        }

        ::up::densemap_move(temp, map);
        ::up::densemap_swap(temp, map);
        return ::up::densemap_destruct(temp, alloc);
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int densemap_multi_reserve(densemap<K, V, H, E>& map, allocator* alloc, size_t capacity) {
        capacity = (capacity && (capacity < 8)) ? 8 : ceil2(capacity);
        if ((capacity < map.size) || (capacity == map.capacity)) {
            return dense_success;
        }

        densemap<K, V, H, E> temp;
        int retval = ::up::densemap_construct(temp, alloc, capacity, map.hasher(), map.equals());
        if (retval != dense_success) {
            return retval;
        }

        ::up::densemap_multi_move(temp, map);
        ::up::densemap_swap(temp, map);
        return ::up::densemap_destruct(temp, alloc);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_grow(densemap<K, V, H, E>& map, allocator* alloc) noexcept {
        return ::up::densemap_reserve(map, alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_multi_grow(densemap<K, V, H, E>& map, allocator* alloc) noexcept {
        return ::up::densemap_multi_reserve(map, alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_shrink_to_fit(densemap<K, V, H, E>& map, allocator* alloc) noexcept {
        return ::up::densemap_reserve(map, alloc, map.size);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int densemap_multi_shrink_to_fit(densemap<K, V, H, E>& map, allocator* alloc) noexcept {
        return ::up::densemap_multi_reserve(map, alloc, map.size);
    }
}

#endif
