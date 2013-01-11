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

#ifndef UP_DETAIL_CFENV_C99_INL
#define UP_DETAIL_CFENV_C99_INL

#ifndef UP_CFENV_HPP
#   error "Do not include this header directly. Instead include <up/cfenv.hpp>"
#endif

#include <fenv.h>

namespace up
{
    using ::fenv_t;
    using ::fexcept_t;

#ifdef feclearexcept
    namespace detail {
        inline UPALWAYSINLINE
        int feclearexcept_impl(int excepts) noexcept {
            return feclearexcept(excepts);
        }
    }
#   undef feclearexcept
    inline UPALWAYSINLINE
    int feclearexcept(int excepts) noexcept {
        return detail::feclearexcept_impl(excepts);
    }
#else
    using ::feclearexcept;
#endif

#ifdef fegetexceptflag
    namespace detail {
        inline UPALWAYSINLINE
        int fegetexceptflag_impl(fexcept_t* flagp, int excepts) noexcept {
            return fegetexceptflag(flagp, excepts);
        }
    }
#   undef feclearexcept
    inline UPALWAYSINLINE
    int fegetexceptflag(fexcept_t* flagp, int excepts) noexcept {
        return detail::fegetexceptflag_impl(flagp, excepts);
    }
#else
    using ::fegetexceptflag;
#endif

#ifdef feraiseexcept
    namespace detail {
        inline UPALWAYSINLINE
        int feraiseexcept_impl(int excepts) {
            return feraiseexcept(excepts);
        }
    }
#   undef feraiseexcept
    inline UPALWAYSINLINE
    int feraiseexcept(int excepts) {
        return detail::feraiseexcept_impl(excepts);
    }
#else
    using ::feraiseexcept;
#endif

#ifdef fesetexceptflag
    namespace detail {
        inline UPALWAYSINLINE
        int fesetexceptflag_impl(fexcept_t const* flagp, int excepts) noexcept {
            return fesetexceptflag(flagp, excepts);
        }
    }
#   undef fesetexceptflag
    inline UPALWAYSINLINE
    int fesetexceptflag(fexcept_t const* flagp, int excepts) noexcept {
        return detail::fesetexceptflag_impl(flagp, excepts);
    }
#else
    using ::fesetexceptflag;
#endif

#ifdef fetestexcept
    namespace detail {
        inline UPALWAYSINLINE
        int fetestexcept_impl(int excepts) noexcept {
            return fetestexcept(excepts);
        }
    }
#   undef fetestexcept
    inline UPALWAYSINLINE
    int fetestexcept(int excepts) noexcept {
        return detail::fetestexcept_impl(excepts);
    }
#else
    using ::fetestexcept;
#endif

#ifdef fegetround
    namespace detail {
        inline UPALWAYSINLINE
        int fegetround_impl() noexcept {
            return fegetround();
        }
    }
#   undef fegetround
    inline UPALWAYSINLINE
    int fegetround() noexcept {
        return detail::fegetround_impl();
    }
#else
    using ::fegetround;
#endif

#ifdef fesetround
    namespace detail {
        inline UPALWAYSINLINE
        int fesetround_impl(int round) noexcept {
            return fesetround(round);
        }
    }
#   undef fesetround    
    inline UPALWAYSINLINE
    int fesetround(int round) noexcept {
        return detail::fesetround_impl(round);
    }
#else
    using ::fesetround;
#endif

#ifdef fegetenv
    namespace detail {
        inline UPALWAYSINLINE
        int fegetenv_impl(fenv_t* envp) noexcept {
            return fegetenv(envp);
        }
    }
#   undef fegetenv
    inline UPALWAYSINLINE
    int fegetenv(fenv_t* envp) noexcept {
        return detail::fegetenv_impl(envp);
    }
#else
    using ::fegetenv;
#endif

#ifdef feholdexcept
    namespace detail {
        inline UPALWAYSINLINE
        int feholdexcept_impl(fenv_t* envp) noexcept {
            return feholdexcept(envp);
        }
    }
#   undef feholdexcept
    inline UPALWAYSINLINE
    int feholdexcept(fenv_t* envp) noexcept {
        return detail::feholdexcept_impl(envp);
    }
#else
    using ::feholdexcept;
#endif

#ifdef fesetenv
    namespace detail {
        inline UPALWAYSINLINE
        int fesetenv_impl(fenv_t const* envp) noexcept {
            return fesetenv(envp);
        }
    }
#   undef fesetenv
    inline UPALWAYSINLINE
    int fesetenv(fenv_t const* envp) noexcept {
        return detail::fesetenv_impl(envp);
    }
#else
    using ::fesetenv;
#endif

#ifdef feupdateenv
    namespace detail {
        inline UPALWAYSINLINE
        int feupdateenv_impl(fenv_t const* envp) noexcept {
            return feupdateenv(envp);
        }
    }
#   undef feupdateenv
    inline UPALWAYSINLINE
    int feupdateenv(fenv_t const* envp) noexcept {
        return detail::feupdateenv_impl(envp);
    }
#else
    using ::feupdateenv;
#endif
}

#endif
