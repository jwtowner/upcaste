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

#ifndef UP_DETAIL_CUCHAR_WCHAR_UTF16_INL
#define UP_DETAIL_CUCHAR_WCHAR_UTF16_INL

namespace up
{
    inline UPALWAYSINLINE
    wchar_t const* wcsschk(wchar_t const* wcs) noexcept {
        return reinterpret_cast<wchar_t const*>(u16schk(reinterpret_cast<char16_t const*>(wcs)));
    }
    
    inline UPALWAYSINLINE
    wchar_t const* wcsnchk(wchar_t const* wcs, size_t n) noexcept {
        return reinterpret_cast<wchar_t const*>(u16snchk(reinterpret_cast<char16_t const*>(wcs), n));
    }
    
    inline UPALWAYSINLINE
    int wclen(wchar_t const* wcs) noexcept {
        return u16len(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    int wcnlen(wchar_t const* wcs, size_t n) noexcept {
        return u16nlen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    int u32towc(wchar_t* wcs, char32_t u32) noexcept {
        return u32tou16(reinterpret_cast<char16_t*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int u32rtowc(wchar_t* UPRESTRICT wcs, char32_t u32, mbstate_t* UPRESTRICT) noexcept {
        return u32tou16(reinterpret_cast<char16_t*>(wcs), u32);
    }
    
    inline UPALWAYSINLINE
    int wctou32(char32_t* UPRESTRICT u32, wchar_t const* UPRESTRICT wcs, size_t n) noexcept {
        return u16tou32(u32, reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou8slen(wchar_t const* wcs) noexcept {
        return u16stou8slen(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcstou16slen(wchar_t const* wcs) noexcept {
        return u16slen(reinterpret_cast<char16_t const*>(wcs));
    }

    inline UPALWAYSINLINE
    size_t wcstou32slen(wchar_t const* wcs) noexcept {
        return u16stou32slen(reinterpret_cast<char16_t const*>(wcs));
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou8slen(wchar_t const* wcs, size_t n) noexcept {
        return u16sntou8slen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou16slen(wchar_t const* wcs, size_t n) noexcept {
        return u16snlen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcsntou32slen(wchar_t const* wcs, size_t n) noexcept {
        return u16sntou32slen(reinterpret_cast<char16_t const*>(wcs), n);
    }
    
    inline UPALWAYSINLINE
    size_t wcstou8s(char* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        return u16stou8s(dst, reinterpret_cast<char16_t const**>(src), n);
    }
    
    inline UPALWAYSINLINE 
    size_t wcstou16s(char16_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        size_t r = u16snlen(u16sncpy(dst, reinterpret_cast<char16_t const*>(*src), n), n);
        if (n > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }
    
    inline UPALWAYSINLINE
    size_t wcstou32s(char32_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t n) noexcept {
        return u16stou32s(dst, reinterpret_cast<char16_t const**>(src), n);
    }

    inline UPALWAYSINLINE
    size_t wcsntou8s(char* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        return u16sntou8s(dst, reinterpret_cast<char16_t const**>(src), nwc, n);
    }
    
    inline UPALWAYSINLINE 
    size_t wcsntou16s(char16_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        size_t m = n < nwc ? n : nwc;
        size_t r = u16snlen(u16sncpy(dst, reinterpret_cast<char16_t const*>(*src), m), m);
        if (m > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }

    inline UPALWAYSINLINE
    size_t wcsntou32s(char32_t* UPRESTRICT dst, wchar_t const** UPRESTRICT src, size_t nwc, size_t n) noexcept {
        return u16sntou32s(dst, reinterpret_cast<char16_t const**>(src), nwc, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcslen(char const* s) noexcept {
        return u8stou16slen(s);
    }

    inline UPALWAYSINLINE
    size_t u8sntowcslen(char const* s, size_t n) noexcept {
        return u8sntou16slen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u8stowcs(wchar_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t n) noexcept {
        return u8stou16s(reinterpret_cast<char16_t*>(dst), src, n);
    }

    inline UPALWAYSINLINE
    size_t u8sntowcs(wchar_t* UPRESTRICT dst, char const** UPRESTRICT src, size_t nu8, size_t n) noexcept {
        return u8sntou16s(reinterpret_cast<char16_t*>(dst), src, nu8, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcslen(char16_t const* s) noexcept {
        return u16slen(s);
    }

    inline UPALWAYSINLINE
    size_t u16sntowcslen(char16_t const* s, size_t n) noexcept {
        return u16snlen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u16stowcs(wchar_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t n) noexcept {
        size_t r = u16snlen(u16sncpy(reinterpret_cast<char16_t*>(dst), *src, n), n);
        if (n > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }

    inline UPALWAYSINLINE
    size_t u16sntowcs(wchar_t* UPRESTRICT dst, char16_t const** UPRESTRICT src, size_t nu16, size_t n) noexcept {
        size_t m = n < nu16 ? n : nu16;
        size_t r = u16snlen(u16sncpy(reinterpret_cast<char16_t*>(dst), *src, m), m);
        if (m > 0) {
            *src = dst[r] ? (*src + r) : nullptr;
        }
        return r;
    }

    inline UPALWAYSINLINE
    size_t u32stowcslen(char32_t const* s) noexcept {
        return u32stou16slen(s);
    }
    
    inline UPALWAYSINLINE
    size_t u32sntowcslen(char32_t const* s, size_t n) noexcept {
        return u32sntou16slen(s, n);
    }

    inline UPALWAYSINLINE
    size_t u32stowcs(wchar_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t n) noexcept {
        return u32stou16s(reinterpret_cast<char16_t*>(dst), src, n);
    }

    inline UPALWAYSINLINE
    size_t u32sntowcs(wchar_t* UPRESTRICT dst, char32_t const** UPRESTRICT src, size_t nu32, size_t n) noexcept {
        return u32sntou16s(reinterpret_cast<char16_t*>(dst), src, nu32, n);
    }
}

#endif
