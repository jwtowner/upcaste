﻿//
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

//
//  Do not edit! This file is automatically generated.
//

#ifndef UP_NUMERIC_GENERIC_INT4_OPERATIONS_INL
#define UP_NUMERIC_GENERIC_INT4_OPERATIONS_INL

namespace up { namespace math
{
    //
    // Constructor Bindings
    //

    namespace detail
    {
        template< >
        struct constructors<int4>
        {
            static UPALWAYSINLINE
            int4 load(int32_t const* p) noexcept {
                int4 result;
                result.x = p[0];
                result.y = p[1];
                result.z = p[2];
                result.w = p[3];
                return result;
            }

            static UPALWAYSINLINE
            int4 load(size_t offset, int32_t const* p) noexcept {
                int32_t const* q = p + (offset * 4);
                int4 result;
                result.x = q[0];
                result.y = q[1];
                result.z = q[2];
                result.w = q[3];
                return result;
            }

            static UPALWAYSINLINE
            int4 loadunaligned(int32_t const* p) noexcept {
                int4 result;
                result.x = p[0];
                result.y = p[1];
                result.z = p[2];
                result.w = p[3];
                return result;
            }

            static UPALWAYSINLINE
            int4 loadunaligned(size_t offset, int32_t const* p) noexcept {
                int32_t const* q = p + (offset * 4);
                int4 result;
                result.x = q[0];
                result.y = q[1];
                result.z = q[2];
                result.w = q[3];
                return result;
            }

            static UPALWAYSINLINE
            int4 loadreverse(int32_t const* p) noexcept {
                int4 result;
                result.x = p[3];
                result.y = p[2];
                result.z = p[1];
                result.w = p[0];
                return result;
            }

            static UPALWAYSINLINE
            int4 loadreverse(size_t offset, int32_t const* p) noexcept {
                int32_t const* q = p + (offset * 4);
                int4 result;
                result.x = q[3];
                result.y = q[2];
                result.z = q[1];
                result.w = q[0];
                return result;
            }

            static UPALWAYSINLINE
            int4 loadscalar(int32_t const* p) noexcept {
                int4 result;
                result.x = *p; result.y = result.z = result.w = 0.0;
                return result;
            }

            static UPALWAYSINLINE
            int4 loadscalar(size_t offset, int32_t const* p) noexcept {
                int4 result;
                result.x = *(p + offset); result.y = result.z = result.w = 0.0;
                return result;
            }

            static UPALWAYSINLINE
            int4 make(int32_t x) noexcept {
                int4 result;
                result.x = x; result.y = result.z = result.w = 0.0;
                return result;
            }

            static UPALWAYSINLINE
            int4 make(int32_t x, int32_t y, int32_t z, int32_t w) noexcept {
                int4 result;
                result.x = x;
                result.y = y;
                result.z = z;
                result.w = w;
                return result;
            }

            static UPALWAYSINLINE
            int4 replicate(int32_t value) noexcept {
                int4 result;
                result.x = result.y = result.z = result.w = value;
                return result;
            }

            static UPALWAYSINLINE
            int4 replicate(int32_t const* p) noexcept {
                int4 result;
                result.x = result.y = result.z = result.w = *p;
                return result;
            }

            static UPALWAYSINLINE
            int4 replicate(size_t offset, int32_t const* p) noexcept {
                int4 result;
                result.x = result.y = result.z = result.w = *(p + offset);
                return result;
            }

            static UPALWAYSINLINE
            int4 zero() noexcept {
                int4 result;
                result.x = result.y = result.z = result.w = 0.0;
                return result;
            }
        };
    }

    //
    // Load / Store Functions
    //

    inline UPALWAYSINLINE
    int4 load(int4 const* p) noexcept {
        return *p;
    }

    inline UPALWAYSINLINE
    int4 load(size_t offset, int4 const* p) noexcept {
        return *(p + offset);
    }

    inline UPALWAYSINLINE
    int4 loadreverse(int4 const* p) noexcept {
        int4 result;
        result.x = p->w;
        result.y = p->z;
        result.z = p->y;
        result.w = p->x;
        return result;
    }

    inline UPALWAYSINLINE
    int4 loadreverse(size_t offset, int4 const* p) noexcept {
        int4 const* q = p + offset;
        int4 result;
        result.x = q->w;
        result.y = q->z;
        result.z = q->y;
        result.w = q->x;
        return result;
    }

    //
    // Accessor Functions
    //

    inline UPALWAYSINLINE
    int32_t get(int4 const& v, size_t n) noexcept {
        return v.s[n];
    }

    inline UPALWAYSINLINE
    int32_t getx(int4 const& v) noexcept {
        return v.x;
    }

    inline UPALWAYSINLINE
    int32_t gety(int4 const& v) noexcept {
        return v.y;
    }

    inline UPALWAYSINLINE
    int32_t getz(int4 const& v) noexcept {
        return v.z;
    }

    inline UPALWAYSINLINE
    int32_t getw(int4 const& v) noexcept {
        return v.w;
    }

    inline UPALWAYSINLINE
    int4 put(int4 const& v, int32_t value, size_t n) noexcept {
        int4 result = v;
        result.s[n] = value;
        return result;
    }

    inline UPALWAYSINLINE
    int4 putx(int4 const& v, int32_t x) noexcept {
        int4 result;
        result.x = x;
        result.y = v.y;
        result.z = v.z;
        result.w = v.w;
        return result;
    }

    inline UPALWAYSINLINE
    int4 puty(int4 const& v, int32_t y) noexcept {
        int4 result;
        result.x = v.x;
        result.y = y;
        result.z = v.z;
        result.w = v.w;
        return result;
    }

    inline UPALWAYSINLINE
    int4 putz(int4 const& v, int32_t z) noexcept {
        int4 result;
        result.x = v.x;
        result.y = v.y;
        result.z = z;
        result.w = v.w;
        return result;
    }

    inline UPALWAYSINLINE
    int4 putw(int4 const& v, int32_t w) noexcept {
        int4 result;
        result.x = v.x;
        result.y = v.y;
        result.z = v.z;
        result.w = w;
        return result;
    }

    inline UPALWAYSINLINE
    int4 splat(int4 const& v, size_t n) noexcept {
        int4 result;

        switch (n) {
        case 0: result.x = result.y = result.z = result.w = v.x; break;
        case 1: result.x = result.y = result.z = result.w = v.y; break;
        case 2: result.x = result.y = result.z = result.w = v.z; break;
        case 3: result.x = result.y = result.z = result.w = v.w; break;
        default: result = uniform<int4>::zero; break;
        }

        return result;
    }

    inline UPALWAYSINLINE
    int4 splatx(int4 const& v) noexcept {
        int4 result;
        result.x = v.x;
        result.y = v.x;
        result.z = v.x;
        result.w = v.x;
        return result;
    }

    inline UPALWAYSINLINE
    int4 splaty(int4 const& v) noexcept {
        int4 result;
        result.x = v.y;
        result.y = v.y;
        result.z = v.y;
        result.w = v.y;
        return result;
    }

    inline UPALWAYSINLINE
    int4 splatz(int4 const& v) noexcept {
        int4 result;
        result.x = v.z;
        result.y = v.z;
        result.z = v.z;
        result.w = v.z;
        return result;
    }

    inline UPALWAYSINLINE
    int4 splatw(int4 const& v) noexcept {
        int4 result;
        result.x = v.w;
        result.y = v.w;
        result.z = v.w;
        result.w = v.w;
        return result;
    }

    //
    // Relational Functions
    //

    inline UPALWAYSINLINE
    bool all(int4 const& v) noexcept {
        return ((v.u[0] & v.u[1] & v.u[2] & v.u[3]) & 0x80000000u) != 0;
    }

    inline UPALWAYSINLINE
    bool all(int4 const& v, int mask) noexcept {
        int result;
        result = static_cast<int>((v.u[0] & 0x80000000u) >> 31);
        result |= static_cast<int>((v.u[1] & 0x80000000u) >> 30);
        result |= static_cast<int>((v.u[2] & 0x80000000u) >> 29);
        result |= static_cast<int>((v.u[3] & 0x80000000u) >> 28);
        return (result & mask) == mask;
    }

    inline UPALWAYSINLINE
    bool any(int4 const& v) noexcept {
        return ((v.u[0] | v.u[1] | v.u[2] | v.u[3]) & 0x80000000u) != 0;
    }

    inline UPALWAYSINLINE
    bool any(int4 const& v, int mask) noexcept {
        int result;
        result = static_cast<int>((v.u[0] & 0x80000000u) >> 31);
        result |= static_cast<int>((v.u[1] & 0x80000000u) >> 30);
        result |= static_cast<int>((v.u[2] & 0x80000000u) >> 29);
        result |= static_cast<int>((v.u[3] & 0x80000000u) >> 28);
        return (result & mask) != 0;
    }
    //
    // Relational Operators
    //
    
    inline UPALWAYSINLINE
    bool operator==(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
    }
    
    inline UPALWAYSINLINE
    bool operator!=(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w);
    }
    
    inline UPALWAYSINLINE
    bool operator>(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w);
    }
    
    inline UPALWAYSINLINE
    bool operator>=(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x >= v2.x) && (v1.y >= v2.y) && (v1.z >= v2.z) && (v1.w >= v2.w);
    }
    
    inline UPALWAYSINLINE
    bool operator<(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w);
    }
    
    inline UPALWAYSINLINE
    bool operator<=(int4 const& v1, int4 const& v2) noexcept {
        return (v1.x <= v2.x) && (v1.y <= v2.y) && (v1.z <= v2.z) && (v1.w <= v2.w);
    }

    //
    // Arithmetic Functions
    //
    
    inline UPALWAYSINLINE
    int4 add(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x + v2.x;
        result.y = v1.y + v2.y;
        result.z = v1.z + v2.z;
        result.w = v1.w + v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE    
    int4 div(int4 const& v, int32_t s) noexcept {    
        int4 result;
        result.x = v.x / s;
        result.y = v.y / s;
        result.z = v.z / s;
        result.w = v.w / s;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 div(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x / v2.x;
        result.y = v1.y / v2.y;
        result.z = v1.z / v2.z;
        result.w = v1.w / v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE    
    int4 mul(int4 const& v, int32_t s) noexcept {    
        int4 result;
        result.x = v.x * s;
        result.y = v.y * s;
        result.z = v.z * s;
        result.w = v.w * s;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 mul(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x * v2.x;
        result.y = v1.y * v2.y;
        result.z = v1.z * v2.z;
        result.w = v1.w * v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 neg(int4 const& v) noexcept {
        int4 result;
        result.x = -v.x;
        result.y = -v.y;
        result.z = -v.z;
        result.w = -v.w;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 sub(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x - v2.x;
        result.y = v1.y - v2.y;
        result.z = v1.z - v2.z;
        result.w = v1.w - v2.w;
        return result;
    }

    //
    // Arithmetic Operators
    //
    
    inline UPALWAYSINLINE
    int4 operator+(int4 const& v) noexcept {
        int4 result;
        result.x = +v.x;
        result.y = +v.y;
        result.z = +v.z;
        result.w = +v.w;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator+(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x + v2.x;
        result.y = v1.y + v2.y;
        result.z = v1.z + v2.z;
        result.w = v1.w + v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator-(int4 const& v) noexcept {
        int4 result;
        result.x = -v.x;
        result.y = -v.y;
        result.z = -v.z;
        result.w = -v.w;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator-(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x - v2.x;
        result.y = v1.y - v2.y;
        result.z = v1.z - v2.z;
        result.w = v1.w - v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE    
    int4 operator*(int4 const& v, int32_t s) noexcept {    
        int4 result;
        result.x = v.x * s;
        result.y = v.y * s;
        result.z = v.z * s;
        result.w = v.w * s;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator*(int32_t s, int4 const& v) noexcept {    
        int4 result;
        result.x = v.x * s;
        result.y = v.y * s;
        result.z = v.z * s;
        result.w = v.w * s;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator*(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x * v2.x;
        result.y = v1.y * v2.y;
        result.z = v1.z * v2.z;
        result.w = v1.w * v2.w;
        return result;
    }
    
    inline UPALWAYSINLINE    
    int4 operator/(int4 const& v, int32_t s) noexcept {    
        int4 result;
        result.x = v.x / s;
        result.y = v.y / s;
        result.z = v.z / s;
        result.w = v.w / s;
        return result;
    }
    
    inline UPALWAYSINLINE
    int4 operator/(int4 const& v1, int4 const& v2) noexcept {
        int4 result;
        result.x = v1.x / v2.x;
        result.y = v1.y / v2.y;
        result.z = v1.z / v2.z;
        result.w = v1.w / v2.w;
        return result;
    }

}}

#endif