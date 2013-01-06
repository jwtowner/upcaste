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

#ifndef UP_DETAIL_CUCHAR_WCHAR_UTF8_INL
#define UP_DETAIL_CUCHAR_WCHAR_UTF8_INL

namespace up
{
    inline UPALWAYSINLINE
    wchar_t const* wcsschk(wchar_t const* wcs) noexcept {
        return reinterpret_cast<wchar_t const*>(u8schk(reinterpret_cast<char const*>(wcs)));
    }
    
    inline UPALWAYSINLINE
    wchar_t const* wcsnchk(wchar_t const* wcs, size_t n) noexcept {
        return reinterpret_cast<wchar_t const*>(u8snchk(reinterpret_cast<char const*>(wcs), n));
    }
    
    inline UPALWAYSINLINE
    int wclen(wchar_t const* wcs) noexcept {
        return u8len(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    int wcnlen(wchar_t const* wcs, size_t n) noexcept {
        return u8nlen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    int wctou32(char32_t* UPRESTRICT u32, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u8tou32(u32, reinterpret_cast<char const*>(wcs), n);
    }

    inline UPALWAYSINLINE
    int u32towc(wchar_t* wcs, char32_t u32) noexcept {
        return u32tou8(reinterpret_cast<char*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int u32rtowc(wchar_t* UPRESTRICT wcs, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou8(reinterpret_cast<char*>(wcs), u32);
    }

    inline UPALWAYSINLINE
    size_t wcstou8slen(wchar_t const* wcs) noexcept {
        return u8slen(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcstou16slen(wchar_t const* wcs) noexcept {
        return u8stou16slen(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32slen(wchar_t const* wcs) noexcept {
        return u8stou32slen(reinterpret_cast<char const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou8slen(wchar_t const* wcs, size_t n) noexcept {
        return u8snlen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou16slen(wchar_t const* wcs, size_t n) noexcept {
        return u8sntou16slen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou32slen(wchar_t const* wcs, size_t n) noexcept {
        return u8sntou32slen(reinterpret_cast<char const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou8s(char* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        size_t r = u8snlen(u8sncpy(dst, reinterpret_cast<char const*>(*src), n), n);
        if (n > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }
    
    inline UPALWAYSINLINE
    size_t wcstou16s(char16_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        return u8stou16s(dst, reinterpret_cast<char const**>(src), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32s(char32_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        return u8stou32s(dst, reinterpret_cast<char const**>(src), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou8s(char* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        size_t m = n < nwc ? n : nwc;
        size_t r = u8snlen(u8sncpy(dst, reinterpret_cast<char const*>(*src), m), m);
        if (m > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou16s(char16_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        return u8sntou16s(dst, reinterpret_cast<char const**>(src), nwc, n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou32s(char32_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        return u8sntou32s(dst, reinterpret_cast<char const**>(src), nwc, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcslen(char const* s) noexcept {
        return u8slen(s);
    }
    
    inline UPALWAYSINLINE
    size_t u8sntowcslen(char const* s, size_t n) noexcept {
        return u8snlen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcs(wchar_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t n) noexcept {
        size_t r = u8snlen(u8sncpy(reinterpret_cast<char*>(dst), *src, n), n);
        if (n > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }

    inline UPALWAYSINLINE
    size_t u8sntowcs(wchar_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t nu8, size_t n) noexcept {
        size_t m = n < nu8 ? n : nu8;
        size_t r = u8snlen(u8sncpy(reinterpret_cast<char*>(dst), *src, m), m);
        if (m > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }

    inline UPALWAYSINLINE
    size_t u16stowcslen(char16_t const* s) noexcept {
        return u16stou8slen(s);
    }
    
    inline UPALWAYSINLINE
    size_t u16sntowcslen(char16_t const* s, size_t n) noexcept {
        return u16sntou8slen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcs(wchar_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t n) noexcept {
        return u16stou8s(reinterpret_cast<char*>(dst), src, n);
    }

    inline UPALWAYSINLINE
    size_t u16sntowcs(wchar_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t nu16, size_t n) noexcept {
        return u16sntou8s(reinterpret_cast<char*>(dst), src, nu16, n);
    }

    inline UPALWAYSINLINE
    size_t u32stowcslen(char32_t const* s) noexcept {
        return u32stou8slen(s);
    }

    inline UPALWAYSINLINE
    size_t u32sntowcslen(char32_t const* s, size_t n) noexcept {
        return u32sntou8slen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u32stowcs(wchar_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t n) noexcept {
        return u32stou8s(reinterpret_cast<char*>(dst), src, n);
    }

    inline UPALWAYSINLINE
    size_t u32sntowcs(wchar_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t nu32, size_t n) noexcept {
        return u32sntou8s(reinterpret_cast<char*>(dst), src, nu32, n);
    }
}

#endif
