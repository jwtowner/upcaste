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

#ifndef UP_PROLOG_APPLE_HPP
#define UP_PROLOG_APPLE_HPP

#include <TargetConditionals.h>

#ifndef UP_PLATFORM
#   undef UP_BASESYSTEM
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#       define UP_PLATFORM UP_PLATFORM_IOS
#       define UP_BASESYSTEM UP_BASESYSTEM_DARWIN
#   elif TARGET_OS_MAC
#       define UP_PLATFORM UP_PLATFORM_MACOSX
#       define UP_BASESYSTEM UP_BASESYSTEM_DARWIN
#   else
#       error "Upcaste does not support the target platform"
#   endif
#endif

#ifndef UP_ARCHITECTURE
#   if TARGET_CPU_ARM
#       define UP_ARCHITECTURE         UP_ARCHITECTURE_ARM
#       define UP_MAX_CACHE_LINE_SIZE  32
#       define UP_MIN_TLB_PAGE_SIZE    1024
#       if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7R__)
#           define UP_ARCHITECTURE_ARM_VERSION 7
#       elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) \
            || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
#           define UP_ARCHITECTURE_ARM_VERSION 6
#       elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5E__)  \
            || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
#           define UP_ARCHITECTURE_ARM_VERSION 5
#       endif
#   elif TARGET_CPU_PPC
#       define UP_ARCHITECTURE         UP_ARCHITECTURE_POWER
#       define UP_MAX_CACHE_LINE_SIZE  32
#       define UP_MIN_TLB_PAGE_SIZE    4096
#   elif TARGET_CPU_PPC64
#       define UP_ARCHITECTURE         UP_ARCHITECTURE_POWER64
#       define UP_MAX_CACHE_LINE_SIZE  128
#       define UP_MIN_TLB_PAGE_SIZE    4096
#   elif TARGET_CPU_X86
#       define UP_ARCHITECTURE         UP_ARCHITECTURE_X86
#       define UP_MAX_CACHE_LINE_SIZE  64
#       define UP_MIN_TLB_PAGE_SIZE    4096
#   elif TARGET_CPU_X86_64
#       define UP_ARCHITECTURE         UP_ARCHITECTURE_X64
#       define UP_MAX_CACHE_LINE_SIZE  64
#       define UP_MIN_TLB_PAGE_SIZE    4096
#   else
#       error "Upcaste does not support the target CPU architecture"
#   endif
#endif

#if !defined(UP_ARCHITECTURE_32BIT) && !defined(UP_ARCHITECTURE_64BIT)
#   if TARGET_RT_64_BIT
#       define UP_ARCHITECTURE_64BIT
#   else
#       define UP_ARCHITECTURE_32BIT
#   endif
#endif

#if !defined(UP_BYTE_ORDER)
#   if TARGET_RT_LITTLE_ENDIAN
#       define UP_BYTE_ORDER UP_LITTLE_ENDIAN
#   elif TARGET_RT_BIG_ENDIAN
#       define UP_BYTE_ORDER UP_BIG_ENDIAN
#   else
#       error "Upcaste cannot detect the endianess of the target CPU architecture"
#   endif
#endif

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_ARM)
#   if !defined(UP_SIMD_NEON) && defined(__ARM_NEON__)
#       define UP_SIMD_NEON
#   if !defined(UP_SIMD_VFP) && (defined(UP_SIMD_NEON) || defined(UP_ARCHITECTURE_ARM_V7) || defined(UP_ARCHITECTURE_ARM_V6))
#       define UP_SIMD_VFP
#   endif
#elif (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#   if !defined(UP_SIMD_XOP) && defined(__FMA4__)
#       define UP_SIMD_FMA4
#   endif
#   if !defined(UP_SIMD_XOP) && defined(__XOP__)
#       define UP_SIMD_XOP
#   endif
#   if !defined(UP_SIMD_AVX) && (defined(UP_SIMD_XOP) || defined(UP_SIMD_FMA4) || defined(__AVX__))
#       define UP_SIMD_AVX
#   endif
#   if !defined(UP_SIMD_SSE_4_2) && (defined(UP_SIMD_AVX) || defined(__SSE4_2__))
#       define UP_SIMD_SSE_4_2
#   endif
#   if !defined(UP_SIMD_SSE_4_1) && (defined(UP_SIMD_SSE_4_2) || defined(__SSE4_1__))
#       define UP_SIMD_SSE_4_1
#   endif
#   if !defined(UP_SIMD_SSE_3) && (defined(UP_SIMD_SSE_4_1) || defined(__SSE3__))
#       define UP_SIMD_SSE_3
#   endif
#   if !defined(UP_SIMD_SSE_2) && (defined(UP_SIMD_SSE_3) || defined(__SSE2__))
#       define UP_SIMD_SSE_2
#   endif
#   if !defined(UP_SIMD_SSE) && (defined(UP_SIMD_SSE_2) || defined(__SSE__))
#       define UP_SIMD_SSE
#   endif
#endif

#ifndef UP_STDC_EXTENSIONS
#   define UP_STDC_EXTENSIONS UP_STDC_EXTENSIONS_POSIX
#endif

#if !defined(UP_FILESYSTEM)
#   define UP_FILESYSTEM UP_FILESYSTEM_POSIX
#endif

#if !defined(UP_SOCKETS)
#   define UP_SOCKETS UP_SOCKETS_BERKELEY
#endif

#if !defined(UP_NO_IPV6_SUPPORT)
#   ifdef TARGET_OS_IPHONE
#       define UP_NO_IPV6_SUPPORT
#   endif
#endif

#endif
