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

#ifndef UP_CSTDATOMIC_STORAGE_MSVC_X86_64_INL
#define UP_CSTDATOMIC_STORAGE_MSVC_X86_64_INL

#ifndef UP_CSTDATOMIC_HPP
#   error "Do not include this file directly, instead include <up/cstdatomic.hpp>"
#endif

#define ATOMIC_BOOL_LOCK_FREE 2
#define ATOMIC_ADDRESS_LOCK_FREE 2
#define ATOMIC_CHAR_LOCK_FREE 2
#define ATOMIC_CHAR16_T_LOCK_FREE 2
#define ATOMIC_CHAR32_T_LOCK_FREE 2
#define ATOMIC_WCHAR_T_LOCK_FREE 2
#define ATOMIC_SHORT_LOCK_FREE 2
#define ATOMIC_INT_LOCK_FREE 2
#define ATOMIC_LONG_LOCK_FREE 2
#define ATOMIC_LLONG_LOCK_FREE 2
#define UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_1 _InterlockedCompareExchange8
#define UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_2 _InterlockedCompareExchange16
#define UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_4 _InterlockedCompareExchange
#define UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_8 _InterlockedCompareExchange64
#define UP_DETAIL_ATOMIC_EXCHANGE_1 _InterlockedExchange8
#define UP_DETAIL_ATOMIC_EXCHANGE_2 _InterlockedExchange16
#define UP_DETAIL_ATOMIC_EXCHANGE_4 _InterlockedExchange
#define UP_DETAIL_ATOMIC_EXCHANGE_8 _InterlockedExchange64
#define UP_DETAIL_ATOMIC_FETCH_ADD_1 _InterlockedExchangeAdd8
#define UP_DETAIL_ATOMIC_FETCH_ADD_2 _InterlockedExchangeAdd16
#define UP_DETAIL_ATOMIC_FETCH_ADD_4 _InterlockedExchangeAdd
#define UP_DETAIL_ATOMIC_FETCH_ADD_8 _InterlockedExchangeAdd64
#define UP_DETAIL_ATOMIC_FETCH_SUB_1(operand, value) _InterlockedExchangeAdd8(operand, -value)
#define UP_DETAIL_ATOMIC_FETCH_SUB_2(operand, value) _InterlockedExchangeAdd16(operand, -value)
#define UP_DETAIL_ATOMIC_FETCH_SUB_4(operand, value) _InterlockedExchangeAdd(operand, -value)
#define UP_DETAIL_ATOMIC_FETCH_SUB_8(operand, value) _InterlockedExchangeAdd64(operand, -value)
#define UP_DETAIL_ATOMIC_FETCH_AND_1 _InterlockedAnd8
#define UP_DETAIL_ATOMIC_FETCH_AND_2 _InterlockedAnd16
#define UP_DETAIL_ATOMIC_FETCH_AND_4 _InterlockedAnd
#define UP_DETAIL_ATOMIC_FETCH_AND_8 _InterlockedAnd64
#define UP_DETAIL_ATOMIC_FETCH_OR_1 _InterlockedOr8
#define UP_DETAIL_ATOMIC_FETCH_OR_2 _InterlockedOr16
#define UP_DETAIL_ATOMIC_FETCH_OR_4 _InterlockedOr
#define UP_DETAIL_ATOMIC_FETCH_OR_8 _InterlockedOr64
#define UP_DETAIL_ATOMIC_FETCH_XOR_1 _InterlockedXor8
#define UP_DETAIL_ATOMIC_FETCH_XOR_2 _InterlockedXor16
#define UP_DETAIL_ATOMIC_FETCH_XOR_4 _InterlockedXor
#define UP_DETAIL_ATOMIC_FETCH_XOR_8 _InterlockedXor64

#define UP_DETAIL_DECLARE_ATOMIC_STORAGE_OPERATIONS(Raw, Size, Volatile) \
    UPALWAYSINLINE \
    bool compare_exchange_strong(Value& expected, Value desired, memory_order, memory_order) Volatile noexcept { \
        union { Value exp; Raw exp_raw; }; union { Value des; Raw des_raw; }; \
        union { Value act; Raw act_raw; }; exp = expected; des = desired; \
        act_raw = UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_##Size(&this->raw_, des_raw, exp_raw); \
        if (expected == act) { \
            return true; \
        } \
        expected = act; \
        return false; \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_weak(Value& expected, Value desired, memory_order, memory_order) Volatile noexcept { \
        union { Value exp; Raw exp_raw; }; union { Value des; Raw des_raw; }; \
        union { Value act; Raw act_raw; }; exp = expected; des = desired; \
        act_raw = UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_##Size(&this->raw_, des_raw, exp_raw); \
        if (expected == act) { \
            return true; \
        } \
        expected = act; \
        return false; \
    } \
    UPALWAYSINLINE \
    Value exchange(Value desired, memory_order) Volatile noexcept { \
        union { Value des; Raw des_raw; }; union { Value res; Raw res_raw; }; des = desired; \
        res_raw = UP_DETAIL_ATOMIC_EXCHANGE_##Size(&this->raw_, des_raw); \
        return res; \
    } \
    UPALWAYSINLINE \
    Value fetch_add(operand_type operand, memory_order) Volatile noexcept { \
        union { operand_type op; Raw op_raw; }; union { Value res; Raw res_raw; }; op = operand; \
        res_raw = UP_DETAIL_ATOMIC_FETCH_ADD_##Size(&this->raw_, op_raw); \
        return res; \
    } \
    UPALWAYSINLINE \
    Value fetch_sub(operand_type operand, memory_order) Volatile noexcept { \
        union { operand_type op; Raw op_raw; }; union { Value res; Raw res_raw; }; op = operand; \
        res_raw = UP_DETAIL_ATOMIC_FETCH_SUB_##Size(&this->raw_, op_raw); \
        return reinterpret_cast<Value&>(res); \
    } \
    UPALWAYSINLINE \
    Value fetch_and(operand_type operand, memory_order) Volatile noexcept { \
        union { operand_type op; Raw op_raw; }; union { Value res; Raw res_raw; }; op = operand; \
        res_raw = UP_DETAIL_ATOMIC_FETCH_AND_##Size(&this->raw_, op_raw); \
        return res; \
    } \
    UPALWAYSINLINE \
    Value fetch_or(operand_type operand, memory_order) Volatile noexcept { \
        union { operand_type op; Raw op_raw; }; union { Value res; Raw res_raw; }; op = operand; \
        res_raw = UP_DETAIL_ATOMIC_FETCH_OR_##Size(&this->raw_, op_raw); \
        return res; \
    } \
    UPALWAYSINLINE \
    Value fetch_xor(operand_type operand, memory_order) Volatile noexcept { \
        union { operand_type op; Raw op_raw; }; union { Value res; Raw res_raw; }; op = operand; \
        res_raw = UP_DETAIL_ATOMIC_FETCH_XOR_##Size(&this->raw_, op_raw); \
        return res; \
    } \
    UPALWAYSINLINE \
    Value load(memory_order order) const Volatile noexcept { \
        union { Value res; Raw res_raw; }; \
        if (order != memory_order_seq_cst) { \
            res_raw = this->raw_; \
            _ReadWriteBarrier(); \
        } \
        else { \
            _ReadWriteBarrier(); \
            res_raw = this->raw_; \
            _mm_mfence(); \
        } \
        return res; \
    } \
    UPALWAYSINLINE \
    void store(Value desired, memory_order order) Volatile noexcept { \
        union { Value des; Raw des_raw; }; des = desired; \
        if (order != memory_order_seq_cst) { \
            _ReadWriteBarrier(); \
            this->raw_ = des_raw; \
        } \
        else { \
            UP_DETAIL_ATOMIC_EXCHANGE_##Size(&this->raw_, des_raw); \
        } \
    }

#define UP_DETAIL_DECLARE_ATOMIC_STORAGE(Raw, Size) \
    template <class Value> \
    struct atomic_storage<Value, Size, true> \
    { \
        UPNONCOPYABLE(atomic_storage); \
    private: \
        union { alignas(Size) Value value_; Raw raw_; }; \
    public: \
        typedef Value value_type; \
        typedef Value& reference; \
        typedef Value const const_reference; \
        typedef typename std::conditional<!std::is_pointer<Value>::value, Value, ptrdiff_t>::type operand_type; \
        typedef operand_type const const_operand_reference; \
        static constexpr bool is_lock_free = true; \
        UPALWAYSINLINE UPCONSTEXPR atomic_storage() noexcept { } \
        UPALWAYSINLINE UPCONSTEXPR atomic_storage(Value v) noexcept : value_(v) { } \
        UP_DETAIL_DECLARE_ATOMIC_STORAGE_OPERATIONS(Raw, Size, UP_DETAIL_NOT_VOLATILE) \
        UP_DETAIL_DECLARE_ATOMIC_STORAGE_OPERATIONS(Raw, Size, UP_DETAIL_VOLATILE) \
    };

namespace up { namespace detail
{
    UP_DETAIL_DECLARE_ATOMIC_STORAGE(char, 1)
    UP_DETAIL_DECLARE_ATOMIC_STORAGE(short, 2)
    UP_DETAIL_DECLARE_ATOMIC_STORAGE(long, 4)

#if UP_ARCHITECTURE == UP_ARCHITECTURE_X86

    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_exchange8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_fetch_add8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_fetch_sub8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_fetch_and8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_fetch_or8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_fetch_xor8(__int64 volatile* location, __int64 value);
    extern "C" LIBUPCOREAPI __int64 UPCDECL up_atomic_load8(__int64 const volatile* source);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_store8(__int64 volatile* destination, __int64 value);

    template <class Value>
    struct atomic_storage<Value, 8, true>
    {
        UPNONCOPYABLE(atomic_storage);

    private:

        union { alignas(8) Value value_; __int64 raw_; };

    public:

        typedef Value value_type;
        typedef Value& reference;
        typedef Value const const_reference;
        typedef typename conditional<!is_pointer<Value>::value, Value, ptrdiff_t>::type operand_type;
        typedef operand_type const const_operand_reference;

        static constexpr bool is_lock_free = true;

        UPALWAYSINLINE UPCONSTEXPR
        atomic_storage() noexcept {
        }

        UPALWAYSINLINE UPCONSTEXPR
        atomic_storage(Value v)  noexcept
        : value_(v) {
        }

        UPALWAYSINLINE
        bool compare_exchange_strong(Value& expected, Value desired, memory_order, memory_order) volatile noexcept {
            union { Value exp; __int64 exp_raw; }; union { Value des; __int64 des_raw; };
            union { Value act; __int64 act_raw; }; exp = expected; des = desired;
            act_raw = _InterlockedCompareExchange64(&this->raw_, des_raw, exp_raw);
            if (expected == act) {
                return true;
            }
            expected = act;
            return false;
        }

        UPALWAYSINLINE
        bool compare_exchange_weak(Value& expected, Value desired, memory_order, memory_order) volatile noexcept {
            union { Value exp; __int64 exp_raw; }; union { Value desired; __int64 des_raw; };
            union { Value act; __int64 act_raw; }; exp = expected; des = desired;
            act_raw = _InterlockedCompareExchange64(&this->raw_, des_raw, exp_raw);
            if (expected == act) {
                return true;
            }
            expected = act;
            return false;
        }

        UPALWAYSINLINE
        Value exchange(Value desired, memory_order) volatile noexcept {
            union { Value des; __int64 des_raw; }; union { Value res; __int64 res_raw; }; des = desired;
            res_raw = up_atomic_exchange8(&this->raw_, des_raw);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_add(operand_type operand, memory_order) volatile noexcept {
            union { operand_type op; __int64 op_raw; }; union { Value res; __int64 res_raw; }; op = operand;
            res_raw = up_atomic_fetch_add8(&this->raw_, op_raw);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_sub(operand_type operand, memory_order) volatile noexcept {
            union { operand_type op; __int64 op_raw; }; union { Value res; __int64 res_raw; }; op = operand;
            res_raw = up_atomic_fetch_sub8(&this->raw_, op_raw);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_and(operand_type operand, memory_order) volatile noexcept {
            union { operand_type op; __int64 op_raw; }; union { Value res; __int64 res_raw; }; op = operand;
            res_raw = up_atomic_fetch_and8(&this->raw_, op_raw);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_or(operand_type operand, memory_order) volatile noexcept {
            union { operand_type op; __int64 op_raw; }; union { Value res; __int64 res_raw; }; op = operand;
            res_raw = up_atomic_fetch_or8(&this->raw_, op_raw);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_xor(operand_type operand, memory_order) volatile noexcept {
            union { operand_type op; __int64 op_raw; }; union { Value res; __int64 res_raw; }; op = operand;
            res_raw = up_atomic_fetch_xor8(&this->raw_, op_raw);
            return res;
        }

        UPALWAYSINLINE
        Value load(memory_order) const volatile noexcept {
            union { Value res; __int64 res_raw; };
            res_raw = up_atomic_load8(&this->raw_);
            return res;
        }

        UPALWAYSINLINE
        void store(Value desired, memory_order) volatile noexcept {
            union { Value des; __int64 des_raw; }; des = desired;
            up_atomic_store8(&this->raw_, des_raw);
        }
    };

#elif UP_ARCHITECTURE == UP_ARCHITECTURE_X64

    UP_DETAIL_DECLARE_ATOMIC_STORAGE(long long, 8)

    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_exchange16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_fetch_add16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_fetch_sub16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_fetch_and16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_fetch_or16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_fetch_xor16(__int64 volatile* location, __int64 value_high, __int64 value_low, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_load16(__int64 const volatile* source, __int64* res);
    extern "C" LIBUPCOREAPI void UPCDECL up_atomic_store16(__int64 volatile* destination, __int64 value_high, __int64 value_low);

    template <class Value>
    struct atomic_storage<Value, 16, true>
    {
        UPNONCOPYABLE(atomic_storage);

    private:

        union { alignas(16) Value value_; atomic_int128_t raw_; };

    public:

        typedef Value value_type;
        typedef Value& reference;
        typedef Value const& const_reference;
        typedef Value operand_type;
        typedef Value const& const_operand_reference;

        static constexpr bool is_lock_free = true;
            
        UPALWAYSINLINE UPCONSTEXPR
        atomic_storage() noexcept {
        }
            
        UPALWAYSINLINE UPCONSTEXPR
        atomic_storage(Value const& v) noexcept
        : value_(v) {
        }

        UPALWAYSINLINE
        bool compare_exchange_strong(Value& expected, Value const& desired, memory_order, memory_order) volatile noexcept {
            union { Value exp; atomic_int128_t exp_raw; }; exp = expected;
            union { Value des; atomic_int128_t des_raw; }; des = desired;
            if (_InterlockedCompareExchange128(&this->raw_.low, des_raw.high, des_raw.low, &exp_raw.low) == 1) {
                return true;                
            }
            expected = exp;
            return false;
        }

        UPALWAYSINLINE
        bool compare_exchange_weak(Value& expected, Value const& desired, memory_order, memory_order) volatile noexcept {
            union { Value exp; atomic_int128_t exp_raw; }; exp = expected;
            union { Value des; atomic_int128_t des_raw; }; des = desired;
            exp = expected; des = desired;
            if (_InterlockedCompareExchange128(&this->raw_.low, des_raw.high, des_raw.low, &exp_raw.low) == 1) {
                return true;                
            }
            expected = exp;
            return false;
        }

        UPALWAYSINLINE
        Value exchange(Value const& desired, memory_order) volatile noexcept {
            union { Value des; atomic_int128_t des_raw; }; union { Value res; atomic_int128_t res_raw; }; des = desired;
            up_atomic_exchange16(&this->raw_.low, des_raw.high, des_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_add(Value const& operand, memory_order) volatile noexcept {
            union { Value op; atomic_int128_t op_raw; }; union { Value res; atomic_int128_t res_raw; }; op = operand;
            up_atomic_fetch_add16(&this->raw_.low, op_raw.high, op_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_sub(Value const& operand, memory_order) volatile noexcept {
            union { Value op; atomic_int128_t op_raw; }; union { Value res; atomic_int128_t res_raw; }; op = operand;
            up_atomic_fetch_sub16(&this->raw_.low, op_raw.high, op_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_and(Value const& operand, memory_order) volatile noexcept {
            union { Value op; atomic_int128_t op_raw; }; union { Value res; atomic_int128_t res_raw; }; op = operand;
            up_atomic_fetch_and16(&this->raw_.low, op_raw.high, op_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_or(Value const& operand, memory_order) volatile noexcept {
            union { Value op; atomic_int128_t op_raw; }; union { Value res; atomic_int128_t res_raw; }; op = operand;
            up_atomic_fetch_or16(&this->raw_.low, op_raw.high, op_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value fetch_xor(Value const& operand, memory_order) volatile noexcept {
            union { Value op; atomic_int128_t op_raw; }; union { Value res; atomic_int128_t res_raw; }; op = operand;
            up_atomic_fetch_xor16(&this->raw_.low, op_raw.high, op_raw.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        Value load(memory_order) const volatile noexcept {
            union { Value res; atomic_int128_t res_raw; };
            up_atomic_load16(&this->raw_.low, &res_raw.low);
            return res;
        }

        UPALWAYSINLINE
        void store(Value const& desired, memory_order) volatile noexcept {
            union { Value des; atomic_int128_t des_raw; }; des = desired;
            up_atomic_store16(&this->raw_.low, des_raw.high, des_raw.low);
        }
    };
#endif
}}

#undef UP_DETAIL_DECLARE_ATOMIC_STORAGE
#undef UP_DETAIL_DECLARE_ATOMIC_STORAGE_OPERATIONS
#undef UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_1
#undef UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_2
#undef UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_4
#undef UP_DETAIL_ATOMIC_COMPARE_EXCHANGE_8
#undef UP_DETAIL_ATOMIC_EXCHANGE_1
#undef UP_DETAIL_ATOMIC_EXCHANGE_2
#undef UP_DETAIL_ATOMIC_EXCHANGE_4
#undef UP_DETAIL_ATOMIC_EXCHANGE_8
#undef UP_DETAIL_ATOMIC_FETCH_ADD_1
#undef UP_DETAIL_ATOMIC_FETCH_ADD_2
#undef UP_DETAIL_ATOMIC_FETCH_ADD_4
#undef UP_DETAIL_ATOMIC_FETCH_ADD_8
#undef UP_DETAIL_ATOMIC_FETCH_SUB_1
#undef UP_DETAIL_ATOMIC_FETCH_SUB_2
#undef UP_DETAIL_ATOMIC_FETCH_SUB_4
#undef UP_DETAIL_ATOMIC_FETCH_SUB_8
#undef UP_DETAIL_ATOMIC_FETCH_AND_1
#undef UP_DETAIL_ATOMIC_FETCH_AND_2
#undef UP_DETAIL_ATOMIC_FETCH_AND_4
#undef UP_DETAIL_ATOMIC_FETCH_AND_8
#undef UP_DETAIL_ATOMIC_FETCH_OR_1
#undef UP_DETAIL_ATOMIC_FETCH_OR_2
#undef UP_DETAIL_ATOMIC_FETCH_OR_4
#undef UP_DETAIL_ATOMIC_FETCH_OR_8
#undef UP_DETAIL_ATOMIC_FETCH_XOR_1
#undef UP_DETAIL_ATOMIC_FETCH_XOR_2
#undef UP_DETAIL_ATOMIC_FETCH_XOR_4
#undef UP_DETAIL_ATOMIC_FETCH_XOR_8

#endif
