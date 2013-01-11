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

#if (UP_ARCHITECTURE != UP_ARCHITECTURE_X86) && (UP_ARCHITECTURE != UP_ARCHITECTURE_X64)
#   error "Target CPU architecture is currently unsupported!"
#endif

#define FE_DIVBYZERO    0x04
#define FE_INEXACT      0x20
#define FE_INVALID      0x01
#define FE_OVERFLOW     0x08
#define FE_UNDERFLOW    0x10
#define FE_DENORMAL     0x02

#define FE_ALL_EXCEPT   (FE_DIVBYZERO | FE_INEXACT | FE_INVALID \
                        | FE_OVERFLOW | FE_UNDERFLOW | FE_DENORMAL)

#define FE_DOWNWARD     0x0400
#define FE_TONEAREST    0x0000
#define FE_TOWARDZERO   0x0C00
#define FE_UPWARD       0x0800

#define FE_DFL_ENV      ((::up::fenv_t const*)-1)
#define FE_NOMASK_ENV   ((::up::fenv_t const*)-2)

namespace up
{
    struct fenv_t
    {
        unsigned short control_word;
        unsigned short unused0;
        unsigned short status_word;
        unsigned short unused1;
        unsigned short tag_word;
        unsigned short unused2;  
        unsigned int ip_offset;
        unsigned short ip_selector;  
        unsigned short opcode;
        unsigned int data_offset;
        unsigned short data_selector;  
        unsigned short unused3;
        unsigned int sse_mxscr;
    };

    typedef unsigned short fexcept_t;

    extern "C" LIBUPCOREAPI
    int UPFASTCALL feclearexcept(int excepts);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fegetexceptflag(fexcept_t* flagp, int excepts);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL feraiseexcept(int excepts);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fesetexceptflag(fexcept_t const* flagp, int excepts);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fetestexcept(int excepts);

    extern "C" LIBUPCOREAPI
    int UPFASTCALL feenableexcept(int excepts);

    extern "C" LIBUPCOREAPI
    int UPFASTCALL fedisableexcept(int excepts);

    extern "C" LIBUPCOREAPI
    int UPFASTCALL fegetexcept();
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fegetround();
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fesetround(int round);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fegetenv(fenv_t* envp);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL feholdexcept(fenv_t* envp);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL fesetenv(fenv_t const* envp);
    
    extern "C" LIBUPCOREAPI
    int UPFASTCALL feupdateenv(fenv_t const* envp);
}

#endif
