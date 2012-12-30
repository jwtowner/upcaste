//
//  Upcaste Performance Libraries
//  Copyright (C) 2012 Jesse W. Towner
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

#include <up/cuctype.hpp>
#include <up/cassert.hpp>
#include <up/clocale.hpp>
#include <up/cstdint.hpp>
#include <up/cwctype.hpp>

namespace up { namespace
{
    constexpr size_t locale_name_count = 3;
    char const* const locale_names[locale_name_count] = { "C.UTF-8", "C-UTF-8", "en_US.utf8" };
    
    struct locale_holder
    {
        locale_t locale;

        locale_holder()
        : locale(nullptr) {
            for (size_t i = 0; i < locale_name_count; ++i) {
                locale = newlocale(LC_CTYPE_MASK, locale_names[i], nullptr);
                if (locale) {
                    break;
                }
            }

            assert(locale);
        }

        ~locale_holder() {
            freelocale(locale);
            locale = nullptr;
        }
    }
    utf8_holder;
}}

namespace up
{
    LIBUPCOREAPI UPPURE
    int isualnum(uint_least32_t uc) noexcept {
        return iswalnum_l((wchar_t)uc, utf8_holder.locale);
    }

    LIBUPCOREAPI UPPURE
    int isualpha(uint_least32_t uc) noexcept {
        return iswalpha_l((wchar_t)uc, utf8_holder.locale);
    }

    LIBUPCOREAPI UPPURE
    int isublank(uint_least32_t uc) noexcept {
        return iswblank_l((wchar_t)uc, utf8_holder.locale);
    }

    LIBUPCOREAPI UPPURE
    int isucntrl(uint_least32_t uc) noexcept {
        return iswcntrl_l((wchar_t)uc, utf8_holder.locale);
    }
    
    LIBUPCOREAPI UPPURE
    int isugraph(uint_least32_t uc) noexcept {
        return iswgraph_l((wchar_t)uc, utf8_holder.locale);
    }
    
    LIBUPCOREAPI UPPURE
    int isuprint(uint_least32_t uc) noexcept {
        return iswprint_l((wchar_t)uc, utf8_holder.locale);
    }
    
    LIBUPCOREAPI UPPURE
    int isupunct(uint_least32_t uc) noexcept {
        return iswpunct_l((wchar_t)uc, utf8_holder.locale);
    }
    
    LIBUPCOREAPI UPPURE
    int isuspace(uint_least32_t uc) noexcept {
        return iswspace_l((wchar_t)uc, utf8_holder.locale);
    }
}
