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

#include <up/list.hpp>
#include <up/test.hpp>
#include <up/climits.hpp>

namespace list
{
    struct int_node
    {
        up::list_node node;
        int value;
    };

    struct compare_int_node
    {
        bool operator()(int_node const* x, int_node const* y) const noexcept {
            return x->value < y->value;
        }
    };

    void build_int_list(up::list_node* sentinel, int_node* store, int const* values, size_t n) {
        up::list_init(sentinel);

        for (size_t i = 0; i < n; ++i) {
            store[i].value = values[i];
            up::list_link_before(sentinel, up::list_cast<up::list_node*>(&store[i], &int_node::node));
        }
    }

    UP_TEST_CASE(is_sorted) {
        int const sorted_values[10] = { INT_MIN, -963, -41, -1, 0, 1, 2, 8, 2351, INT_MAX };
        int const unsorted_values[10] = { 8, -963, INT_MAX, -1, 2, 2351, 0, INT_MIN, 1, -41 };
        int_node node_store[10];
        up::list_node root;
        bool result;

#ifndef UP_NO_LAMBDAS
        auto compare = [](int_node const* x, int_node const* y) {
            return x->value < y->value;
        };
#else
        compare_int_node compare;
#endif

        build_int_list(&root, node_store, sorted_values, 10);
        require(up::list_validate(&root, 10));

        result = up::list_is_sorted<int_node, &int_node::node>(&root, compare);

        require(result && up::list_validate(&root, 10));

        build_int_list(&root, node_store, unsorted_values, 10);
        require(up::list_validate(&root, 10));

        result = up::list_is_sorted<int_node, &int_node::node>(&root, compare);

        require(!result && up::list_validate(&root, 10));
    }

    UP_TEST_CASE(sort) {
        int const unsorted_values[10] = { 8, -963, INT_MAX, -1, 2, 2351, 0, INT_MIN, 1, -41 };
        int_node node_store[10];
        up::list_node root;
        bool result;

        build_int_list(&root, node_store, unsorted_values, 10);
        require(up::list_validate(&root, 10));

#ifndef UP_NO_LAMBDAS
        auto compare = [](int_node const* x, int_node const* y) {
            return x->value < y->value;
        };
#else
        compare_int_node compare;
#endif

        result = up::list_is_sorted<int_node, &int_node::node>(&root, compare);

        require(!result && up::list_validate(&root, 10));

        up::list_sort<int_node, &int_node::node>(&root, compare);

        require(up::list_validate(&root, 10));

        result = up::list_is_sorted<int_node, &int_node::node>(&root, compare);

        require(result && up::list_validate(&root, 10));
    }
}
