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

#ifndef UP_HASH_HPP
#define UP_HASH_HPP

#include <up/cstddef.hpp>
#include <up/cstdint.hpp>

namespace up
{
    constexpr uint32_t hashprime32 = 709607u;
    constexpr uint64_t hashprime64 = 11400714819323198393ull;
    constexpr uint32_t hashseed32 = 0x811C9DC5u;
    constexpr uint64_t hashseed64 = 0xCBF29CE484222325ull;

    extern LIBUPCOREAPI UPNONNULLALL
    uint32_t hash32_accumulate(uint32_t hashcode, void const* key, size_t n) noexcept;

    extern LIBUPCOREAPI UPNONNULLALL
    uint64_t hash64_accumulate(uint64_t hashcode, void const* key, size_t n) noexcept;

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, uint32_t key1, uint32_t key2) noexcept {
        return (hashcode ^ (((key1 << 5) | (key1 >> 27)) ^ key2)) * hashprime32;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, uint64_t key1, uint64_t key2) noexcept {
        return hash32_accumulate(
            hash32_accumulate(hashcode, static_cast<uint32_t>(key1 & UINT32_MAX), static_cast<uint32_t>((key1 >> 32) & UINT32_MAX)),
            static_cast<uint32_t>(key2 & UINT32_MAX), static_cast<uint32_t>((key2 >> 32) & UINT32_MAX)
        );
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, unsigned char key) noexcept {
        return (hashcode ^ static_cast<uint32_t>(key)) * hashprime32;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, unsigned short key) noexcept {
        return (hashcode ^ static_cast<uint32_t>(key)) * hashprime32;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, unsigned int key) noexcept {
        return (hashcode ^ static_cast<uint32_t>(key)) * hashprime32;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, unsigned long key) noexcept {
#if (ULONG_MAX <= UINT32_MAX) && !defined(UP_LONG_PTR_64)
        return (hashcode ^ static_cast<uint32_t>(key)) * hashprime32;
#else
        return hash32_accumulate(hashcode, static_cast<uint32_t>(key & UINT32_MAX), static_cast<uint32_t>((key >> 32) & UINT32_MAX));
#endif
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_accumulate(uint32_t hashcode, unsigned long long key) noexcept {
        return hash32_accumulate(hashcode, static_cast<uint32_t>(key & UINT32_MAX), static_cast<uint32_t>((key >> 32) & UINT32_MAX));
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, uint32_t key1, uint32_t key2) noexcept {
        return (hashcode ^ (((key1 << 5) | (key1 >> 27)) ^ key2)) * hashprime64;
    }

    inline UPHIDDENINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, uint64_t key1, uint64_t key2) noexcept {
        return (hashcode ^ (((key1 << 5) | (key1 >> 59)) ^ key2)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, unsigned char key) noexcept {
        return (hashcode ^ static_cast<uint64_t>(key)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, unsigned short key) noexcept {
        return (hashcode ^ static_cast<uint64_t>(key)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, unsigned int key) noexcept {
        return (hashcode ^ static_cast<uint64_t>(key)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, unsigned long key) noexcept {
        return (hashcode ^ static_cast<uint64_t>(key)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_accumulate(uint64_t hashcode, unsigned long long key) noexcept {
        return (hashcode ^ static_cast<uint64_t>(key)) * hashprime64;
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint32_t hash32_finalize(uint32_t hashcode) noexcept {
        return hashcode ^ (hashcode >> 16);
    }

    inline UPALWAYSINLINE UPCONSTEXPR
    uint64_t hash64_finalize(uint64_t hashcode) noexcept {
        return hashcode ^ (hashcode >> 32);
    }

#if (SIZE_MAX <= UINT_LEAST32_MAX) && !defined(UP_LONG_PTR_64)
    constexpr size_t hashprime = hashprime32;
    constexpr size_t hashseed = hashseed32;
    inline UPALWAYSINLINE UPNONNULLALL size_t hash_accumulate(size_t hashcode, void const* key, size_t n) noexcept { return hash32_accumulate(hashcode, key, n); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, uint32_t key1, uint32_t key2) noexcept { return hash32_accumulate(hashcode, key1, key2); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, uint64_t key1, uint64_t key2) noexcept { return hash32_accumulate(hashcode, key1, key2); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned char key) noexcept { return hash32_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned short key) noexcept { return hash32_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned int key) noexcept { return hash32_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned long key) noexcept { return hash32_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned long long key) noexcept { return hash32_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_finalize(size_t hashcode) noexcept { return hash32_finalize(hashcode); }
#else
    constexpr size_t hashprime = hashprime64;
    constexpr size_t hashseed = hashseed64;
    inline UPALWAYSINLINE UPNONNULLALL size_t hash_accumulate(size_t hashcode, void const* key, size_t n) noexcept { return hash64_accumulate(hashcode, key, n); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, uint32_t key1, uint32_t key2) noexcept { return hash64_accumulate(hashcode, key1, key2); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, uint64_t key1, uint64_t key2) noexcept { return hash64_accumulate(hashcode, key1, key2); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned char key) noexcept { return hash64_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned short key) noexcept { return hash64_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned int key) noexcept { return hash64_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned long key) noexcept { return hash64_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_accumulate(size_t hashcode, unsigned long long key) noexcept { return hash64_accumulate(hashcode, key); }
    inline UPALWAYSINLINE UPCONSTEXPR size_t hash_finalize(size_t hashcode) noexcept { return hash64_finalize(hashcode); }
#endif
}

#endif
