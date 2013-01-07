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

#ifndef UP_SPARSEMAP_HPP
#define UP_SPARSEMAP_HPP

#include <up/detail/sparse.inl>

namespace up
{   
    template <class Key, class Value, class Hasher, class Equals>
    struct UPVISIBLE sparsemap
    {
        typedef Key key_type;
        typedef Value value_type;
        typedef Hasher hasher_type;
        typedef Equals equals_type;
        compressed_pair<slist_node*, Hasher> buckets_and_hasher;
        compressed_pair<size_t, Equals> num_buckets_and_equals;
        size_t size;
        UPALWAYSINLINE slist_node* buckets() const noexcept { return buckets_and_hasher.first(); }
        UPALWAYSINLINE size_t num_buckets() const noexcept { return num_buckets_and_equals.first(); }
        UPALWAYSINLINE Hasher const& hasher() const noexcept { return buckets_and_hasher.second(); }
        UPALWAYSINLINE Equals const& equals() const noexcept { return num_buckets_and_equals.second(); }
    };

    template <class K, class V, class H, class E>
    UPVISIBLE
    bool sparsemap_validate(sparsemap<K, V, H, E> const& map) noexcept {
        H const& hasher = map.hasher();
        slist_node const* const buckets = map.buckets();
        size_t const num_buckets = map.num_buckets();
        size_t const size = map.size;
        size_t const mask = num_buckets - 1;
        sparserecord<K, V> const* record;
        slist_node const* node;
        size_t index, count;

        if ( (num_buckets && !buckets)
            || (!num_buckets && buckets)
            || (!num_buckets && (size > 0))
            || ((num_buckets & (num_buckets - 1)) != 0)
        ) {
            return false;
        }

        for (index = 0, count = 0; index < num_buckets; ++index) {
            for (node = buckets[index].next; node; ++count, node = node->next) {
                record = ::up::slist_cast<sparserecord<K, V> const*>(node, &sparserecord<K, V>::node);
                if ((hasher(record->key) & mask) != index) {
                    return false;
                }
            }
        }

        if (count != size) {
            return false;
        }

        return true;
    }

    template <class K, class V>
    inline UPALWAYSINLINE UPPURE
    ssize_t sparsemap_memory_footprint(size_t size, size_t num_buckets) noexcept {
        return ::up::sparse_memory_footprint(size, num_buckets, sizeof(sparserecord<K, V>));
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    ssize_t sparsemap_memory_footprint(sparsemap<K, V, H, E> const& map) noexcept {
        return ::up::sparse_memory_footprint(map.size, map.num_buckets(), sizeof(sparserecord<K, V>));
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    bool sparsemap_is_overloaded(sparsemap<K, V, H, E> const& map, double load_factor) noexcept {
        return ::up::sparse_is_overloaded(map.size, map.num_buckets(), load_factor);
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    bool sparsemap_is_empty(sparsemap<K, V, H, E> const& map) noexcept {
        return !map.size;
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    size_t sparsemap_size(sparsemap<K, V, H, E> const& map) noexcept {
        return map.size;
    }

    template <class K, class V, class H, class E>
    inline UPALWAYSINLINE
    size_t sparsemap_num_buckets(sparsemap<K, V, H, E> const& map) noexcept {
        return map.num_buckets();
    }

    template <class K, class V, class H, class E, class Recycle>
    UPVISIBLE
    typename enable_if<!is_convertible<Recycle, allocator*>::value>::type
    sparsemap_clear(sparsemap<K, V, H, E>& map, Recycle recycle) noexcept {
        slist_node* bucket = map.buckets();
        slist_node* const end_bucket = bucket + map.num_buckets();
        slist_node* curr, * next;

        if (map.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    curr->next = nullptr;
                    recycle(::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node));
                }
                bucket->next = nullptr;
            }
            map.size = 0;
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_clear(sparsemap<K, V, H, E>& map, allocator* record_alloc) noexcept {
        if (!record_alloc) {
            return sparse_badallocator;
        }

        slist_node* bucket = map.buckets();
        slist_node* const end_bucket = bucket + map.num_buckets();
        slist_node* curr, * next;

        if (map.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    ::up::destruct_deallocate(
                        record_alloc,
                        ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node)
                    );
                }
                bucket->next = nullptr;
            }
            map.size = 0;
        }

        return sparse_success;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    void sparsemap_swap(sparsemap<K, V, H, E>& map1, sparsemap<K, V, H, E>& map2) noexcept {
        if (&map1 != &map2) {
            ::up::swap(map1.buckets_and_hasher, map2.buckets_and_hasher);
            ::up::swap(map1.num_buckets_and_equals, map2.num_buckets_and_equals);
            ::up::swap(map1.size, map2.size);
        }
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    void sparsemap_construct(sparsemap<K, V, H, E>& map, H const& hasher = H(), E const& equals = E()) noexcept {
        map.buckets_and_hasher.first(nullptr);
        map.buckets_and_hasher.second(hasher);
        map.num_buckets_and_equals.first(0);
        map.num_buckets_and_equals.second(equals);
        map.size = 0;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_construct(
        sparsemap<K, V, H, E>& map,
        allocator* bucket_alloc,
        size_t num_buckets,
        H const& hasher = H(),
        E const& equals = E()
    ) {
        if (!bucket_alloc) {
            return sparse_badallocator;
        }

        size_t const num_buckets2 = ::up::ceil2(num_buckets);
        if (num_buckets2 > (SIZE_MAX / sizeof(slist_node))) {
            return sparse_badlength;
        }

        slist_node* buckets = nullptr;
        if (num_buckets2 > 0) {
            buckets = ::up::allocate_construct_n<slist_node>(bucket_alloc, num_buckets2);
            if (!buckets) {
                return sparse_nomem;
            }
        }
        
        map.buckets_and_hasher.first(buckets);
        map.buckets_and_hasher.second(hasher);
        map.num_buckets_and_equals.first(num_buckets2);
        map.num_buckets_and_equals.second(equals);
        map.size = 0;
        return sparse_success;
    }

    template <class K, class V, class H, class E, class Recycle>
    UPVISIBLE
    typename enable_if<!is_convertible<Recycle, allocator*>::value, int>::type
    sparsemap_destruct(sparsemap<K, V, H, E>& map, allocator* bucket_alloc, Recycle recycle) {
        if (!bucket_alloc) {
            return sparse_badallocator;
        }

        ::up::sparsemap_clear(map, recycle);

        size_t const num_buckets = map.num_buckets();
        if (num_buckets) {
            ::up::deallocate_n(bucket_alloc, map.buckets(), num_buckets);
            map.buckets_and_hasher.first(nullptr);
            map.num_buckets_and_equals.first(0);
            map.size = 0;
        }

        return sparse_success;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_destruct(sparsemap<K, V, H, E>& map, allocator* bucket_alloc, allocator* record_alloc) {
        if (!bucket_alloc) {
            return sparse_badallocator;
        }

        int const retval = ::up::sparsemap_clear(map, record_alloc);
        if (retval != sparse_success) {
            return retval;
        }

        size_t const num_buckets = map.num_buckets();
        if (num_buckets) {
            ::up::deallocate_n(bucket_alloc, map.buckets(), num_buckets);
            map.buckets_and_hasher.first(nullptr);
            map.num_buckets_and_equals.first(0);
            map.size = 0;
        }

        return sparse_success;
    }

    template <class K, class V, class H, class E, class Function>
    inline UPHIDDENINLINE
    void sparsemap_foreach(sparsemap<K, V, H, E> const& map, Function func) noexcept {
        slist_node* bucket = map.buckets(), * curr;
        slist_node* const end_bucket = bucket + map.num_buckets();
        sparserecord<K, V>* record;

        for ( ; bucket < end_bucket; ++bucket) {
            for (curr = bucket->next; curr; curr = curr->next) {
                record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
                func(record->key, record->value);
            }
        }
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    sparserecord<K, V>* sparsemap_find(
        sparsemap<K, V, H, E> const& map,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals
    ) {
        if (!map.size) {
            return nullptr;
        }

        H const& record_hasher = map.hasher();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = map.buckets();
        sparserecord<K, V>* record;
        slist_node* curr;

        for (curr = buckets[hashcode & mask].next; curr; curr = curr->next) {
            record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
            if ((record_hasher(record->key) == hashcode) && equals(record->key, key)) {
                return record;
            }
        }

        return nullptr;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    sparserecord<K, V>* sparsemap_find(sparsemap<K, V, H, E> const& map, Key const& key) {
        return ::up::sparsemap_find(map, key, map.hasher(), map.equals());
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    sparserecord<K, V>* sparsemap_erase(sparsemap<K, V, H, E>& map, sparserecord<K, V>* record) {
        if (!map.size || !record) {
            return nullptr;
        }

        H const& hasher = map.hasher();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(record->key);
        slist_node* const buckets = map.buckets();
        slist_node* const record_node = ::up::slist_cast<slist_node*>(record, &sparserecord<K, V>::node);
        slist_node* prev, * curr;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = curr->next) {
            if (curr == record_node) {
                prev->next = curr->next;
                curr->next = nullptr;
                --map.size;
                return record;
            }
            else {
                prev = curr;
            }
        }

        return nullptr;
    }

    template <class K, class V, class H, class E, class Recycle>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(sparsemap<K, V, H, E>& map, sparserecord<K, V>* record, Recycle recycle) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, record);
        if (result) {
            recycle(result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(sparsemap<K, V, H, E>& map, allocator* record_alloc, sparserecord<K, V>* record) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, record);
        if (result) {
            ::up::destruct_deallocate(record_alloc, result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    UPVISIBLE
    sparserecord<K, V>* sparsemap_erase(
        sparsemap<K, V, H, E>& map,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals
    ) {
        if (!map.size) {
            return nullptr;
        }

        H const& record_hasher = map.hasher();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = map.buckets();
        sparserecord<K, V>* record;
        slist_node* prev, * curr;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = curr->next) {
            record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
            if ((record_hasher(record->key) == hashcode) && equals(record->key, key)) {
                prev->next = curr->next;
                curr->next = nullptr;
                --map.size;
                return record;
            }
            else {
                prev = curr;
            }
        }

        return nullptr;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals, class Recycle>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(
        sparsemap<K, V, H, E>& map,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals,
        Recycle recycle
    ) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, key, hasher, equals);
        if (result) {
            recycle(result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals
    ) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, key, hasher, equals);
        if (result) {
            ::up::destruct_deallocate(record_alloc, result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    sparserecord<K, V>* sparsemap_erase(sparsemap<K, V, H, E>& map, Key const& key) {
        return ::up::sparsemap_erase(map, key, map.hasher(), map.equals());
    }

    template <class K, class V, class H, class E, class Key, class Recycle>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(sparsemap<K, V, H, E>& map, Key const& key, Recycle recycle) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, key, map.hasher(), map.equals());
        if (result) {
            recycle(result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t sparsemap_erase(sparsemap<K, V, H, E>& map, allocator* record_alloc, Key const& key) {
        sparserecord<K, V>* result = ::up::sparsemap_erase(map, key, map.hasher(), map.equals());
        if (result) {
            ::up::destruct_deallocate(record_alloc, result);
            return 1;
        }
        return 0;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals, class Recycle>
    UPVISIBLE
    size_t sparsemap_multi_erase(
        sparsemap<K, V, H, E>& map,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals,
        Recycle recycle
    ) {
        if (!map.size) {
            return 0;
        }

        H const& record_hasher = map.hasher();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = map.buckets();
        slist_node* prev, * curr, * next;
        sparserecord<K, V>* record;
        size_t count = 0;

        for (prev = &buckets[hashcode & mask], curr = prev->next; curr; curr = next) {
            next = curr->next;
            record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
            if ((record_hasher(record->key) == hashcode) && equals(record->key, key)) {
                prev->next = next;
                curr->next = nullptr;
                recycle(record);
                ++count;
            }
            else {
                prev = curr;
            }
        }

        map.size -= count;
        return count;
    }

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    size_t sparsemap_multi_erase(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        Key const& key,
        Hasher const& hasher,
        Equals const& equals
    ) {
        allocator_deleter<sparserecord<K, V> > deleter(record_alloc);
        return ::up::sparsemap_multi_erase(map, key, hasher, equals, deleter);
    }

    template <class K, class V, class H, class E, class Key, class Recycle>
    inline UPHIDDENINLINE
    size_t sparsemap_multi_erase(sparsemap<K, V, H, E>& map, Key const& key, Recycle recycle) {
        return ::up::sparsemap_multi_erase(map, key, map.hasher(), map.equals(), recycle);
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    size_t sparsemap_multi_erase(sparsemap<K, V, H, E>& map, allocator* record_alloc, Key const& key) {
        allocator_deleter<sparserecord<K, V> > deleter(record_alloc);
        return ::up::sparsemap_multi_erase(map, key, map.hasher(), map.equals(), deleter);
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    sparseresult<K, V> sparsemap_prepare(sparsemap<K, V, H, E>& map, K const& key, slist_node** bucket_ptr) {
        sparseresult<K, V> result;
        if (!map.num_buckets()) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        H const& hasher = map.hasher();
        E const& equals = map.equals();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = map.buckets();
        *bucket_ptr = &buckets[hashcode & mask];

        for (slist_node* node = (*bucket_ptr)->next; node; node = node->next) {
            sparserecord<K, V>* record = ::up::slist_cast<sparserecord<K, V>*>(node, &sparserecord<K, V>::node);
            if ((hasher(record->key) == hashcode) && equals(record->key, key)) {
                result.record = record;
                result.success = false;
                return result;
            }
        }

        result.record = nullptr;
        result.success = true;
        return result;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    sparseresult<K, V> sparsemap_insert(sparsemap<K, V, H, E>& map, sparserecord<K, V>* record) {
        sparseresult<K, V> result;
        if (!record) {
            result.record = nullptr;
            result.success = false;
            return result;
        }
                
        slist_node* bucket = nullptr;
        result = ::up::sparsemap_prepare(map, record->key, &bucket);
        if (result.success) {
            result.record = record;
            record->node.next = bucket->next;
            bucket->next = &record->node;
            ++map.size;
        }

        return result;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    sparseresult<K, V> sparsemap_insert(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        K const& key,
        V const& value
    ) {
        sparseresult<K, V> result;
        if (!record_alloc) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        slist_node* bucket = nullptr;
        result = ::up::sparsemap_prepare(map, key, &bucket);

        if (result.success) {
            result.record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, value);
            if (result.record) {
                result.record->node.next = bucket->next;
                bucket->next = &result.record->node;
                ++map.size;
            }
            else {
                result.success = false;
            }
        }

        return result;
    }

#ifndef UP_NO_RVALUE_REFERENCES
    template <class K, class V, class H, class E>
    UPVISIBLE
    sparseresult<K, V> sparsemap_insert(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        K const& key,
        V&& value
    ) {
        sparseresult<K, V> result;
        if (!record_alloc) {
            result.record = nullptr;
            result.success = false;
            return result;
        }

        slist_node* bucket = nullptr;
        result = ::up::sparsemap_prepare(map, key, &bucket);

        if (result.success) {
            result.record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, ::up::move(value));
            if (result.record) {
                result.record->node.next = bucket->next;
                bucket->next = &result.record->node;
                ++map.size;
            }
            else {
                result.success = false;
            }
        }

        return result;
    }
#endif

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    slist_node* sparsemap_multi_prepare(sparsemap<K, V, H, E>& map, K const& key) {
        if (!map.num_buckets()) {
            return nullptr;
        }

        H const& hasher = map.hasher();
        size_t const mask = map.num_buckets() - 1;
        size_t const hashcode = hasher(key);
        slist_node* const buckets = map.buckets();
        return &buckets[hashcode & mask];
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    sparserecord<K, V>* sparsemap_multi_insert(sparsemap<K, V, H, E>& map, sparserecord<K, V>* record) {
        if (!record) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparsemap_multi_prepare(map, record->key);
        record->node.next = bucket->next;
        bucket->next = &record->node;
        ++map.size;
        return record;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    sparserecord<K, V>* sparsemap_multi_insert(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        K const& key,
        V const& value
    ) {
        if (!record_alloc) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparsemap_multi_prepare(map, key);
        if (!bucket) {
            return nullptr;
        }

        sparserecord<K, V>* record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, value);
        if (!record) {
            return nullptr;
        }

        record->node.next = bucket->next;
        bucket->next = &record->node;
        ++map.size;
        return record;
    }

#ifndef UP_NO_RVALUE_REFERENCES
    template <class K, class V, class H, class E>
    UPVISIBLE
    sparserecord<K, V>* sparsemap_multi_insert(
        sparsemap<K, V, H, E>& map,
        allocator* record_alloc,
        K const& key,
        V&& value
    ) {
        if (!record_alloc) {
            return nullptr;
        }

        slist_node* bucket = ::up::sparsemap_multi_prepare(map, key);
        if (!bucket) {
            return nullptr;
        }

        sparserecord<K, V>* record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, ::up::move(value));
        if (!record) {
            return nullptr;
        }

        record->node.next = bucket->next;
        bucket->next = &record->node;
        ++map.size;
        return record;
    }
#endif

    template <class K, class V, class H, class E, class Key, class Hasher, class Equals>
    inline UPHIDDENINLINE
    typename conditional<is_fundamental<V>::value, V, V const&>::type
    sparsemap_get(
        sparsemap<K, V, H, E>& map, Key const& key,
        Hasher const& hasher,
        Equals const& equals,
        typename conditional<is_fundamental<V>::value, V, V const&>::type default_value = V()
    ) {
        sparserecord<K, V>* record = ::up::sparsemap_find(map, key, hasher, equals);
        return record ? record->value : default_value;
    }

    template <class K, class V, class H, class E, class Key>
    inline UPHIDDENINLINE
    typename conditional<is_fundamental<V>::value, V, V const&>::type
    sparsemap_get(
        sparsemap<K, V, H, E>& map,
        Key const& key,
        typename conditional<is_fundamental<V>::value, V, V const&>::type default_value = V()
    ) {
        sparserecord<K, V>* record = ::up::sparsemap_find(map, key);
        return record ? record->value : default_value;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_set(sparsemap<K, V, H, E>& map, allocator* record_alloc, K const& key, V const& value) {
        if (!record_alloc) {
            return sparse_badallocator;
        }

        slist_node* bucket = nullptr;
        sparseresult<K, V> result = ::up::sparsemap_prepare(map, key, &bucket);

        if (result.success) {
            result.record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, value);
            if (!result.record) {
                return sparse_nomem;
            }
            result.record->node.next = bucket->next;
            bucket->next = &result.record->node;
            ++map.size;
        }
        else {
            result.record->value = value;
        }

        return sparse_success;
    }

#ifndef UP_NO_RVALUE_REFERENCES
    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_set(sparsemap<K, V, H, E>& map, allocator* record_alloc, K const& key, V&& value) {
        if (!record_alloc) {
            return sparse_badallocator;
        }

        slist_node* bucket = nullptr;
        sparseresult<K, V> result = ::up::sparsemap_prepare(map, key, &bucket);

        if (result.success) {
            result.record = ::up::allocate_construct<sparserecord<K, V> >(record_alloc, key, ::up::move(value));
            if (!result.record) {
                return sparse_nomem;
            }
            result.record->node.next = bucket->next;
            bucket->next = &result.record->node;
            ++map.size;
        }
        else {
            result.record->value = ::up::move(value);
        }

        return sparse_success;
    }
#endif

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_copy(sparsemap<K, V, H, E>& dst, sparsemap<K, V, H, E> const& src, allocator* record_alloc) {
        if (!record_alloc) {
            return sparse_badallocator;
        }
        else if (&dst == &src) {
            return sparse_success;
        }

        slist_node* bucket = src.buckets(), * curr;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparserecord<K, V>* record;
        sparseresult<K, V> result;

        for ( ; bucket < end_bucket; ++bucket) {
            for (curr = bucket->next; curr; curr = curr->next) {
                record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
                result = ::up::sparsemap_insert(dst, record_alloc, record->key, record->value);
                if (!result.record) {
                    return sparse_nomem;
                }
            }
        }

        return sparse_success;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_multi_copy(sparsemap<K, V, H, E>& dst, sparsemap<K, V, H, E> const& src, allocator* record_alloc) {
        if (!record_alloc) {
            return sparse_badallocator;
        }
        else if (&dst == &src) {
            return sparse_success;
        }

        slist_node* bucket = src.buckets(), * curr;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparserecord<K, V>* record, * new_record;

        for ( ; bucket < end_bucket; ++bucket) {
            for (curr = bucket->next; curr; curr = curr->next) {
                record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
                new_record = ::up::sparsemap_multi_insert(dst, record_alloc, record->key, record->value);
                if (!new_record) {
                    return sparse_nomem;
                }
            }
        }

        return sparse_success;
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void sparsemap_move(sparsemap<K, V, H, E>& dst, sparsemap<K, V, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.num_buckets()) {
            ::up::sparsemap_swap(dst, src);
            return;
        }
        
        slist_node* bucket = src.buckets(), * curr, * next;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparserecord<K, V>* record;
        sparseresult<K, V> result;

        if (src.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
                    result = ::up::sparsemap_insert(dst, record);
                }
                bucket->next = nullptr;
            }
            src.size = 0;
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    void sparsemap_multi_move(sparsemap<K, V, H, E>& dst, sparsemap<K, V, H, E>& src) {
        if (&dst == &src) {
            return;
        }
        else if (!dst.num_buckets()) {
            ::up::sparsemap_swap(dst, src);
            return;
        }

        slist_node* bucket = src.buckets(), * curr, * next;
        slist_node* const end_bucket = bucket + src.num_buckets();
        sparserecord<K, V>* record;
        sparseresult<K, V> result;

        if (src.size) {
            for ( ; bucket < end_bucket; ++bucket) {
                for (curr = bucket->next; curr; curr = next) {
                    next = curr->next;
                    record = ::up::slist_cast<sparserecord<K, V>*>(curr, &sparserecord<K, V>::node);
                    result = ::up::sparsemap_multi_insert(dst, record);
                }
                bucket->next = nullptr;
            }
            src.size = 0;
        }
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_reserve(sparsemap<K, V, H, E>& map, allocator* bucket_alloc, size_t num_buckets) {
        num_buckets = (num_buckets && (num_buckets < 8)) ? 8 : ceil2(num_buckets);
        if ((num_buckets < map.size) || (num_buckets == map.num_buckets())) {
            return sparse_success;
        }

        sparsemap<K, V, H, E> temp;
        int retval = ::up::sparsemap_construct(temp, bucket_alloc, num_buckets, map.hasher(), map.equals());
        if (retval != sparse_success) {
            return retval;
        }

        ::up::sparsemap_move(temp, map);
        ::up::sparsemap_swap(temp, map);
        return ::up::sparsemap_destruct(temp, bucket_alloc, bucket_alloc);
    }

    template <class K, class V, class H, class E>
    UPVISIBLE
    int sparsemap_multi_reserve(sparsemap<K, V, H, E>& map, allocator* bucket_alloc, size_t num_buckets) {
        num_buckets = (num_buckets && (num_buckets < 8)) ? 8 : ceil2(num_buckets);
        if ((num_buckets < map.size) || (num_buckets == map.num_buckets())) {
            return sparse_success;
        }

        sparsemap<K, V, H, E> temp;
        int retval = ::up::sparsemap_construct(temp, bucket_alloc, num_buckets, map.hasher(), map.equals());
        if (retval != sparse_success) {
            return retval;
        }

        ::up::sparsemap_multi_move(temp, map);
        ::up::sparsemap_swap(temp, map);
        return ::up::sparsemap_destruct(temp, bucket_alloc, bucket_alloc);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int sparsemap_grow(sparsemap<K, V, H, E>& map, allocator* bucket_alloc) noexcept {
        return ::up::sparsemap_reserve(map, bucket_alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int sparsemap_multi_grow(sparsemap<K, V, H, E>& map, allocator* bucket_alloc) noexcept {
        return ::up::sparsemap_multi_reserve(map, bucket_alloc, map.size ? (map.size << 1) : 8);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int sparsemap_shrink_to_fit(sparsemap<K, V, H, E>& map, allocator* bucket_alloc) noexcept {
        return ::up::sparsemap_reserve(map, bucket_alloc, map.size);
    }

    template <class K, class V, class H, class E>
    inline UPHIDDENINLINE
    int sparsemap_multi_shrink_to_fit(sparsemap<K, V, H, E>& map, allocator* bucket_alloc) noexcept {
        return ::up::sparsemap_multi_reserve(map, bucket_alloc, map.size);
    }
}

#endif
