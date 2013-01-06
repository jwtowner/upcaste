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

//
// Based on FNVA1_Jesteress by Georgi Marinov <sanmayce@sanmayce.com>
// http://www.sanmayce.com/Fastest_Hash/index.html
//

#include <up/hash.hpp>
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    uint32_t hash32_accumulate(uint32_t hashcode, void const* key, size_t n) noexcept {
        assert(key || !n);

        uint32_t const* dword, * dword_end;
        uint32_t hashcode1, hashcode2;
        uintptr_t address;

        hashcode1 = hashcode;
#if SIZE_MAX <= UINT32_MAX
        hashcode2 = n;
#else
        hashcode2 = static_cast<uint32_t>(n & UINT32_MAX) ^ static_cast<uint32_t>((n >> 32) & UINT32_MAX);
#endif

        for (dword = static_cast<uint32_t const*>(key), dword_end = dword + ((n >> 2) & ~3); dword < dword_end; dword += 4) {
            hashcode1 = hash32_accumulate(hashcode1, *dword, *(dword + 1));
            hashcode2 = hash32_accumulate(hashcode2, *(dword + 2), *(dword + 3));
        }

        address = reinterpret_cast<uintptr_t>(dword);
        hashcode = hashcode1 ^ hashcode2;

        if (n & 8) {
            hashcode = hash32_accumulate(hashcode, *reinterpret_cast<uint32_t const*>(address), *reinterpret_cast<uint32_t const*>(address + 4));
            address += 8;
        }

        if (n & 4) {
            hashcode = hash32_accumulate(hashcode, *reinterpret_cast<uint32_t const*>(address));
            address += 4;
        }

        if (n & 2) {
            hashcode = hash32_accumulate(hashcode, *reinterpret_cast<uint16_t const*>(address));
            address += 2;
        }

        if (n & 1) {
            hashcode = hash32_accumulate(hashcode, *reinterpret_cast<uint8_t const*>(address));
        }

        return hashcode;
    }
}
