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

#ifndef UP_MATH_MATRICES_HPP
#define UP_MATH_MATRICES_HPP

#include <up/math/vectors.hpp>

namespace up { namespace math
{
    union LIBUPCOREAPI int4x2
    {
        int4 v[2];
        int32_t m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        int4x2() = default;
        int4x2(int4x2 const&) = default;
        int4x2& operator=(int4x2 const&) = default;
#else
        UPALWAYSINLINE int4x2() noexcept { }
#endif

        UPALWAYSINLINE
        int4x2(int4_cval0_t v0, int4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        int4x2(
            int32_t m00, int32_t m01, int32_t m02, int32_t m03, 
            int32_t m10, int32_t m11, int32_t m12, int32_t m13
        )
        noexcept {
            v[0] = set<int4>(x00, x01, x02, x03);
            v[1] = set<int4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE int4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE int4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI int4x3
    {
        int4 v[3];
        int32_t m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        int4x3() = default;
        int4x3(int4x3 const&) = default;
        int4x3& operator=(int4x3 const&) = default;
#else
        UPALWAYSINLINE int4x3() noexcept { }
#endif

        UPALWAYSINLINE
        int4x3(int4_cval0_t v0, int4_cval1_t v1, int4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        int4x3(
            int32_t m00, int32_t m01, int32_t m02, int32_t m03, 
            int32_t m10, int32_t m11, int32_t m12, int32_t m13, 
            int32_t m20, int32_t m21, int32_t m22, int32_t m23
        )
        noexcept {
            v[0] = set<int4>(x00, x01, x02, x03);
            v[1] = set<int4>(x10, x11, x12, x13);
            v[2] = set<int4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE int4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE int4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI int4x4
    {
        int4 v[4];
        int32_t m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        int4x4() = default;
        int4x4(int4x4 const&) = default;
        int4x4& operator=(int4x4 const&) = default;
#else
        UPALWAYSINLINE int4x4() noexcept { }
#endif

        UPALWAYSINLINE
        int4x4(int4_cval0_t v0, int4_cval1_t v1, int4_cval2_t v2, int4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        int4x4(
            int32_t m00, int32_t m01, int32_t m02, int32_t m03, 
            int32_t m10, int32_t m11, int32_t m12, int32_t m13, 
            int32_t m20, int32_t m21, int32_t m22, int32_t m23, 
            int32_t m30, int32_t m31, int32_t m32, int32_t m33
        )
        noexcept {
            v[0] = set<int4>(x00, x01, x02, x03);
            v[1] = set<int4>(x10, x11, x12, x13);
            v[2] = set<int4>(x20, x21, x22, x23);
            v[3] = set<int4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE int4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE int4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI uint4x2
    {
        uint4 v[2];
        uint32_t m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        uint4x2() = default;
        uint4x2(uint4x2 const&) = default;
        uint4x2& operator=(uint4x2 const&) = default;
#else
        UPALWAYSINLINE uint4x2() noexcept { }
#endif

        UPALWAYSINLINE
        uint4x2(uint4_cval0_t v0, uint4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        uint4x2(
            uint32_t m00, uint32_t m01, uint32_t m02, uint32_t m03, 
            uint32_t m10, uint32_t m11, uint32_t m12, uint32_t m13
        )
        noexcept {
            v[0] = set<uint4>(x00, x01, x02, x03);
            v[1] = set<uint4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE uint4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE uint4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI uint4x3
    {
        uint4 v[3];
        uint32_t m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        uint4x3() = default;
        uint4x3(uint4x3 const&) = default;
        uint4x3& operator=(uint4x3 const&) = default;
#else
        UPALWAYSINLINE uint4x3() noexcept { }
#endif

        UPALWAYSINLINE
        uint4x3(uint4_cval0_t v0, uint4_cval1_t v1, uint4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        uint4x3(
            uint32_t m00, uint32_t m01, uint32_t m02, uint32_t m03, 
            uint32_t m10, uint32_t m11, uint32_t m12, uint32_t m13, 
            uint32_t m20, uint32_t m21, uint32_t m22, uint32_t m23
        )
        noexcept {
            v[0] = set<uint4>(x00, x01, x02, x03);
            v[1] = set<uint4>(x10, x11, x12, x13);
            v[2] = set<uint4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE uint4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE uint4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI uint4x4
    {
        uint4 v[4];
        uint32_t m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        uint4x4() = default;
        uint4x4(uint4x4 const&) = default;
        uint4x4& operator=(uint4x4 const&) = default;
#else
        UPALWAYSINLINE uint4x4() noexcept { }
#endif

        UPALWAYSINLINE
        uint4x4(uint4_cval0_t v0, uint4_cval1_t v1, uint4_cval2_t v2, uint4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        uint4x4(
            uint32_t m00, uint32_t m01, uint32_t m02, uint32_t m03, 
            uint32_t m10, uint32_t m11, uint32_t m12, uint32_t m13, 
            uint32_t m20, uint32_t m21, uint32_t m22, uint32_t m23, 
            uint32_t m30, uint32_t m31, uint32_t m32, uint32_t m33
        )
        noexcept {
            v[0] = set<uint4>(x00, x01, x02, x03);
            v[1] = set<uint4>(x10, x11, x12, x13);
            v[2] = set<uint4>(x20, x21, x22, x23);
            v[3] = set<uint4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE uint4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE uint4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint32_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint32_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI float4x2
    {
        float4 v[2];
        float m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        float4x2() = default;
        float4x2(float4x2 const&) = default;
        float4x2& operator=(float4x2 const&) = default;
#else
        UPALWAYSINLINE float4x2() noexcept { }
#endif

        UPALWAYSINLINE
        float4x2(float4_cval0_t v0, float4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        float4x2(
            float m00, float m01, float m02, float m03, 
            float m10, float m11, float m12, float m13
        )
        noexcept {
            v[0] = set<float4>(x00, x01, x02, x03);
            v[1] = set<float4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE float4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE float4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE float& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE float operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI float4x3
    {
        float4 v[3];
        float m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        float4x3() = default;
        float4x3(float4x3 const&) = default;
        float4x3& operator=(float4x3 const&) = default;
#else
        UPALWAYSINLINE float4x3() noexcept { }
#endif

        UPALWAYSINLINE
        float4x3(float4_cval0_t v0, float4_cval1_t v1, float4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        float4x3(
            float m00, float m01, float m02, float m03, 
            float m10, float m11, float m12, float m13, 
            float m20, float m21, float m22, float m23
        )
        noexcept {
            v[0] = set<float4>(x00, x01, x02, x03);
            v[1] = set<float4>(x10, x11, x12, x13);
            v[2] = set<float4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE float4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE float4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE float& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE float operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI float4x4
    {
        float4 v[4];
        float m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        float4x4() = default;
        float4x4(float4x4 const&) = default;
        float4x4& operator=(float4x4 const&) = default;
#else
        UPALWAYSINLINE float4x4() noexcept { }
#endif

        UPALWAYSINLINE
        float4x4(float4_cval0_t v0, float4_cval1_t v1, float4_cval2_t v2, float4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        float4x4(
            float m00, float m01, float m02, float m03, 
            float m10, float m11, float m12, float m13, 
            float m20, float m21, float m22, float m23, 
            float m30, float m31, float m32, float m33
        )
        noexcept {
            v[0] = set<float4>(x00, x01, x02, x03);
            v[1] = set<float4>(x10, x11, x12, x13);
            v[2] = set<float4>(x20, x21, x22, x23);
            v[3] = set<float4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE float4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE float4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE float& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE float operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI long4x2
    {
        long4 v[2];
        int64_t m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        long4x2() = default;
        long4x2(long4x2 const&) = default;
        long4x2& operator=(long4x2 const&) = default;
#else
        UPALWAYSINLINE long4x2() noexcept { }
#endif

        UPALWAYSINLINE
        long4x2(long4_cval0_t v0, long4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        long4x2(
            int64_t m00, int64_t m01, int64_t m02, int64_t m03, 
            int64_t m10, int64_t m11, int64_t m12, int64_t m13
        )
        noexcept {
            v[0] = set<long4>(x00, x01, x02, x03);
            v[1] = set<long4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE long4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE long4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI long4x3
    {
        long4 v[3];
        int64_t m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        long4x3() = default;
        long4x3(long4x3 const&) = default;
        long4x3& operator=(long4x3 const&) = default;
#else
        UPALWAYSINLINE long4x3() noexcept { }
#endif

        UPALWAYSINLINE
        long4x3(long4_cval0_t v0, long4_cval1_t v1, long4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        long4x3(
            int64_t m00, int64_t m01, int64_t m02, int64_t m03, 
            int64_t m10, int64_t m11, int64_t m12, int64_t m13, 
            int64_t m20, int64_t m21, int64_t m22, int64_t m23
        )
        noexcept {
            v[0] = set<long4>(x00, x01, x02, x03);
            v[1] = set<long4>(x10, x11, x12, x13);
            v[2] = set<long4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE long4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE long4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI long4x4
    {
        long4 v[4];
        int64_t m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        long4x4() = default;
        long4x4(long4x4 const&) = default;
        long4x4& operator=(long4x4 const&) = default;
#else
        UPALWAYSINLINE long4x4() noexcept { }
#endif

        UPALWAYSINLINE
        long4x4(long4_cval0_t v0, long4_cval1_t v1, long4_cval2_t v2, long4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        long4x4(
            int64_t m00, int64_t m01, int64_t m02, int64_t m03, 
            int64_t m10, int64_t m11, int64_t m12, int64_t m13, 
            int64_t m20, int64_t m21, int64_t m22, int64_t m23, 
            int64_t m30, int64_t m31, int64_t m32, int64_t m33
        )
        noexcept {
            v[0] = set<long4>(x00, x01, x02, x03);
            v[1] = set<long4>(x10, x11, x12, x13);
            v[2] = set<long4>(x20, x21, x22, x23);
            v[3] = set<long4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE long4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE long4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE int64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE int64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI ulong4x2
    {
        ulong4 v[2];
        uint64_t m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        ulong4x2() = default;
        ulong4x2(ulong4x2 const&) = default;
        ulong4x2& operator=(ulong4x2 const&) = default;
#else
        UPALWAYSINLINE ulong4x2() noexcept { }
#endif

        UPALWAYSINLINE
        ulong4x2(ulong4_cval0_t v0, ulong4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        ulong4x2(
            uint64_t m00, uint64_t m01, uint64_t m02, uint64_t m03, 
            uint64_t m10, uint64_t m11, uint64_t m12, uint64_t m13
        )
        noexcept {
            v[0] = set<ulong4>(x00, x01, x02, x03);
            v[1] = set<ulong4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE ulong4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE ulong4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI ulong4x3
    {
        ulong4 v[3];
        uint64_t m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        ulong4x3() = default;
        ulong4x3(ulong4x3 const&) = default;
        ulong4x3& operator=(ulong4x3 const&) = default;
#else
        UPALWAYSINLINE ulong4x3() noexcept { }
#endif

        UPALWAYSINLINE
        ulong4x3(ulong4_cval0_t v0, ulong4_cval1_t v1, ulong4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        ulong4x3(
            uint64_t m00, uint64_t m01, uint64_t m02, uint64_t m03, 
            uint64_t m10, uint64_t m11, uint64_t m12, uint64_t m13, 
            uint64_t m20, uint64_t m21, uint64_t m22, uint64_t m23
        )
        noexcept {
            v[0] = set<ulong4>(x00, x01, x02, x03);
            v[1] = set<ulong4>(x10, x11, x12, x13);
            v[2] = set<ulong4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE ulong4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE ulong4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI ulong4x4
    {
        ulong4 v[4];
        uint64_t m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        ulong4x4() = default;
        ulong4x4(ulong4x4 const&) = default;
        ulong4x4& operator=(ulong4x4 const&) = default;
#else
        UPALWAYSINLINE ulong4x4() noexcept { }
#endif

        UPALWAYSINLINE
        ulong4x4(ulong4_cval0_t v0, ulong4_cval1_t v1, ulong4_cval2_t v2, ulong4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        ulong4x4(
            uint64_t m00, uint64_t m01, uint64_t m02, uint64_t m03, 
            uint64_t m10, uint64_t m11, uint64_t m12, uint64_t m13, 
            uint64_t m20, uint64_t m21, uint64_t m22, uint64_t m23, 
            uint64_t m30, uint64_t m31, uint64_t m32, uint64_t m33
        )
        noexcept {
            v[0] = set<ulong4>(x00, x01, x02, x03);
            v[1] = set<ulong4>(x10, x11, x12, x13);
            v[2] = set<ulong4>(x20, x21, x22, x23);
            v[3] = set<ulong4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE ulong4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE ulong4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE uint64_t& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE uint64_t operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI double4x2
    {
        double4 v[2];
        double m[2][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        double4x2() = default;
        double4x2(double4x2 const&) = default;
        double4x2& operator=(double4x2 const&) = default;
#else
        UPALWAYSINLINE double4x2() noexcept { }
#endif

        UPALWAYSINLINE
        double4x2(double4_cval0_t v0, double4_cval1_t v1) noexcept {
            v[0] = v0;
            v[1] = v1;
        }
        
        UPALWAYSINLINE
        double4x2(
            double m00, double m01, double m02, double m03, 
            double m10, double m11, double m12, double m13
        )
        noexcept {
            v[0] = set<double4>(x00, x01, x02, x03);
            v[1] = set<double4>(x10, x11, x12, x13);
        }

        UPALWAYSINLINE double4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE double4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE double& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE double operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI double4x3
    {
        double4 v[3];
        double m[3][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        double4x3() = default;
        double4x3(double4x3 const&) = default;
        double4x3& operator=(double4x3 const&) = default;
#else
        UPALWAYSINLINE double4x3() noexcept { }
#endif

        UPALWAYSINLINE
        double4x3(double4_cval0_t v0, double4_cval1_t v1, double4_cval2_t v2) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        
        UPALWAYSINLINE
        double4x3(
            double m00, double m01, double m02, double m03, 
            double m10, double m11, double m12, double m13, 
            double m20, double m21, double m22, double m23
        )
        noexcept {
            v[0] = set<double4>(x00, x01, x02, x03);
            v[1] = set<double4>(x10, x11, x12, x13);
            v[2] = set<double4>(x20, x21, x22, x23);
        }

        UPALWAYSINLINE double4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE double4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE double& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE double operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    union LIBUPCOREAPI double4x4
    {
        double4 v[4];
        double m[4][4];

#ifndef UP_NO_DEFAULTED_FUNCTIONS
        double4x4() = default;
        double4x4(double4x4 const&) = default;
        double4x4& operator=(double4x4 const&) = default;
#else
        UPALWAYSINLINE double4x4() noexcept { }
#endif

        UPALWAYSINLINE
        double4x4(double4_cval0_t v0, double4_cval1_t v1, double4_cval2_t v2, double4_cval3_t v3) noexcept {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = v3;
        }
        
        UPALWAYSINLINE
        double4x4(
            double m00, double m01, double m02, double m03, 
            double m10, double m11, double m12, double m13, 
            double m20, double m21, double m22, double m23, 
            double m30, double m31, double m32, double m33
        )
        noexcept {
            v[0] = set<double4>(x00, x01, x02, x03);
            v[1] = set<double4>(x10, x11, x12, x13);
            v[2] = set<double4>(x20, x21, x22, x23);
            v[3] = set<double4>(x30, x31, x32, x33);
        }

        UPALWAYSINLINE double4_ref_t operator[](size_t column) noexcept { return v[column]; }
        UPALWAYSINLINE double4_cref_t operator[](size_t column) const noexcept { return v[column]; }
        UPALWAYSINLINE double& operator()(size_t column, size_t row) noexcept { return m[column][row]; }
        UPALWAYSINLINE double operator()(size_t column, size_t row) const noexcept { return m[column][row]; }
    };

    typedef int4x2& int4x2_ref_t;
    typedef int4x2 const& int4x2_cref_t;
    typedef int4x2 const& int4x2_cval_t;
    typedef int4x3& int4x3_ref_t;
    typedef int4x3 const& int4x3_cref_t;
    typedef int4x3 const& int4x3_cval_t;
    typedef int4x4& int4x4_ref_t;
    typedef int4x4 const& int4x4_cref_t;
    typedef int4x4 const& int4x4_cval_t;
    typedef uint4x2& uint4x2_ref_t;
    typedef uint4x2 const& uint4x2_cref_t;
    typedef uint4x2 const& uint4x2_cval_t;
    typedef uint4x3& uint4x3_ref_t;
    typedef uint4x3 const& uint4x3_cref_t;
    typedef uint4x3 const& uint4x3_cval_t;
    typedef uint4x4& uint4x4_ref_t;
    typedef uint4x4 const& uint4x4_cref_t;
    typedef uint4x4 const& uint4x4_cval_t;
    typedef float4x2& float4x2_ref_t;
    typedef float4x2 const& float4x2_cref_t;
    typedef float4x2 const& float4x2_cval_t;
    typedef float4x3& float4x3_ref_t;
    typedef float4x3 const& float4x3_cref_t;
    typedef float4x3 const& float4x3_cval_t;
    typedef float4x4& float4x4_ref_t;
    typedef float4x4 const& float4x4_cref_t;
    typedef float4x4 const& float4x4_cval_t;
    typedef long4x2& long4x2_ref_t;
    typedef long4x2 const& long4x2_cref_t;
    typedef long4x2 const& long4x2_cval_t;
    typedef long4x3& long4x3_ref_t;
    typedef long4x3 const& long4x3_cref_t;
    typedef long4x3 const& long4x3_cval_t;
    typedef long4x4& long4x4_ref_t;
    typedef long4x4 const& long4x4_cref_t;
    typedef long4x4 const& long4x4_cval_t;
    typedef ulong4x2& ulong4x2_ref_t;
    typedef ulong4x2 const& ulong4x2_cref_t;
    typedef ulong4x2 const& ulong4x2_cval_t;
    typedef ulong4x3& ulong4x3_ref_t;
    typedef ulong4x3 const& ulong4x3_cref_t;
    typedef ulong4x3 const& ulong4x3_cval_t;
    typedef ulong4x4& ulong4x4_ref_t;
    typedef ulong4x4 const& ulong4x4_cref_t;
    typedef ulong4x4 const& ulong4x4_cval_t;
    typedef double4x2& double4x2_ref_t;
    typedef double4x2 const& double4x2_cref_t;
    typedef double4x2 const& double4x2_cval_t;
    typedef double4x3& double4x3_ref_t;
    typedef double4x3 const& double4x3_cref_t;
    typedef double4x3 const& double4x3_cval_t;
    typedef double4x4& double4x4_ref_t;
    typedef double4x4 const& double4x4_cref_t;
    typedef double4x4 const& double4x4_cval_t;
}}

#endif