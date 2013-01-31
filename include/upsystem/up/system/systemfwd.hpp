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

#ifndef UP_SYSTEMFWD_HPP
#define UP_SYSTEMFWD_HPP

#ifdef __APPLE__
#   include <OpenCL/cl.h>
#   include <OpenCL/cl_ext.h>
#else
#   include <CL/cl.h>
#   include <CL/cl_ext.h>
#endif

#include <up/cfloat.hpp>
#include <up/cstddef.hpp>
#include <up/cstdint.hpp>

#ifdef UP_LINKAGE_SHARED
#   ifdef LIBUPSYSTEM_EXPORT_SYMBOLS
#       define LIBUPSYSTEMAPI UPEXPORT
#       define LIBUPSYSTEMEXTERN UPEXPORTEXTERN
#       define LIBUPSYSTEMEXCEPTAPI UPEXPORTEXCEPT
#   else
#       define LIBUPSYSTEMAPI UPIMPORT
#       define LIBUPSYSTEMEXTERN UPIMPORTEXTERN
#       define LIBUPSYSTEMEXCEPTAPI UPIMPORTEXCEPT
#   endif
#else
#   define LIBUPSYSTEMAPI
#   define LIBUPSYSTEMEXTERN extern
#   define LIBUPSYSTEMEXCEPTAPI
#endif

#if defined(UP_NO_EXTERN_TEMPLATES) && !defined(LIBUPSYSTEM_NO_EXTERN_TEMPLATES)
#   define LIBUPSYSTEM_NO_EXTERN_TEMPLATES
#endif

#endif
