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

#ifndef UP_PROLOG_LINUX_HPP
#define UP_PROLOG_LINUX_HPP

#ifndef UP_PLATFORM
#   undef UP_BASESYSTEM
#   if defined(__linux) || defined(__linux__)
#       define UP_PLATFORM UP_PLATFORM_LINUX
#       define UP_BASESYSTEM UP_BASESYSTEM_LINUX
#   else
#       error "Upcaste does not support the target platform!"
#   endif
#endif

#ifndef UP_ARCHITECTURE
#   if defined(__arm) || defined(__arm__)
#       define UP_ARCHITECTURE UP_ARCHITECTURE_ARM
#   elif defined(__i386) || defined(__i386__)
#       define UP_ARCHITECTURE UP_ARCHITECTURE_X86
#   elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)
#       define UP_ARCHITECTURE UP_ARCHITECTURE_X64
#   else
#       error "Upcaste does not support the target CPU architecture!"
#   endif
#endif

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_ARM) && !defined(UP_ARCHITECTURE_ARM_V7) \
    && !defined(UP_ARCHITECTURE_ARM_V6) && !defined(UP_ARCHITECTURE_ARM_V5)
#   if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) \
        || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7R__)
#       define UP_ARCHITECTURE_ARM_V7
#   elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) \
        || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
#       define UP_ARCHITECTURE_ARM_V6
#   elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5E__)  \
        || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
#       define UP_ARCHITECTURE_ARM_V5
#   endif
#endif

#if !defined(UP_ARCHITECTURE_32BIT) && !defined(UP_ARCHITECTURE_64BIT)
#   if UP_ARCHITECTURE == UP_ARCHITECTURE_ARM
#       define UP_ARCHITECTURE_32BIT
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X86
#       define UP_ARCHITECTURE_32BIT
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X64
#       define UP_ARCHITECTURE_64BIT
#   endif
#endif

#ifndef UP_BYTE_ORDER
#   define UP_BYTE_ORDER UP_LITTLE_ENDIAN
#endif

#ifndef UP_EXPENSIVE_BRANCHING
#   if UP_ARCHITECTURE == UP_ARCHITECTURE_ARM
#       define UP_EXPENSIVE_BRANCHING
#   endif
#endif

#ifndef UP_MAX_CACHE_LINE_SIZE
#   if UP_ARCHITECTURE == UP_ARCHITECTURE_ARM
#       define UP_MAX_CACHE_LINE_SIZE 32
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X86
#       define UP_MAX_CACHE_LINE_SIZE 64
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X64
#       define UP_MAX_CACHE_LINE_SIZE 64
#   endif
#endif

#ifndef UP_MIN_TLB_PAGE_SIZE
#   if UP_ARCHITECTURE == UP_ARCHITECTURE_ARM
#       define UP_MIN_TLB_PAGE_SIZE 1024
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X86
#       define UP_MIN_TLB_PAGE_SIZE 4096
#   elif UP_ARCHITECTURE == UP_ARCHITECTURE_X64
#       define UP_MIN_TLB_PAGE_SIZE 4096
#   endif
#endif

#if (UP_ARCHITECTURE == UP_ARCHITECTURE_ARM)
#   if !defined(UP_SIMD_NEON) && defined(__ARM_NEON__)
#       define UP_SIMD_NEON
#   elif !defined(UP_SIMD_VFP) && (defined(UP_SIMD_NEON) || defined(UP_ARCHITECTURE_ARM_V7) || defined(UP_ARCHITECTURE_ARM_V6))
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
#   if !defined(UP_SIMD_SSSE_3) && (defined(UP_SIMD_SSE_4_1) || defined(__SSSE3__))
#       define UP_SIMD_SSSE_3
#   endif
#   if !defined(UP_SIMD_SSE_3) && (defined(UP_SIMD_SSSE_3) || defined(__SSE3__))
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

#ifndef UP_FILESYSTEM
#   define UP_FILESYSTEM UP_FILESYSTEM_POSIX
#endif

#ifndef UP_SOCKETS
#   define UP_SOCKETS UP_SOCKETS_BERKELEY
#endif

#endif
