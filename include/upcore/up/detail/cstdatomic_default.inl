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

#ifndef UP_DETAIL_CSTDATOMIC_DEFAULT_INL
#define UP_DETAIL_CSTDATOMIC_DEFAULT_INL

#include <up/cassert.hpp>
#include <up/cstdalign.hpp>
#include <type_traits>

#ifndef ATOMIC_VAR_INIT
#   define ATOMIC_VAR_INIT(var) var
#endif

namespace up
{
    template <class T>
    inline UPALWAYSINLINE
    T kill_dependency(T const& x) {
        T t(x);
        return t;
    }

    enum memory_order
    {
        memory_order_relaxed,
        memory_order_consume,
        memory_order_acquire,
        memory_order_release,
        memory_order_acq_rel,
        memory_order_seq_cst
    };
}

#define UP_DETAIL_NOT_VOLATILE
#define UP_DETAIL_VOLATILE volatile

#if (UP_COMPILER == UP_COMPILER_GCC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/cstdatomic_flag_gcc_x86_x64.inl>
#       include <up/detail/cstdatomic_fence_gcc_x86_x64.inl>
#       include <up/detail/cstdatomic_pause_gcc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#elif (UP_COMPILER == UP_COMPILER_MSVC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/cstdatomic_flag_msvc_x86_x64.inl>
#       include <up/detail/cstdatomic_fence_msvc_x86_x64.inl>
#       include <up/detail/cstdatomic_pause_msvc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#else
#   error "Compiler not currently supported for atomic operations!"
#endif

#define UP_DETAIL_DEFINE_ATOMIC_FLAG_FREE_FUNCTIONS(Volatile) \
    extern LIBUPCOREAPI \
    void atomic_flag_spin_lock_explicit(atomic_flag Volatile* flag, memory_order order) noexcept; \
    inline UPALWAYSINLINE \
    void atomic_flag_spin_lock(atomic_flag Volatile* flag) noexcept { \
        atomic_flag_spin_lock_explicit(flag, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    void atomic_flag_clear(atomic_flag Volatile* flag) noexcept { \
        flag->clear(); \
    } \
    inline UPALWAYSINLINE \
    void atomic_flag_clear_explicit(atomic_flag Volatile* flag, memory_order order) noexcept{ \
        flag->clear(order); \
    } \
    inline UPALWAYSINLINE \
    bool atomic_flag_test_and_set(atomic_flag Volatile* flag) noexcept { \
        return flag->test_and_set(); \
    } \
    inline UPALWAYSINLINE \
    bool atomic_flag_test_and_set_explicit(atomic_flag Volatile* flag, memory_order order) noexcept { \
        return flag->test_and_set(order); \
    } \
    inline UPALWAYSINLINE \
    bool atomic_flag_test_test_and_set(atomic_flag Volatile* flag) noexcept { \
        return flag->test_test_and_set(); \
    } \
    inline UPALWAYSINLINE \
    bool atomic_flag_test_test_and_set_explicit(atomic_flag Volatile* flag, memory_order order) noexcept { \
        return flag->test_test_and_set(order); \
    }

namespace up
{
    UP_DETAIL_DEFINE_ATOMIC_FLAG_FREE_FUNCTIONS(UP_DETAIL_NOT_VOLATILE)
    UP_DETAIL_DEFINE_ATOMIC_FLAG_FREE_FUNCTIONS(UP_DETAIL_VOLATILE)
}

#undef UP_DETAIL_DEFINE_ATOMIC_FLAG_FREE_FUNCTIONS

namespace up { namespace detail
{
    struct atomic_int128_t
    {
#if (UP_BYTE_ORDER == UP_LITTLE_ENDIAN)
        long long low;
        long long high;
#else
        long long high;
        long long low;
#endif
    };

    template <size_t Size>
    struct UPHIDDEN atomic_storage_size
    {
        static constexpr size_t s1 = Size - 1;
        static constexpr size_t s2 = s1 | (s1 >> 1);
        static constexpr size_t s3 = s2 | (s2 >> 2);
        static constexpr size_t s4 = s3 | (s3 >> 4);
        static constexpr size_t s5 = s4 | (s4 >> 8);
        static constexpr size_t s6 = s5 | (s5 >> 16);
#if (SIZE_MAX == 0xFFFFFFFFu)
        static constexpr size_t value = s6 + 1;
#elif (SIZE_MAX == 0xFFFFFFFFFFFFFFFFull)
        static constexpr size_t s7 = s6 | (s6 >> 32);
        static constexpr size_t value = s7 + 1;
#else
#   error "Unexpected sizeof(size_t)"
#endif
    };

#define UP_DETAIL_ATOMIC_STORAGE_DEFAULT_OPERATIONS(Volatile) \
    UPALWAYSINLINE \
    Value load(memory_order) const Volatile noexcept { \
        lock_type lock(this); \
        return value_; \
    } \
    UPALWAYSINLINE \
    void store(const_reference desired, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        value_ = desired; \
    } \
    UPALWAYSINLINE \
    Value exchange(const_reference desired, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = desired; \
        return result; \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_strong(reference expected, const_reference desired, memory_order, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        if (expected == value_) { \
            value_ = desired; \
            return true; \
        } \
        else { \
            expected = value_; \
            return false; \
        } \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_weak(reference expected, const_reference desired, memory_order, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        if (expected == value_) { \
            value_ = desired; \
            return true; \
        } \
        else { \
            expected = value_; \
            return false; \
        } \
    } \
    UPALWAYSINLINE \
    Value fetch_add(const_operand_reference operand, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = value_ + operand; \
        return result; \
    } \
    UPALWAYSINLINE \
    Value fetch_sub(const_operand_reference operand, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = value_ - operand; \
        return result; \
    } \
    UPALWAYSINLINE \
    Value fetch_and(const_operand_reference operand, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = value_ & operand; \
        return result; \
    } \
    UPALWAYSINLINE \
    Value fetch_or(const_operand_reference operand, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = value_ | operand; \
        return result; \
    } \
    UPALWAYSINLINE \
    Value fetch_xor(const_operand_reference operand, memory_order) Volatile noexcept { \
        lock_type lock(this); \
        Value result = value_; \
        value_ = value_ ^ operand; \
        return result; \
    }

    template <class Value, size_t Size = atomic_storage_size<sizeof(Value)>::value, bool Trivial = std::is_trivial<Value>::value>
    struct atomic_storage
    {
        UPNONCOPYABLE(atomic_storage);

    public:

        typedef Value value_type;
        typedef Value& reference;
        typedef typename std::conditional<std::is_scalar<Value>::value, Value, Value const&>::type const_reference;
        typedef typename std::conditional<std::is_pointer<Value>::value, ptrdiff_t, Value>::type operand_type;
        typedef typename std::conditional<std::is_scalar<operand_type>::value, operand_type, operand_type const&>::type const_operand_reference;
        static constexpr bool is_lock_free = false;
        UPALWAYSINLINE UPCONSTEXPR atomic_storage() noexcept : locked_(ATOMIC_FLAG_INIT) { }
        UPALWAYSINLINE UPCONSTEXPR atomic_storage(const_reference v) noexcept : value_(v), locked_(ATOMIC_FLAG_INIT) { }
        UP_DETAIL_ATOMIC_STORAGE_DEFAULT_OPERATIONS(UP_DETAIL_NOT_VOLATILE)
        UP_DETAIL_ATOMIC_STORAGE_DEFAULT_OPERATIONS(UP_DETAIL_VOLATILE)

    private:

        struct lock_type
        {
            UPNONCOPYABLE(lock_type);
        public:
            UPALWAYSINLINE lock_type(atomic_storage const volatile* s) noexcept : storage_(s) { ::up::atomic_flag_spin_lock_explicit(&storage_->locked_, memory_order_acquire); }
            UPALWAYSINLINE ~lock_type() noexcept { ::up::atomic_flag_clear_explicit(&storage_->locked_, memory_order_release); }
        private:
            atomic_storage const volatile* const storage_;
        };

        value_type volatile value_;
        mutable atomic_flag volatile locked_;
    };
#undef UP_DETAIL_ATOMIC_STORAGE_DEFAULT_OPERATIONS
}}

#if (UP_COMPILER == UP_COMPILER_GCC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/cstdatomic_storage_gcc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#elif (UP_COMPILER == UP_COMPILER_MSVC)
#   if (UP_ARCHITECTURE == UP_ARCHITECTURE_X86) || (UP_ARCHITECTURE == UP_ARCHITECTURE_X64)
#       include <up/detail/cstdatomic_storage_msvc_x86_x64.inl>
#   else
#       error "Architecture not currently supported for atomic operations!"
#   endif
#else
#   error "Compiler not currently supported for atomic operations!"
#endif

namespace up { namespace detail
{
    inline UPALWAYSINLINE UPCONSTEXPR
    memory_order calculate_failure_order(memory_order order) noexcept {
        return (order != memory_order_release) ? ((order != memory_order_acq_rel) ? order : memory_order_acquire) : memory_order_relaxed;
    }

#define UP_DETAIL_ATOMIC_BASE_OPERATIONS(Volatile) \
    UPALWAYSINLINE \
    bool is_lock_free() const Volatile noexcept { \
        return storage_type::is_lock_free; \
    } \
    UPALWAYSINLINE \
    T operator=(const_reference t) Volatile noexcept { \
        this->storage_.store(t, memory_order_seq_cst); \
        return t; \
    } \
    UPALWAYSINLINE \
    operator T() const Volatile noexcept { \
        return this->storage_.load(memory_order_seq_cst); \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_strong(reference expected, const_reference desired, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.compare_exchange_strong(expected, desired, order, ::up::detail::calculate_failure_order(order)); \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_strong(reference expected, const_reference desired, memory_order success_order, memory_order failure_order) Volatile noexcept { \
        return this->storage_.compare_exchange_strong(expected, desired, success_order, failure_order); \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_weak(reference expected, const_reference desired, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.compare_exchange_weak(expected, desired, order, ::up::detail::calculate_failure_order(order)); \
    } \
    UPALWAYSINLINE \
    bool compare_exchange_weak(reference expected, const_reference desired, memory_order success_order, memory_order failure_order) Volatile noexcept { \
        return this->storage_.compare_exchange_weak(expected, desired, success_order, failure_order); \
    } \
    UPALWAYSINLINE \
    T exchange(const_reference desired, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.exchange(desired, order); \
    } \
    UPALWAYSINLINE \
    T load(memory_order order = memory_order_seq_cst) const Volatile noexcept { \
        UPASSERT((order != memory_order_release) && (order != memory_order_acq_rel)); \
        return this->storage_.load(order); \
    } \
    UPALWAYSINLINE \
    void store(const_reference desired, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        UPASSERT((order != memory_order_consume) && (order != memory_order_acquire) && (order != memory_order_acq_rel)); \
        this->storage_.store(desired, order); \
    }

#define UP_DETAIL_ATOMIC_ADDRESS_BASE_OPERATIONS(Volatile) \
    UPALWAYSINLINE \
    T2* operator+=(ptrdiff_t value) Volatile noexcept { \
        value *= sizeof(Object); \
        uintptr_t address = reinterpret_cast<intptr_t>(this->storage_.fetch_add(value, memory_order_seq_cst)); \
        return reinterpret_cast<T2*>(address + value); \
    } \
    UPALWAYSINLINE \
    T2* operator-=(ptrdiff_t value) Volatile noexcept { \
        value *= sizeof(Object); \
        uintptr_t address = reinterpret_cast<uintptr_t>(this->storage_.fetch_sub(value, memory_order_seq_cst)); \
        return reinterpret_cast<T2*>(address - value); \
    } \
    UPALWAYSINLINE \
    T2* operator++() Volatile noexcept { \
        uintptr_t address = reinterpret_cast<uintptr_t>(this->storage_.fetch_add(sizeof(Object), memory_order_seq_cst)); \
        return reinterpret_cast<T2*>(address + sizeof(Object)); \
    } \
    UPALWAYSINLINE \
    T2* operator++(int) Volatile noexcept { \
        return this->storage_.fetch_add(sizeof(Object), memory_order_seq_cst); \
    } \
    UPALWAYSINLINE \
    T2* operator--() Volatile noexcept { \
        uintptr_t address = reinterpret_cast<uintptr_t>(this->storage_.fetch_sub(sizeof(Object), memory_order_seq_cst)); \
        return reinterpret_cast<T2*>(address - sizeof(Object)); \
    } \
    UPALWAYSINLINE \
    T2* operator--(int) Volatile noexcept { \
        return this->storage_.fetch_sub(sizeof(Object), memory_order_seq_cst); \
    } \
    UPALWAYSINLINE \
    T2* fetch_add(ptrdiff_t value, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_add(value * sizeof(Object), order); \
    } \
    UPALWAYSINLINE \
    T2* fetch_sub(ptrdiff_t value, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_sub(value * sizeof(Object), order); \
    }

#define UP_DETAIL_ATOMIC_INTEGRAL_BASE_OPERATIONS(Volatile) \
    UPALWAYSINLINE \
    T2 operator+=(const_operand_reference operand) Volatile noexcept { \
        return this->storage_.fetch_add(operand, memory_order_seq_cst) + operand; \
    } \
    UPALWAYSINLINE \
    T2 operator-=(const_operand_reference operand) Volatile noexcept { \
        return this->storage_.fetch_sub(operand, memory_order_seq_cst) - operand; \
    } \
    UPALWAYSINLINE \
    T2 operator&=(const_operand_reference operand) Volatile noexcept { \
        return this->storage_.fetch_and(operand, memory_order_seq_cst) & operand; \
    } \
    UPALWAYSINLINE \
    T2 operator|=(const_operand_reference operand) Volatile noexcept { \
        return this->storage_.fetch_or(operand, memory_order_seq_cst) | operand; \
    } \
    UPALWAYSINLINE \
    T2 operator^=(const_operand_reference operand) Volatile noexcept { \
        return this->storage_.fetch_xor(operand, memory_order_seq_cst) ^ operand; \
    } \
    UPALWAYSINLINE \
    T2 operator++() Volatile noexcept { \
        return this->storage_.fetch_add(1, memory_order_seq_cst) + 1; \
    } \
    UPALWAYSINLINE \
    T2 operator++(int) Volatile noexcept { \
        return this->storage_.fetch_add(1, memory_order_seq_cst); \
    } \
    UPALWAYSINLINE \
    T2 operator--() Volatile noexcept { \
        return this->storage_.fetch_sub(1, memory_order_seq_cst) - 1; \
    } \
    UPALWAYSINLINE \
    T2 operator--(int) Volatile noexcept { \
        return this->storage_.fetch_sub(1, memory_order_seq_cst); \
    } \
    UPALWAYSINLINE \
    T2 fetch_add(const_operand_reference operand, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_add(operand, order); \
    } \
    UPALWAYSINLINE \
    T2 fetch_sub(const_operand_reference operand, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_sub(operand, order); \
    } \
    UPALWAYSINLINE \
    T2 fetch_and(const_operand_reference operand, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_and(operand, order); \
    } \
    UPALWAYSINLINE \
    T2 fetch_or(const_operand_reference operand, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_or(operand, order); \
    } \
    UPALWAYSINLINE \
    T2 fetch_xor(const_operand_reference operand, memory_order order = memory_order_seq_cst) Volatile noexcept { \
        return this->storage_.fetch_xor(operand, order); \
    }
    
#ifndef LIBUPCORE_NO_EXTERN_TEMPLATES
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<void*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<void const*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<void volatile*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<void const volatile*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<signed char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<unsigned short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<unsigned int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<unsigned long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<unsigned long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<bool>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_storage<char32_t>;
#endif
#endif

    template <class T>
    struct atomic_base
    {
        UPNONCOPYABLE(atomic_base);
    protected:
        typedef atomic_storage<T> storage_type;
    public:
        typedef typename storage_type::reference reference;
        typedef typename storage_type::const_reference const_reference;
        UPDEFAULTCTOR(atomic_base);
        UPALWAYSINLINE UPCONSTEXPR atomic_base(const_reference t) noexcept : storage_(t) { }
        UP_DETAIL_ATOMIC_BASE_OPERATIONS(UP_DETAIL_NOT_VOLATILE)
        UP_DETAIL_ATOMIC_BASE_OPERATIONS(UP_DETAIL_VOLATILE)
    protected:
        storage_type storage_;
    };

#ifndef LIBUPCORE_NO_EXTERN_TEMPLATES
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<bool>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<void*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<void const*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<void volatile*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<void const volatile*>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<signed char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<unsigned short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<unsigned int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<unsigned long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<unsigned long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_base<char32_t>;
#endif
#endif

    template <class Pointer, class Object>
    struct atomic_address_base;

    template <class T2, class Object>
    struct atomic_address_base<T2*, Object> : public atomic_base<T2*>
    {
        UPNONCOPYABLE(atomic_address_base);
    public:
        UPDEFAULTCTOR(atomic_address_base);
        UPALWAYSINLINE UPCONSTEXPR atomic_address_base(T2* t) noexcept : atomic_base<T2*>(t) { }
        UP_DETAIL_ATOMIC_ADDRESS_BASE_OPERATIONS(UP_DETAIL_NOT_VOLATILE)
        UP_DETAIL_ATOMIC_ADDRESS_BASE_OPERATIONS(UP_DETAIL_VOLATILE)
    };

#ifndef LIBUPCORE_NO_EXTERN_TEMPLATES
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_address_base<void*, unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_address_base<void const*, unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_address_base<void volatile*, unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_address_base<void const volatile*, unsigned char>;
#endif

    template <class T2>
    struct atomic_integral_base : public atomic_base<T2>
    {
        UPNONCOPYABLE(atomic_integral_base);
    public:
        typedef typename atomic_base<T2>::const_reference const_reference;
        typedef typename atomic_base<T2>::storage_type::const_operand_reference const_operand_reference;
        UPDEFAULTCTOR(atomic_integral_base);
        UPALWAYSINLINE UPCONSTEXPR atomic_integral_base(const_reference t) noexcept : atomic_base<T2>(t) { }
        UP_DETAIL_ATOMIC_INTEGRAL_BASE_OPERATIONS(UP_DETAIL_NOT_VOLATILE)
        UP_DETAIL_ATOMIC_INTEGRAL_BASE_OPERATIONS(UP_DETAIL_VOLATILE)
    };

#ifndef LIBUPCORE_NO_EXTERN_TEMPLATES
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<signed char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<unsigned char>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<unsigned short>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<unsigned int>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<unsigned long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<unsigned long long>;
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<char>;
#ifndef UP_NO_NATIVE_WCHAR_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<wchar_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<char16_t>;
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    LIBUPCOREEXTERN template struct LIBUPCOREAPI atomic_integral_base<char32_t>;
#endif
#endif

#undef UP_DETAIL_ATOMIC_BASE_OPERATIONS
#undef UP_DETAIL_ATOMIC_INTEGRAL_BASE_OPERATIONS
#undef UP_DETAIL_ATOMIC_ADDRESS_BASE_OPERATIONS
}}

namespace up
{
    template <class U> struct atomic;

    template <class U>
    struct UPVISIBLE atomic : public ::up::detail::atomic_base<U>
    {
        UPNONCOPYABLE(atomic);
    public:
        UPDEFAULTCTOR(atomic);
        UPALWAYSINLINE UPCONSTEXPR atomic(U u) : ::up::detail::atomic_base<U>(u) { }
    };

    template <>
    struct LIBUPCOREAPI atomic<bool> : public ::up::detail::atomic_base<bool>
    {
        UPNONCOPYABLE(atomic);
    public:
        UPDEFAULTCTOR(atomic);
        UPALWAYSINLINE UPCONSTEXPR atomic(bool b) : ::up::detail::atomic_base<bool>(b) { }
    };

    template <class U>
    struct UPVISIBLE atomic<U*> : public ::up::detail::atomic_address_base<U*, U>
    {
        UPNONCOPYABLE(atomic);
    public:
        UPDEFAULTCTOR(atomic);
        UPALWAYSINLINE UPCONSTEXPR atomic(U* a) : ::up::detail::atomic_address_base<U*, U>(a) { }
    };

#define UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION(U) \
    template <> struct LIBUPCOREAPI atomic<U> : public ::up::detail::atomic_address_base<U, unsigned char> { \
        UPNONCOPYABLE(atomic); \
    public: \
        UPDEFAULTCTOR(atomic); \
        UPALWAYSINLINE UPCONSTEXPR atomic(U a) : ::up::detail::atomic_address_base<U, unsigned char>(a) { } \
    };

#define UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(U) \
    template <> struct LIBUPCOREAPI atomic<U> : public ::up::detail::atomic_integral_base<U> { \
        UPNONCOPYABLE(atomic); \
    public: \
        UPDEFAULTCTOR(atomic); \
        UPALWAYSINLINE UPCONSTEXPR atomic(U u) : ::up::detail::atomic_integral_base<U>(u) { } \
    };

    UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION(void*);
    UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION(void const*);
    UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION(void volatile*);
    UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION(void const volatile*);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(signed char);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(short);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(int);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(long);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(long long);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned char);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned short);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned int);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned long);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned long long);
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(char);
#ifndef UP_NO_NATIVE_WCHAR_T
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(wchar_t);
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(char16_t);
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION(char32_t);
#endif

#undef UP_DETAIL_DECLARE_ATOMIC_VOID_ADDRESS_SPECIALIZATION
#undef UP_DETAIL_DECLARE_ATOMIC_INTEGRAL_SPECIALIZATION

    typedef atomic<void*> atomic_address;
    typedef atomic<void const*> atomic_const_address;
    typedef atomic<void volatile*> atomic_volatile_address;
    typedef atomic<void const volatile*> atomic_const_volatile_address;
    typedef atomic<bool> atomic_bool;
    typedef atomic<char> atomic_char;
    typedef atomic<signed char> atomic_schar;
    typedef atomic<unsigned char> atomic_uchar;
    typedef atomic<short> atomic_short;
    typedef atomic<unsigned short> atomic_ushort;
    typedef atomic<int> atomic_int;
    typedef atomic<unsigned int> atomic_uint;
    typedef atomic<long> atomic_long;
    typedef atomic<unsigned long> atomic_ulong;
    typedef atomic<long long> atomic_llong;
    typedef atomic<unsigned long long> atomic_ullong;
    typedef atomic<wchar_t> atomic_wchar_t;
    typedef atomic<char16_t> atomic_char16_t;
    typedef atomic<char32_t> atomic_char32_t;
    typedef atomic<int8_t> atomic_int8_t;
    typedef atomic<uint8_t> atomic_uint8_t;
    typedef atomic<int16_t> atomic_int16_t;
    typedef atomic<uint16_t> atomic_uint16_t;
    typedef atomic<int32_t> atomic_int32_t;
    typedef atomic<uint32_t> atomic_uint32_t;
    typedef atomic<int64_t> atomic_int64_t;
    typedef atomic<uint64_t> atomic_uint64_t;
    typedef atomic<int_least8_t> atomic_int_least8_t;
    typedef atomic<uint_least8_t> atomic_uint_least8_t;
    typedef atomic<int_least16_t> atomic_int_least16_t;
    typedef atomic<uint_least16_t> atomic_uint_least16_t;
    typedef atomic<int_least32_t> atomic_int_least32_t;
    typedef atomic<uint_least32_t> atomic_uint_least32_t;
    typedef atomic<int_least64_t> atomic_int_least64_t;
    typedef atomic<uint_least64_t> atomic_uint_least64_t;
    typedef atomic<int_fast8_t> atomic_int_fast8_t;
    typedef atomic<uint_fast8_t> atomic_uint_fast8_t;
    typedef atomic<int_fast16_t> atomic_int_fast16_t;
    typedef atomic<uint_fast16_t> atomic_uint_fast16_t;
    typedef atomic<int_fast32_t> atomic_int_fast32_t;
    typedef atomic<uint_fast32_t> atomic_uint_fast32_t;
    typedef atomic<int_fast64_t> atomic_int_fast64_t;
    typedef atomic<uint_fast64_t> atomic_uint_fast64_t;
    typedef atomic<intptr_t> atomic_intptr_t;
    typedef atomic<uintptr_t> atomic_uintptr_t;
    typedef atomic<size_t> atomic_size_t;
    typedef atomic<ptrdiff_t> atomic_ptrdiff_t;
    typedef atomic<intmax_t> atomic_intmax_t;
    typedef atomic<uintmax_t> atomic_uintmax_t;

#define UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS_IMPL(Volatile) \
    template <class T> \
    inline UPALWAYSINLINE \
    bool atomic_is_lock_free(atomic<T> const Volatile* a) noexcept { \
        return a->is_lock_free(); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    void atomic_init(atomic<T> Volatile* a, T desired) noexcept { \
        a->store(desired, memory_order_relaxed); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    void atomic_store(atomic<T> Volatile* a, T desired) noexcept { \
        a->store(desired, memory_order_seq_cst); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    void atomic_store_explicit(atomic<T> Volatile* a, T desired, memory_order order) noexcept { \
        a->store(desired, order); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    T atomic_load(atomic<T> const Volatile* a) noexcept noexcept { \
        return a->load(memory_order_seq_cst); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    T atomic_load_explicit(atomic<T> const Volatile* a, memory_order order) noexcept { \
        return a->load(order); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    T atomic_exchange(atomic<T> Volatile* a, T desired) noexcept { \
        return a->exchange(desired, memory_order_seq_cst); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    T atomic_exchange_explicit(atomic<T> Volatile* a, T desired, memory_order order) noexcept { \
        return a->exchange(desired, order); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    bool atomic_compare_exchange_strong(atomic<T> Volatile* a, T* expected, T desired) noexcept { \
        return a->compare_exchange_strong(*expected, desired, memory_order_seq_cst, memory_order_seq_cst); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    bool atomic_compare_exchange_strong_explicit(atomic<T> Volatile* a, T* expected, T desired, memory_order success_order, memory_order failure_order) noexcept { \
        return a->compare_exchange_strong(*expected, desired, success_order, failure_order); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    bool atomic_compare_exchange_weak(atomic<T> Volatile* a, T* expected, T desired) { \
        return a->compare_exchange_weak(*expected, desired, memory_order_seq_cst, memory_order_seq_cst); \
    } \
    template <class T> \
    inline UPALWAYSINLINE \
    bool atomic_compare_exchange_weak_explicit(atomic<T> Volatile* a, T* expected, T desired, memory_order success_order, memory_order failure_order) noexcept { \
        return a->compare_exchange_weak(*expected, desired, success_order, failure_order); \
    }

#define UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS() \
    UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS_IMPL(UP_DETAIL_NOT_VOLATILE)
    UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS_IMPL(UP_DETAIL_VOLATILE)

#define UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, Volatile) \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_add(AtomicType Volatile* a, ptrdiff_t operand) noexcept { \
        return a->fetch_add(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_add_explicit(AtomicType Volatile* a, ptrdiff_t operand, memory_order order) noexcept { \
        return a->fetch_add(operand, order); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_sub(AtomicType Volatile* a, ptrdiff_t operand) noexcept { \
        return a->fetch_sub(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_sub_explicit(AtomicType Volatile* a, ptrdiff_t operand, memory_order order) noexcept { \
        return a->fetch_sub(operand, order); \
    }

#define UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS(AtomicType, ValueType) \
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, UP_DETAIL_NOT_VOLATILE) \
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, UP_DETAIL_VOLATILE)

#define UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, Volatile) \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_add(AtomicType Volatile* a, ValueType operand) noexcept { \
        return a->fetch_add(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_add_explicit(AtomicType Volatile* a, ValueType operand, memory_order order) noexcept { \
        return a->fetch_add(operand, order); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_sub(AtomicType Volatile* a, ValueType operand) noexcept { \
        return a->fetch_sub(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_sub_explicit(AtomicType Volatile* a, ValueType operand, memory_order order) noexcept { \
        return a->fetch_sub(operand, order); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_and(AtomicType Volatile* a, ValueType operand) noexcept { \
        return a->fetch_and(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_and_explicit(AtomicType Volatile* a, ValueType operand, memory_order order) noexcept { \
        return a->fetch_and(operand, order); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_or(AtomicType Volatile* a, ValueType operand) noexcept { \
        return a->fetch_or(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_or_explicit(AtomicType Volatile* a, ValueType operand, memory_order order) noexcept { \
        return a->fetch_or(operand, order); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_xor(AtomicType Volatile* a, ValueType operand) noexcept { \
        return a->fetch_xor(operand, memory_order_seq_cst); \
    } \
    inline UPALWAYSINLINE \
    ValueType atomic_fetch_xor_explicit(AtomicType Volatile* a, ValueType operand, memory_order order) noexcept { \
        return a->fetch_xor(operand, order); \
    }

#define UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(AtomicType, ValueType) \
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, UP_DETAIL_NOT_VOLATILE) \
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS_IMPL(AtomicType, ValueType, UP_DETAIL_VOLATILE)

    UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS()
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS(atomic_address, void*)
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS(atomic_const_address, void const*)
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS(atomic_volatile_address, void volatile*)
    UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS(atomic_const_volatile_address, void const volatile*)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_schar, signed char)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_short, short)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_int, int)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_long, long)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_llong, long long)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_uchar, unsigned char)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_ushort, unsigned short)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_uint, unsigned int)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_ulong, unsigned long)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_ullong, unsigned long long)
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_char, char)
#ifndef UP_NO_NATIVE_WCHAR_T
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_wchar_t, wchar_t)
#endif
#ifndef UP_NO_NATIVE_CHAR16_T
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_char16_t, char16_t)
#endif
#ifndef UP_NO_NATIVE_CHAR32_T
    UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS(atomic_char32_t, char32_t)
#endif

#undef UP_DETAIL_VOLATILE
#undef UP_DETAIL_NOT_VOLATILE
#undef UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS
#undef UP_DETAIL_DEFINE_ATOMIC_FREE_FUNCTIONS_IMPL
#undef UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS
#undef UP_DETAIL_DEFINE_ATOMIC_ADDRESS_FREE_FUNCTIONS_IMPL
#undef UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS
#undef UP_DETAIL_DEFINE_ATOMIC_INTEGERAL_FREE_FUNCTIONS_IMPL

}

#endif
