
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

#ifndef UP_TYPE_TRAITS_CXX11_INL
#define UP_TYPE_TRAITS_CXX11_INL

#include <type_traits>

namespace up
{
    using ::std::integral_constant;
    using ::std::true_type;
    using ::std::false_type;
    using ::std::is_void;
    using ::std::is_integral;
    using ::std::is_floating_point;
    using ::std::is_array;
    using ::std::is_pointer;
    using ::std::is_lvalue_reference;
    using ::std::is_rvalue_reference;
    using ::std::is_member_object_pointer;
    using ::std::is_member_function_pointer;
    using ::std::is_enum;
    using ::std::is_union;
    using ::std::is_class;
    using ::std::is_function;
    using ::std::is_reference;
    using ::std::is_arithmetic;
    using ::std::is_fundamental;
    using ::std::is_object;
    using ::std::is_scalar;
    using ::std::is_compound;
    using ::std::is_member_pointer;
    using ::std::is_const;
    using ::std::is_volatile;
    using ::std::is_trivial;
    using ::std::is_trivially_copyable;
    using ::std::is_standard_layout;
    using ::std::is_pod;
    using ::std::is_literal_type;
    using ::std::is_empty;
    using ::std::is_polymorphic;
    using ::std::is_abstract;
    using ::std::is_signed;
    using ::std::is_unsigned;
    using ::std::is_constructible;
    using ::std::is_default_constructible;
    using ::std::is_copy_constructible;
    using ::std::is_move_constructible;
    using ::std::is_assignable;
    using ::std::is_copy_assignable;
    using ::std::is_move_assignable;
    using ::std::is_destructible;
    using ::std::is_trivially_constructible;
    using ::std::is_trivially_default_constructible;
    using ::std::is_trivially_copy_constructible;
    using ::std::is_trivially_move_constructible;
    using ::std::is_trivially_assignable;
    using ::std::is_trivially_copy_assignable;
    using ::std::is_trivially_move_assignable;
    using ::std::is_trivially_destructible;
    using ::std::is_nothrow_constructible;
    using ::std::is_nothrow_default_constructible;
    using ::std::is_nothrow_copy_constructible;
    using ::std::is_nothrow_move_constructible;
    using ::std::is_nothrow_assignable;
    using ::std::is_nothrow_copy_assignable;
    using ::std::is_nothrow_move_assignable;
    using ::std::is_nothrow_destructible;
    using ::std::has_virtual_destructor;
    using ::std::alignment_of;
    using ::std::rank;
    using ::std::extent;
    using ::std::is_same;
    using ::std::is_base_of;
    using ::std::is_convertible;
    using ::std::remove_const;
    using ::std::remove_volatile;
    using ::std::remove_cv;
    using ::std::add_const;
    using ::std::add_volatile;
    using ::std::add_cv;
    using ::std::remove_reference;
    using ::std::add_lvalue_reference;
    using ::std::add_rvalue_reference;
    using ::std::make_signed;
    using ::std::make_unsigned;
    using ::std::remove_extent;
    using ::std::remove_all_extents;
    using ::std::remove_pointer;
    using ::std::add_pointer;
    using ::std::aligned_storage;
    using ::std::decay;
    using ::std::enable_if;
    using ::std::conditional;
    using ::std::common_type;
    using ::std::underlying_type;
    using ::std::result_of;
}

#endif
