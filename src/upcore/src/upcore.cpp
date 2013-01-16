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

#include <up/allocator.hpp>
#include <up/atomic.hpp>
#include <up/bitwise.hpp>
#include <up/byteswap.hpp>
#include <up/cassert.hpp>
#include <up/cctype.hpp>
#include <up/cerrno.hpp>
#include <up/cfenv.hpp>
#include <up/cfloat.hpp>
#include <up/cinttypes.hpp>
#include <up/ciso646.hpp>
#include <up/climits.hpp>
#include <up/clocale.hpp>
#include <up/cmath.hpp>
#include <up/csetjmp.hpp>
#include <up/csignal.hpp>
#include <up/cstdalign.hpp>
#include <up/cstdarg.hpp>
#include <up/cstdbool.hpp>
#include <up/cstddef.hpp>
#include <up/cstdint.hpp>
#include <up/cstdio.hpp>
#include <up/cstdlib.hpp>
#include <up/cstring.hpp>
#include <up/cthreads.hpp>
#include <up/ctime.hpp>
#include <up/cuchar.hpp>
#include <up/cuctype.hpp>
#include <up/cwchar.hpp>
#include <up/cwctype.hpp>
#include <up/densemap.hpp>
#include <up/denseset.hpp>
#include <up/filesystem.hpp>
#include <up/hash.hpp>
#include <up/hashmap.hpp>
#include <up/hashset.hpp>
#include <up/limits.hpp>
#include <up/list.hpp>
#include <up/log.hpp>
#include <up/math.hpp>
#include <up/memory.hpp>
#include <up/page.hpp>
#include <up/prolog.hpp>
#include <up/random.hpp>
#include <up/region.hpp>
#include <up/slist.hpp>
#include <up/sparsemap.hpp>
#include <up/sparseset.hpp>
#include <up/tchar.hpp>
#include <up/type_traits.hpp>
#include <up/utility.hpp>

extern "C"
{
    LIBUPCOREAPI UPUSED
    char const* const _Upcaste_Upcore_Copyright_ = "Upcaste Performance Libraries. Copyright (C) 2012-2013 Jesse W. Towner";
#if UP_COMPILER == UP_COMPILER_MSVC
#   ifndef UP_LINKAGE_SHARED
#       pragma comment(linker, "/include:__Upcaste_Upcore_Copyright_")
#   endif
#endif
}
