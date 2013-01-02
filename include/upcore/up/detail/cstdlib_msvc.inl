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

#ifndef UP_DETAIL_CSTDLIB_MSVC_INL
#define UP_DETAIL_CSTDLIB_MSVC_INL

#ifndef UP_CSTDLIB_HPP
#   error "Do not include this header directly. Instead include <up/cstdlib.hpp>"
#endif

#define malloca(n) _malloca(n)
#define freea(p) _freea(p)

namespace up
{
    inline UPALWAYSINLINE UPALLOC UPWARNRESULT
    void* aligned_alloc(size_t alignment, size_t size) noexcept {
        return ::_aligned_malloc(size, alignment);
    }
    
    inline UPALWAYSINLINE
    void aligned_free(void* ptr) noexcept {
        ::_aligned_free(ptr);
    }
}

#endif
