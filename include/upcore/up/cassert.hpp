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

#ifndef UP_CASSERT_HPP
#define UP_CASSERT_HPP

#include <up/prolog.hpp>

namespace up
{
    namespace detail
    {
        template <unsigned long long> struct static_assert_test { };
        template <bool> struct static_assert_error;
        template <> struct static_assert_error<true> { enum { value = true }; };
    }

    typedef bool (UPCDECL *assert_handler)(char const* file, long line, char const* condition, char const* message);

    extern LIBUPCOREAPI UPNONNULLALL
    bool UPCDECL default_assert_handler(char const* file, long line, char const* condition, char const* message);

    extern LIBUPCOREAPI UPWARNRESULT
    assert_handler get_assert_handler();

    extern LIBUPCOREAPI
    assert_handler set_assert_handler(assert_handler handler);

    extern LIBUPCOREAPI UPNONNULLALL
    bool report_assertion(char const* file, long line, char const* condition);

    extern LIBUPCOREAPI UPNONNULLALL
    bool report_assertion(char const* file, long line, char const* condition, char const* message, ...);
}

#define UP_DETAIL_REQUIRE_IMPL(condition, ...) { if (!(condition) && ::up::report_assertion(__FILE__, __LINE__, UPSTRINGIZE(condition), __VA_ARGS__)) { UPABORT(); } }
#define UP_DETAIL_REQUIRE_APPROX_IMPL(x, y, d, ...) UP_DETAIL_REQUIRE_IMPL((((x < y) ? (y - x) : (x - y)) <= (d)), __VA_ARGS__)
#define UP_DETAIL_ASSERT_IMPL(condition, ...) UPASSUME(!!(condition))
#define UP_DETAIL_ASSERT_APPROX_IMPL(x, y, d, ...) UPASSUME(!!(((x < y) ? (y - x) : (x - y)) <= (d)))
#define UP_DETAIL_VERIFY_IMPL(condition, ...) ((void)(condition))
#define UP_DETAIL_VERIFY_APPROX_IMPL(x, y, d, ...) UP_DETAIL_VERIFY_IMPL((((x < y) ? (y - x) : (x - y)) <= (d)), __VA_ARGS__)
#define UPREQUIRE(...) UP_DETAIL_REQUIRE_IMPL(__VA_ARGS__)
#define UPREQUIRE_THROW(condition, action) { if (!(condition) && ::up::report_assertion(__FILE__, __LINE__, UPSTRINGIZE(condition), UPSTRINGIZE(action))) { UPTHROW(action); } }
#define UPREQUIRE_APPROX(x, y, d, ...) UP_DETAIL_REQUIRE_APPROX_IMPL(x, y, d, __VA_ARGS__)
#define UPREQUIRE_APPROX_THROW(x, y, d, action) UP_REQUIRE_THROW((((x < y) ? (y - x) : (x - y)) <= (d)), action)

#ifndef UP_NO_NAKED_ASSERT
#   define require(...) UPREQUIRE(__VA_ARGS__)
#   define require_throw(condition, action) UPREQUIRE_THROW(condition, action)
#   define require_approx(x, y, d, ...) UPREQUIRE_APPROX(x, y, d, __VA_ARGS__)
#   define require_approx_throw(x, y, d, action) UPREQUIRE_APPROX_THROW(x, y, d, action)
#endif

#define UPSTATIC_ASSERT(expression, message) \
    typedef ::up::detail::static_assert_test<sizeof(::up::detail::static_assert_error<(expression) ? true : false>)> \
    UPCONCATENATE(static_assert_line_, __LINE__)

#if defined(UP_NO_STATIC_ASSERT) && !defined(static_assert)
#   define static_assert UPSTATIC_ASSERT
#endif

#endif

#include <cassert>

#undef UPASSERT
#undef UPASSERT_THROW
#undef UPASSERT_APPROX
#undef UPASSERT_APPROX_THROW
#undef UPVERIFY
#undef UPVERIFY_THROW
#undef UPVERIFY_APPROX
#undef UPVERIFY_APPROX_THROW

#ifndef NDEBUG
#   define UPASSERT(...) UPREQUIRE(__VA_ARGS__)
#   define UPASSERT_THROW(condition, action) UPREQUIRE_THROW(condition, action)
#   define UPASSERT_APPROX(x, y, d, ...) UPREQUIRE_APPROX(x, y, d, __VA_ARGS__)
#   define UPASSERT_APPROX_THROW(x, y, d, action) UPREQUIRE_APPROX_THROW(x, y, d, action)
#   define UPVERIFY(...) UPREQUIRE(__VA_ARGS__)
#   define UPVERIFY_THROW(condition, action) UPREQUIRE_THROW(condition, action)
#   define UPVERIFY_APPROX(x, y, d, ...) UPREQUIRE_APPROX(x, y, d, __VA_ARGS__)
#   define UPVERIFY_APPROX_THROW(x, y, d, action) UPREQUIRE_APPROX_THROW(x, y, d, action)
#else
#   define UPASSERT(...) UP_DETAIL_ASSERT_IMPL(__VA_ARGS__)
#   define UPASSERT_THROW(condition, action) { if (!(condition)) { UPTHROW(action); } }
#   define UPASSERT_APPROX(...) UP_DETAIL_ASSERT_APPROX_IMPL(__VA_ARGS__)
#   define UPASSERT_APPROX_THROW(x, y, d, action) UPASSERT_THROW((((x < y) ? (y - x) : (x - y)) <= (d)), action)
#   define UPVERIFY(...) UP_DETAIL_VERIFY_IMPL(__VA_ARGS__)
#   define UPVERIFY_THROW(condition, action) { if (!(condition)) { UPTHROW(action); } }
#   define UPVERIFY_APPROX(...) UP_DETAIL_VERIFY_APPROX_IMPL(__VA_ARGS__)
#   define UPVERIFY_APPROX_THROW(x, y, d, action) UPVERIFY_THROW((((x < y) ? (y - x) : (x - y)) <= (d)), action)
#endif

#ifndef UP_NO_NAKED_ASSERT
#   undef assert
#   undef assert_throw
#   undef assert_approx
#   undef assert_approx_throw
#   undef verify
#   undef verify_throw
#   undef verify_approx
#   undef verify_approx_throw
#   define assert(...) UPASSERT(__VA_ARGS__)
#   define assert_throw(condition, action) UPASSERT_THROW(condition, action)
#   define assert_approx(x, y, d, ...) UPASSERT_APPROX(x, y, d, __VA_ARGS__)
#   define assert_approx_throw(x, y, d, action) UPASSERT_APPROX_THROW(x, y, d, action)
#   define verify(...) UPVERIFY(__VA_ARGS__)
#   define verify_throw(condition, action) UPVERIFY_THROW(condition, action)
#   define verify_approx(x, y, d, ...) UPVERIFY_APPROX(x, y, d, __VA_ARGS__)
#   define verify_approx_throw(x, y, d, action) UPVERIFY_APPROX_THROW(x, y, d, action)
#endif
