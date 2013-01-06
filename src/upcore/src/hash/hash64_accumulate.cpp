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
// Based on FNVA1_Tesla by Georgi Marinov <sanmayce@sanmayce.com>
// http://www.sanmayce.com/Fastest_Hash/index.html
//

#include <up/hash.hpp>
#include <up/cassert.hpp>

namespace up
{
    LIBUPCOREAPI UPNONNULLALL
    uint64_t hash64_accumulate(uint64_t hashcode, void const* key, size_t n) noexcept {
        assert(key || !n);

        uint64_t const* qword, * qword_end;
        uint64_t hashcode1, hashcode2;
        uintptr_t address;

        hashcode1 = hashcode;
        hashcode2 = n;

        for (qword = static_cast<uint64_t const*>(key), qword_end = qword + ((n >> 3) & ~3); qword < qword_end; qword += 4) {
            hashcode1 = hash64_accumulate(hashcode1, *qword, *(qword + 1));
            hashcode2 = hash64_accumulate(hashcode2, *(qword + 2), *(qword + 3));
        }

        address = reinterpret_cast<uintptr_t>(qword);
        hashcode = hashcode1 ^ hashcode2;
            
        if (n & 16) {
            hashcode = hash64_accumulate(hashcode, *reinterpret_cast<uint32_t const*>(address), *reinterpret_cast<uint32_t const*>(address + 8));
            address += 16;
        }

        if (n & 8) {
            hashcode = hash64_accumulate(hashcode, *reinterpret_cast<uint32_t const*>(address), *reinterpret_cast<uint32_t const*>(address + 4));
            address += 8;
        }

        if (n & 4) {
            hashcode = hash64_accumulate(hashcode, *reinterpret_cast<uint32_t const*>(address));
            address += 4;
        }

        if (n & 2) {
            hashcode = hash64_accumulate(hashcode, *reinterpret_cast<uint16_t const*>(address));
            address += 2;
        }
            
        if (n & 1) {
            hashcode = hash64_accumulate(hashcode, *reinterpret_cast<uint8_t const*>(address));
        }

        return hashcode;
    }
}
