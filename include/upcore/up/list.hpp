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

#ifndef UP_LIST_HPP
#define UP_LIST_HPP

#include <up/cstddef.hpp>

namespace up
{   
    struct LIBUPCOREAPI list_node
    {
        list_node* next;
        list_node* prev;
    };
    
    extern LIBUPCOREAPI
    list_node* list_sort_tree_recursive(list_node* node, list_node* tree) noexcept;
    
    extern LIBUPCOREAPI
    void list_swap(list_node* sentinel1, list_node* sentinel2) noexcept;
    
    extern LIBUPCOREAPI
    bool list_validate(list_node const* sentinel, ptrdiff_t expected_size = -1) noexcept;

    namespace detail
    {
        template <class T, class U, class V>
        struct UPHIDDEN list_cast_impl
        {
            static UPALWAYSINLINE
            T cast(U* x, list_node V::* p) noexcept {
                return static_cast<T>(&(x->*p));
            }
        };

        template <class T, class V>
        struct UPHIDDEN list_cast_impl<T, list_node, V>
        {
            static UPALWAYSINLINE
            T cast(list_node* x, list_node V::* p) noexcept {
                uintptr_t addr = reinterpret_cast<uintptr_t>(x) - reinterpret_cast<uintptr_t>(&(((V*)0)->*p));
                return static_cast<T>(reinterpret_cast<V*>(addr));
            }
        };

        template <class T, class V>
        struct UPHIDDEN list_cast_impl<T, list_node const, V>
        {
            static UPALWAYSINLINE
            T cast(list_node const* x, list_node V::* p) noexcept {
                uintptr_t addr = reinterpret_cast<uintptr_t>(x) - reinterpret_cast<uintptr_t>(&(((V const*)0)->*p));
                return static_cast<T>(reinterpret_cast<V const*>(addr));
            }
        };
    }

    template <class T, class U, class V>
    inline UPALWAYSINLINE
    T list_cast(U* x, list_node V::* p) noexcept {
        return ::up::detail::list_cast_impl<T, U, V>::cast(x, p);
    }

    inline UPALWAYSINLINE
    void list_construct(list_node* node) noexcept {
        node->prev = node->next = node;
    }

    inline UPALWAYSINLINE
    void list_move(list_node* new_node, list_node* old_node) noexcept {
        if (old_node->next != old_node) {
            new_node->next = old_node->next;
            new_node->prev = old_node->prev;
            old_node->next = old_node->prev = old_node;
        }
        else {
            new_node->next = new_node->prev = new_node;
        }
    }

    inline UPALWAYSINLINE
    bool list_is_empty(list_node const* node) noexcept {
        return node->next == node;
    }

    inline UPALWAYSINLINE
    bool list_is_linked(list_node const* node) noexcept {
        return node->next != node;
    }

    inline UPALWAYSINLINE
    size_t list_size(list_node const* first, list_node const* last) noexcept {
        size_t count = 0;
        for ( ; first != last; first = first->next) {
            ++count;
        }
        return count;
    }

    inline UPALWAYSINLINE
    size_t list_size(list_node const* sentinel) noexcept {
        return ::up::list_size(sentinel->next, sentinel);
    }

    template <class Node, list_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void list_foreach(list_node* first, list_node* last, Function func) noexcept {
        for ( ; first != last; first = first->next) {
            func(::up::list_cast<Node*>(first, NodePtr));
        }
    }

    template <class Node, list_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void list_foreach(list_node* sentinel, Function func) noexcept {
        ::up::list_foreach<Node, NodePtr>(sentinel->next, sentinel, func);
    }

    template <class Node, list_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    bool list_is_sorted(list_node const* first, list_node const* last, Compare comp) noexcept {
        if (first != last) {
            for (list_node const* second = first->next; second != last; first = second, second = second->next) {
                if (!comp(::up::list_cast<Node const*>(first, NodePtr), ::up::list_cast<Node const*>(second, NodePtr))) {
                    return false;
                }
            }
        }
        return true;
    }

    template <class Node, list_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    bool list_is_sorted(list_node const* sentinel, Compare comp) noexcept {
        return ::up::list_is_sorted<Node, NodePtr>(sentinel->next, sentinel, comp);
    }

    inline UPALWAYSINLINE
    void list_erase(list_node* first, list_node* last) noexcept {
        first->prev->next = last;
        last->prev = first->prev;
        for (list_node* next; first != last; first = next) {
            next = first->next;
            first->prev = first->next = first;
        }
    }
    
    template <class Node, list_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void list_erase(list_node* first, list_node* last, Function func) noexcept {
        first->prev->next = last;
        last->prev = first->prev;
        for (list_node* node = first, * next; node != last; node = next) {
            next = node->next;
            node->prev = node->next = node;
            func(::up::list_cast<Node*>(node, NodePtr));
        }
    }

    inline UPALWAYSINLINE
    void list_clear(list_node* sentinel) noexcept {
        ::up::list_erase(sentinel->next, sentinel);
    }

    template <class Node, list_node Node::* NodePtr, class Function>
    inline UPALWAYSINLINE
    void list_clear(list_node* sentinel, Function func) noexcept {
        ::up::list_erase<Node, NodePtr>(sentinel->next, sentinel, func);
    }

    inline UPALWAYSINLINE
    void list_unlink(list_node* node) noexcept {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = node->next = node;
    }

    inline UPALWAYSINLINE
    void list_link_before(list_node* node, list_node* new_node) noexcept {
        new_node->prev = node->prev;
        node->prev->next = new_node;
        node->prev = new_node;
        new_node->next = node;
    }

    inline UPALWAYSINLINE
    void list_link_before_prev(list_node* node, list_node* new_node) noexcept {
        new_node->prev = node->prev;
        node->prev->next = new_node;
    }

    inline UPALWAYSINLINE
    void list_link_before_next(list_node* node, list_node* new_node) noexcept {
        node->prev = new_node;
        new_node->next = node;
    }

    inline UPALWAYSINLINE
    void list_link_after(list_node* node, list_node* new_node) noexcept {
        new_node->next = node->next;
        node->next->prev = new_node;
        node->next = new_node;
        new_node->prev = node;
    }

    inline UPALWAYSINLINE
    void list_link_after_prev(list_node* node, list_node* new_node) noexcept {
        node->next = new_node;
        new_node->prev = node;
    }

    inline UPALWAYSINLINE
    void list_link_after_next(list_node* node, list_node* new_node) noexcept {
        new_node->next = node->next;
        node->next->prev = new_node;
    }

    inline UPALWAYSINLINE
    void list_splice_before(list_node* node, list_node* first, list_node* last) noexcept {
        if (first != last) {
            list_node* const first_prev = first->prev;
            list_node* const last_prev = last->prev;
            first_prev->next = last;
            last->prev = first_prev;
            first->prev = node->prev;
            node->prev->next = first;
            node->prev = last_prev;
            last_prev->next = node;
        }
    }

    inline UPALWAYSINLINE
    void list_splice_after(list_node* node, list_node* first, list_node* last) noexcept {
        if (first != last) {
            list_node* const first_prev = first->prev;
            list_node* const last_prev = last->prev;
            first_prev->next = last;
            last->prev = first_prev;
            last_prev->next = node->next;
            node->next->prev = last_prev;
            node->next = first;
            first->prev = node;
        }
    }

    inline UPALWAYSINLINE
    void list_push_front(list_node* sentinel, list_node* node) noexcept {
        node->next = sentinel->next;
        sentinel->next->prev = node;
        sentinel->next = node;
        node->prev = sentinel;
    }

    inline UPALWAYSINLINE
    void list_push_back(list_node* sentinel, list_node* node) noexcept {
        node->prev = sentinel->prev;
        sentinel->prev->next = node;
        sentinel->prev = node;
        node->next = sentinel;
    }

    inline UPALWAYSINLINE
    list_node* list_pop_front(list_node* sentinel) noexcept {
        list_node* const front = sentinel->next;
        sentinel->next = front->next;
        sentinel->next->prev = sentinel;
        front->next = front->prev = front;
        return front;
    }

    inline UPALWAYSINLINE
    list_node* list_pop_back(list_node* sentinel) noexcept {
        list_node* const back = sentinel->prev;
        sentinel->prev = back->prev;
        sentinel->prev->next = sentinel;
        back->next = back->prev = back;
        return back;
    }

    inline UPALWAYSINLINE
    list_node* list_peek_front(list_node const* sentinel) noexcept {
        return sentinel->next;
    }

    inline UPALWAYSINLINE
    list_node* list_peek_back(list_node const* sentinel) noexcept {
        return sentinel->prev;
    }

    inline UPALWAYSINLINE
    void list_reverse(list_node* first, list_node* last) noexcept {
        first->prev->next = last->prev;
        last->prev = first;
        for (list_node* node = first, * next; node != last; node = next) {
            next = node->next;
            node->next = node->prev;
            node->prev = next;
        }
    }

    inline UPALWAYSINLINE
    void list_reverse(list_node* sentinel) noexcept {
        ::up::list_reverse(sentinel->next, sentinel);
    }

    template <class Node, list_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    void list_merge(list_node* sentinel1, list_node* sentinel2, Compare comp) noexcept {
        if (sentinel1 != sentinel2) {
            list_node* first1 = sentinel1->next;
            list_node* first2 = sentinel2->next;
            while ((first1 != sentinel1) && (first2 != sentinel2)) {
                if (comp(::up::list_cast<Node const*>(first1, NodePtr), ::up::list_cast<Node const*>(first2, NodePtr))) {
                    first1 = first1->next;
                }
                else {
                    list_node* next2 = first2->next;
                    ::up::link_before(first1, first2);
                    first2 = next2;
                }
            }
            ::up::list_link_before_next(first2, sentinel2);
            ::up::list_splice_before(sentinel1, first2, sentinel2);
        }
    }

    template <class Node, list_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    list_node* list_make_tree(list_node* sentinel, Compare comp) noexcept {
        list_node* const tree = sentinel->next;
        list_node* left = tree->next;
        tree->prev = tree->next = nullptr;
        while (left != sentinel) {
            list_node* right = tree, ** right_ptr;
            do {
                if (comp(::up::list_cast<Node const*>(left, NodePtr), ::up::list_cast<Node const*>(right, NodePtr))) {
                    right_ptr = &right->prev;
                    right = right->prev;
                }
                else {
                    right_ptr = &right->next;
                    right = right->next;
                }
            }
            while (right);
            right = left;
            left = left->next;
            right->prev = right->next = nullptr;
            *right_ptr = right;
        }
        return tree;
    }

    inline UPALWAYSINLINE
    void list_sort_tree(list_node* sentinel, list_node* tree) noexcept {
        list_node* const head = ::up::list_sort_tree_recursive(sentinel, tree);
        ::up::list_link_before_next(sentinel, head);
    }

    template <class Node, list_node Node::* NodePtr, class Compare>
    inline UPALWAYSINLINE
    void list_sort(list_node* sentinel, Compare comp) noexcept {
        list_node* const tree = ::up::list_make_tree<Node, NodePtr, Compare>(sentinel, comp);
        ::up::list_sort_tree(sentinel, tree);
    }
}

#endif
