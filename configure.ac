#############################################################################
#
# Upcaste Performance Libraries
# Copyright (C) 2012-2013 Jesse W. Towner
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#############################################################################

AC_INIT([Upcaste Performance Libraries],[0.1],[jessetowner@lavabit.com],[upcaste],[http://github.com/upcaste/upcaste])
AC_PREREQ([2.67])
AC_CANONICAL_TARGET
AC_CONFIG_HEADERS([include/upcore/up/prolog/target.hpp:project/tundra/target.hpp.in])
AC_LANG([C++])

#############################################################################
# General
#############################################################################

AC_PROG_CXX

AS_CASE(
    [$build_os],
    [*linux*],
    [build_cpu_count=`grep processor /proc/cpuinfo | wc -l`],
    [build_cpu_count="1"]
)

#############################################################################
# Dependencies
#############################################################################

dependency_warnings=""
dependency_target_directories=""
dependency_root_directory="dependency"
dependency_download_directory="${dependency_root_directory}/download"
dependency_patch_directory="${dependency_root_directory}/patch"
dependency_src_directory="${dependency_root_directory}/src"
dependency_usr_directory="${dependency_root_directory}/usr"

AC_CHECK_PROG(have_bzip2, bzip2, yes, no)
AC_CHECK_PROG(have_gzip, gzip, yes, no)
AC_CHECK_PROG(have_tar, tar, yes, no)
AC_CHECK_PROG(have_wget, wget, yes, no)
AC_CHECK_PROG(have_tundra, tundra, yes, no)

AS_IF([test "x$have_tundra" != "xyes"],
    [AC_MSG_ERROR([The Tundra Build Processor 1.2 or later is required to build Upcaste. You can fetch the sources from: https://github.com/deplinenoise/tundra])])

AS_IF([test "x$have_wget" != "xyes"],
    [AC_MSG_ERROR([Please install wget so that third-party dependencies may be downloaded.])])

AS_IF([test "x$have_tar" != "xyes"],
    [AC_MSG_ERROR([Please install tar so that third-party dependencies may be unarchived.])])

AS_IF([test "x$have_bzip2" != "xyes"],
    [AC_MSG_ERROR([Please install bzip2 so that third-party dependencies may be decompressed.])])

AS_IF([test "x$have_gzip" != "xyes"],
    [AC_MSG_ERROR([Please install gzip so that third-party dependencies may be decompressed.])])

AC_ARG_ENABLE(
    [local-packages],
    [AS_HELP_STRING([--enable-local-packages],[Build and use certain third-party packages locally for Hurricane.
        By default, the configuration process will rely on dependencies
        being already installed on the system. By enabling this option, the
        configuration process will download, build and link against private
        copies of these packages, which may be useful to ensure long-term
        software stability across package versions and multiple operating
        system distributions. Affected packages include libglew, libjpeg,
        liblzma, libpng, libsdl, libz.])])

AC_DEFUN(
    [UP_INSTALL_LOCAL_DEPENDENCY],
    [[
        archivefilename=`basename $1`
        if test $archivefilename != "${archivefilename/.tar.}" ; then
            archivebase="${archivefilename%.tar.*}"
            archiveext="tar.${archivefilename#*.tar.}"
        else
            archivebase="${archivefilename%.*}"
            archiveext="${archivefilename##*.}"
        fi
        archivesrcdir="${dependency_src_directory}/${archivebase}"
        if test "x$fileext" = "xtar.bz2" ; then
            tar_flags="-xjf"
        elif test "x$fileext" = "xtar.gz" -o "x$fileext" = "xtgz" ; then
            tar_flags="-xzf"
        elif test "x$fileext" = "xtar.xz" ; then
            tar_flags="-xJf"
        else
            tar_flags="-xf"
        fi
        rm -rf $dependency_usr_directory
        mkdir -p $dependency_download_directory
        mkdir -p $dependency_src_directory
        (cd $dependency_src_directory && exec rm -rf $archivebase)
        echo "downloading $archivefilename"
        (cd $dependency_download_directory && exec wget --timestamping $1)
        echo "decompressing $archivefilename"
        (cd $dependency_src_directory && exec tar --overwrite $tar_flags "../download/${archivefilename}")
        if test ! -d $archivesrcdir ; then
            (cd $dependency_src_directory && exec find . -type d -name $archivebase* | xargs -I{} mv {} $archivebase)
        fi
        echo "patching $archivebase"
        (cd $archivesrcdir && exec find "../../patch" -type f -name $archivebase* | xargs -I{} -t patch --verbose -p1 -i "{}")
        dependency_target_directories="${dependency_target_directories} ${archivesrcdir}"
    ]])

AC_DEFUN(
    [UP_WARN_IF_MISSING_DEPENDENCY],
    [[
        if test "x$1" != "xyes" ; then
            dependency_warnings="${dependency_warnings}\nconfigure warning: $2 is unavailable. Dependent targets will not be built!"
        fi
    ]])

AC_DEFUN(
    [UP_SHOW_DEPENDENCY_WARNINGS],
    [[
        if test -n "${dependency_warnings}" ; then
            echo -e "${dependency_warnings}\n"
        fi
    ]])

#############################################################################
# Standard C/C++ Library Headers
#############################################################################

AC_MSG_NOTICE([------ Standard C/C++ Library Headers ------])

# UP_HAS_STDC_INTTYPES

AC_CHECK_HEADER(
    [inttypes.h],
    [AC_MSG_CHECKING([for inttypes.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <inttypes.h> ]],
            [
                imaxabs(-5);
                imaxdiv(1000, 50);
                strtoimax("123", 0, 0);
                strtoumax("123", 0, 0);
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_INTTYPES)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_LOCALE

AC_CHECK_HEADER(
    [locale.h],
    [AC_MSG_CHECKING([for locale.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <locale.h> ]],
            [
                setlocale(LC_ALL, "C");
                setlocale(LC_COLLATE, "C");
                setlocale(LC_CTYPE, "C");
                setlocale(LC_MONETARY, "C");
                setlocale(LC_NUMERIC, "C");
                setlocale(LC_TIME, "C");
                localeconv();
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_LOCALE)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_MATH_C99

AC_CHECK_HEADER(
    [math.h],
    [AC_MSG_CHECKING([for math.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <math.h> ]],
            [
                acosh(1.0);
                asinh(1.0);
                atanh(1.0);
                erf(1.0);
                erfc(1.0);
                lgamma(1.0);
                tgamma(1.0);
                nan("1234");
                fpclassify(-1241.0);
                signbit(-1.0);
                rint(-66.43);
                round(44.6);
                trunc(612.512);
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_MATH_C99)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_THREADS

AC_CHECK_HEADER(
    [threads.h],
    [AC_MSG_CHECKING([for threads.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <threads.h> ]],
            [
                thrd_yield();
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_THREADS)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_TIME

AC_CHECK_HEADER(
    [time.h],
    [AC_MSG_CHECKING([for time.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <time.h> ]],
            [
                clock();
                difftime(time_t(), time_t());
                mktime(0);
                time(0);
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_TIME)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_UCHAR

AC_CHECK_HEADER(
    [uchar.h],
    [AC_MSG_CHECKING([for uchar.h compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [[ #include <uchar.h> ]],
            [
                mbrtoc16(0, 0, 0, 0);
                mbrtoc32(0, 0, 0, 0);
                c16rtomb(0, 0, 0);
                c32rtomb(0, 0, 0);
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_UCHAR)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDC_WCHAR

AC_CHECK_HEADER([wchar.h])
AC_CHECK_HEADER([wctype.h])

AS_IF([test "${ac_cv_header_wchar_h+set}" = "set" -a "${ac_cv_header_wctype_h+set}" = "set"],
    [AC_MSG_CHECKING([for wchar_t compliance])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
            [
                #include <wctype.h>
                #include <wchar.h>
                #include <stdlib.h>
            ],
            [
                iswspace(L' ');
                wcslen(L"hello");
                wcstombs(0, 0, 0);
            ]
        )],
        [AC_MSG_RESULT(yes)
        AC_DEFINE(UP_HAS_STDC_WCHAR)],
        [AC_MSG_RESULT(no)])])

# UP_HAS_STDCXX_ATOMIC

AC_MSG_CHECKING([for C++11 <atomic> compliance])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <atomic> ]],
        [
            using std::kill_dependency;
            using std::memory_order;
            using std::atomic_signal_fence;
            using std::atomic_thread_fence;
            using std::atomic;
            using std::atomic_flag;
            using std::atomic_bool;
            using std::atomic_char;
            using std::atomic_schar;
            using std::atomic_uchar;
            using std::atomic_short;
            using std::atomic_ushort;
            using std::atomic_int;
            using std::atomic_uint;
            using std::atomic_long;
            using std::atomic_ulong;
            using std::atomic_llong;
            using std::atomic_ullong;
            using std::atomic_char16_t;
            using std::atomic_char32_t;
            using std::atomic_wchar_t;
            using std::atomic_int8_t;
            using std::atomic_uint8_t;
            using std::atomic_int16_t;
            using std::atomic_uint16_t;
            using std::atomic_int32_t;
            using std::atomic_uint32_t;
            using std::atomic_int64_t;
            using std::atomic_uint64_t;
            using std::atomic_int_least8_t;
            using std::atomic_uint_least8_t;
            using std::atomic_int_least16_t;
            using std::atomic_uint_least16_t;
            using std::atomic_int_least32_t;
            using std::atomic_uint_least32_t;
            using std::atomic_int_least64_t;
            using std::atomic_uint_least64_t;
            using std::atomic_int_fast8_t;
            using std::atomic_uint_fast8_t;
            using std::atomic_int_fast16_t;
            using std::atomic_uint_fast16_t;
            using std::atomic_int_fast32_t;
            using std::atomic_uint_fast32_t;
            using std::atomic_int_fast64_t;
            using std::atomic_uint_fast64_t;
            using std::atomic_intptr_t;
            using std::atomic_uintptr_t;
            using std::atomic_size_t;
            using std::atomic_ptrdiff_t;
            using std::atomic_intmax_t;
            using std::atomic_uintmax_t;
            using std::atomic_thread_fence;
            using std::atomic_signal_fence;
            using std::atomic_init;
            using std::atomic_is_lock_free;
            using std::atomic_store;
            using std::atomic_store_explicit;
            using std::atomic_load;
            using std::atomic_load_explicit;
            using std::atomic_exchange;
            using std::atomic_exchange_explicit;
            using std::atomic_compare_exchange_strong;
            using std::atomic_compare_exchange_strong_explicit;
            using std::atomic_compare_exchange_weak;
            using std::atomic_compare_exchange_weak_explicit;
            using std::atomic_fetch_add;
            using std::atomic_fetch_add_explicit;
            using std::atomic_fetch_sub;
            using std::atomic_fetch_sub_explicit;
            using std::atomic_fetch_or;
            using std::atomic_fetch_or_explicit;
            using std::atomic_fetch_xor;
            using std::atomic_fetch_xor_explicit;
            using std::atomic_fetch_and;
            using std::atomic_fetch_and_explicit;
            using std::atomic_flag_test_and_set;
            using std::atomic_flag_test_and_set_explicit;
            using std::atomic_flag_clear;
            using std::atomic_flag_clear_explicit;            
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDCXX_ATOMIC)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDCXX_TYPE_TRAITS

AC_MSG_CHECKING([for C++11 <type_traits> compliance])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <type_traits> ]],
        [
            using std::is_trivial;
            using std::is_trivially_destructible;
            using std::is_nothrow_copy_constructible;
            using std::is_nothrow_copy_assignable;
            using std::is_nothrow_move_constructible;
            using std::is_nothrow_move_assignable;
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDCXX_TYPE_TRAITS)],
    [AC_MSG_RESULT(no)])


#############################################################################
# Standard C/C++ Library Features
#############################################################################

AC_MSG_NOTICE([------ Standard C/C++ Library Features ------])

# UP_HAS_STDC_ALLOCA

AC_MSG_CHECKING([for alloca])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ void* p = alloca(256); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_ALLOCA)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_ALIGNED_ALLOC

AC_MSG_CHECKING([for aligned_alloc])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ void* p = aligned_alloc(16, 256); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_ALIGNED_ALLOC)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_MAX_ALIGN

AC_MSG_CHECKING([for max_align_t])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stddef.h> ]],
        [[ max_align_t x; ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_MAX_ALIGN)],
    [AC_MSG_RESULT(no)])


# UP_HAS_STDC_SNPRINTF

AC_MSG_CHECKING([for snprintf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [[ snprintf(0, 0, "foo %d bar", 42); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_SNPRINTF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_STRTOF

AC_MSG_CHECKING([for strtof])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ strtof("44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_STRTOF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_STRTOD

AC_MSG_CHECKING([for strtod])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ strtod("44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_STRTOD)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_STRTOLD

AC_MSG_CHECKING([for strtold])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ strtold("44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_STRTOLD)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_STRTOLL

AC_MSG_CHECKING([for strtoll])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ strtoll("44", 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_STRTOLL)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_STRTOULL

AC_MSG_CHECKING([for strtoull])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ strtoull("44", 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_STRTOULL)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_QUICK_EXIT

AC_MSG_CHECKING([for at_quick_exit and quick_exit])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [
            at_quick_exit(NULL);
            quick_exit(EXIT_SUCCESS);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_QUICK_EXIT)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_TIMESPEC_GET

AC_MSG_CHECKING([for timespec_get])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <time.h> ]],
        [[ timespec_get(0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_TIMESPEC_GET)],
    [AC_MSG_RESULT(no)])


# UP_HAS_STDC_UNSAFE_EXIT

AC_MSG_CHECKING([for _Exit])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ _Exit(EXIT_SUCCESS); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_UNSAFE_EXIT)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_WCSTOF

AC_MSG_CHECKING([for wcstof])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcstof(L"44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_WCSTOF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_WCSTOD

AC_MSG_CHECKING([for wcstod])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcstod(L"44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_WCSTOD)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_WCSTOLD

AC_MSG_CHECKING([for wcstold])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcstold(L"44e+12", 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_WCSTOLD)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_WCSTOLL

AC_MSG_CHECKING([for wcstoll])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcstoll(L"44", 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_WCSTOLL)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_WCSTOULL

AC_MSG_CHECKING([for wcstoull])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcstoull(L"44", 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_WCSTOULL)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_VFSCANF

AC_MSG_CHECKING([for vfscanf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [
            va_list ap;
            vfscanf(0, "upcaste", ap);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_VFSCANF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_VSCANF

AC_MSG_CHECKING([for vscanf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [
            va_list ap;
            vscanf("upcaste", ap);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_VSCANF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_VSSCANF

AC_MSG_CHECKING([for vsscanf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [
            va_list ap;
            vsscanf("upcaste", "upcaste", ap);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_VSSCANF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_VSNPRINTF

AC_MSG_CHECKING([for vsnprintf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [
            va_list ap;
            vsnprintf(0, 0, "upcaste", ap);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_STDC_VSNPRINTF)],
    [AC_MSG_RESULT(no)])

#############################################################################
# POSIX Library
#############################################################################

AC_MSG_NOTICE([------ POSIX Base Specification ------])

# UP_HAS_POSIX_THREADS

AC_MSG_CHECKING([for posix threads])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <pthread.h> ]],
        [[ pthread_create(0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_THREADS)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_PTHREAD_MUTEX_TIMEDLOCK

AC_MSG_CHECKING([for pthread_mutex_timedlock])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <pthread.h> ]],
        [[ pthread_mutex_timedlock(0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_PTHREAD_MUTEX_TIMEDLOCK)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_PTHREAD_MUTEX_ADAPTIVE_NP

AC_MSG_CHECKING([for PTHREAD_MUTEX_ADAPTIVE_NP])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <pthread.h> ]],
        [[ int x = PTHREAD_MUTEX_ADAPTIVE_NP; ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_PTHREAD_MUTEX_ADAPTIVE_NP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_PTHREAD_MUTEX_ERRORCHECK

AC_MSG_CHECKING([for PTHREAD_MUTEX_ERRORCHECK])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <pthread.h> ]],
        [[ int x = PTHREAD_MUTEX_ERRORCHECK; ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_PTHREAD_MUTEX_ERRORCHECK)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_PTHREAD_MUTEX_TIMED_NP

AC_MSG_CHECKING([for PTHREAD_MUTEX_TIMED_NP])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <pthread.h> ]],
        [[ int x = PTHREAD_MUTEX_TIMED_NP; ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_PTHREAD_MUTEX_TIMED_NP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_LOCALE

AC_MSG_CHECKING([for posix locale extensions])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <locale.h>
        ],
        [
            newlocale(LC_ALL, "C", 0);
            freelocale(0);
            uselocale(0);
            duplocale(0);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_LOCALE)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_ASPRINTF

AC_MSG_CHECKING([for asprintf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [[ asprintf(0, "upcaste"); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_ASPRINTF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_STDC_VASPRINTF

AC_MSG_CHECKING([for vasprintf])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <stdarg.h>
            #include <stdio.h>
        ],
        [
            va_list ap;
            vasprintf(0, "upcaste", ap);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_VASPRINTF)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_FSEEKO

AC_MSG_CHECKING([for fseeko])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [[ fseeko(0, 0, SEEK_SET); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_FSEEKO)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_FTELLO

AC_MSG_CHECKING([for ftello])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [[ ftello(0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_FTELLO)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_GETDELIM

AC_MSG_CHECKING([for getdelim])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [[ getdelim(0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_GETDELIM)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_GETLINE

AC_MSG_CHECKING([for getline])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [[ getline(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_GETLINE)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_GETWDELIM

AC_MSG_CHECKING([for getwdelim])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ getwdelim(0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_GETWDELIM)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_GETLINE

AC_MSG_CHECKING([for getwline])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ getwline(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_GETWLINE)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_MEMALIGN

AC_MSG_CHECKING([for memalign])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdlib.h> ]],
        [[ int r = posix_memalign(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_MEMALIGN)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_MEMSTREAM

AC_MSG_CHECKING([for fmemopen])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <stdio.h> ]],
        [ 
            fmemopen(0, 0, "wb");
            open_memstream(0, 0);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_MEMSTREAM)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_MEMCCPY

AC_MSG_CHECKING([for memccpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ memccpy(0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_MEMCCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WMEMCCPY

AC_MSG_CHECKING([for wmemccpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wmemccpy(0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WMEMCCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_MBSNRTOWCS

AC_MSG_CHECKING([for mbsnrtowcs])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ mbsnrtowcs(0, 0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_MBSNRTOWCS)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCSNRTOMBS

AC_MSG_CHECKING([for wcsnrtombs])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcsnrtombs(0, 0, 0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCSNRTOMBS)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_SSIZE

AC_MSG_CHECKING([for ssize_t])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <sys/types.h> ]],
        [[ ssize_t x = 0; ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_SSIZE)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STPCPY

AC_MSG_CHECKING([for stpcpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ stpcpy(0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STPCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STPNCPY

AC_MSG_CHECKING([for stpncpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ stpncpy(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STPNCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STRDUP

AC_MSG_CHECKING([for strdup])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strdup("upcaste"); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STRDUP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STRCASECMP

AC_MSG_CHECKING([for strcasecmp])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strcasecmp("upcaste", "upcaste"); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STRCASECMP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STRNCASECMP

AC_MSG_CHECKING([for strncasecmp])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strncasecmp("upcaste", "upcaste", 7); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STRNCASECMP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STRNLEN

AC_MSG_CHECKING([for strnlen])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strnlen("upcaste", 7); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STRNLEN)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_STRTOK_R

AC_MSG_CHECKING([for strtok_r]) 
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strtok_r(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_STRTOK_R)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCPCPY

AC_MSG_CHECKING([for wcpcpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcpcpy(0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCPCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCPNCPY

AC_MSG_CHECKING([for wcpncpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcpncpy(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCPNCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCSDUP

AC_MSG_CHECKING([for wcsdup])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcsdup(L"upcaste"); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCSDUP)],
    [AC_MSG_RESULT(no)])
    
# UP_HAS_POSIX_WCSCASECMP

AC_MSG_CHECKING([for wcscasecmp])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcscasecmp(L"upcaste", L"upcaste"); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCSCASECMP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCSNCASECMP

AC_MSG_CHECKING([for wcsncasecmp])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcsncasecmp(L"upcaste", L"upcaste", 7); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCSNCASECMP)],
    [AC_MSG_RESULT(no)])

# UP_HAS_POSIX_WCSNLEN

AC_MSG_CHECKING([for wcsnlen])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcsnlen(L"hello world", 11); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_POSIX_WCSNLEN)],
    [AC_MSG_RESULT(no)])

#############################################################################
# The GNU C Library
#############################################################################

AC_MSG_NOTICE([------ GNU C Library Functions ------])

# UP_HAS_GNU_MEMPCPY

AC_MSG_CHECKING([for mempcpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ mempcpy(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_GNU_MEMPCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_GNU_WMEMPCPY

AC_MSG_CHECKING([for wmempcpy])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wmempcpy(0, 0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_GNU_WMEMPCPY)],
    [AC_MSG_RESULT(no)])

# UP_HAS_GNU_STRNDUP

AC_MSG_CHECKING([for strndup])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [[ strndup(0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_GNU_STRNDUP)],
    [AC_MSG_RESULT(no)])
 
# UP_HAS_GNU_WCSNDUP

AC_MSG_CHECKING([for wcsndup])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <wchar.h> ]],
        [[ wcsndup(0, 0); ]]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_GNU_WCSNDUP)],
    [AC_MSG_RESULT(no)])

#############################################################################
# BSD LIBC Library
#############################################################################

AC_MSG_NOTICE([------ BSD C Library Functions ------])

# UP_HAS_BSD_MEMSET_PATTERN

AC_MSG_CHECKING([for memset_pattern])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
        [[ #include <string.h> ]],
        [
            memset_pattern4(0, 0, 0);
            memset_pattern8(0, 0, 0);
            memset_pattern16(0, 0, 0);
        ]
    )],
    [AC_MSG_RESULT(yes)
    AC_DEFINE(UP_HAS_BSD_MEMSET_PATTERN)],
    [AC_MSG_RESULT(no)])


#############################################################################
# Heterogeneous Compute
#############################################################################

AC_MSG_NOTICE([------ Heterogeneous Compute ------])

have_opencl=yes
AC_CHECK_HEADER([CL/opencl.h], [], [have_opencl=no])
AC_CHECK_LIB([OpenCL], [clCreateSubBuffer, clSetMemObjectDestructorCallback], [], [have_opencl=no])
UP_WARN_IF_MISSING_DEPENDENCY([$have_opencl], [OpenCL 1.1])

#############################################################################
# Output
#############################################################################

AC_SUBST([build_cpu_count])
AC_SUBST([dependency_usr_directory])
AC_SUBST([dependency_target_directories])
AC_CONFIG_FILES([Makefile:project/tundra/Makefile.in tundra.lua:project/tundra/tundra.lua.in])
AC_OUTPUT

#############################################################################
# Dependency Configuration
#############################################################################

AC_MSG_NOTICE([------ Dependency Configuration ------])

UP_SHOW_DEPENDENCY_WARNINGS

AC_MSG_NOTICE([------ Finished ------])

