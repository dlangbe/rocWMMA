/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (C) 2021-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#ifndef ROCWMMA_TYPE_TRAITS_UTILITY_HPP
#define ROCWMMA_TYPE_TRAITS_UTILITY_HPP

namespace rocwmma
{
    namespace detail
    {
        /////////////////////////////////////////
        ///////////  Helper Types  //////////////
        /////////////////////////////////////////

        template <bool B, typename T, typename F>
        struct conditional
        {
        };

        template <typename T, typename F>
        struct conditional<true, T, F>
        {
            using type = T;
        };

        template <typename T, typename F>
        struct conditional<false, T, F>
        {
            using type = F;
        };

        template <bool B, typename T, typename F>
        using conditional_t = typename conditional<B, T, F>::type;

        template <typename DataT, DataT Val>
        struct integral_constant
        {
            static constexpr DataT                value = Val;
            typedef DataT                         value_type;
            typedef integral_constant<DataT, Val> type;
            constexpr                             operator value_type() const noexcept
            {
                return value;
            }
            constexpr value_type operator()() const noexcept
            {
                return value;
            }
        };

        template <typename DataT, DataT Value>
        constexpr DataT integral_constant<DataT, Value>::value;

        typedef integral_constant<bool, true>  true_type;
        typedef integral_constant<bool, false> false_type;

        template <typename T1, typename T2>
        struct is_same : public false_type
        {
        };

        template <typename T>
        struct is_same<T, T> : public true_type
        {
        };

        template <typename T>
        ROCWMMA_HOST_DEVICE constexpr const T& max(const T& a, const T& b)
        {
            return (a < b) ? b : a;
        }

        template <typename T>
        ROCWMMA_HOST_DEVICE constexpr const T& min(const T& a, const T& b)
        {
            return (a < b) ? a : b;
        }

        template <typename...>
        struct __or_;

        template <>
        struct __or_<> : public false_type
        {
        };

        template <typename T>
        struct __or_<T> : public T
        {
        };

        template <typename T1, typename T2>
        struct __or_<T1, T2> : public conditional<T1::value, T1, T2>::type
        {
        };

        template <typename T1, typename T2, typename T3, typename... TN>
        struct __or_<T1, T2, T3, TN...>
            : public conditional<T1::value, T1, __or_<T2, T3, TN...>>::type
        {
        };

        template <typename...>
        struct __and_;

        template <>
        struct __and_<> : public true_type
        {
        };

        template <typename T1>
        struct __and_<T1> : public T1
        {
        };

        template <typename T1, typename T2>
        struct __and_<T1, T2> : public conditional<T1::value, T2, T1>::type
        {
        };

        template <typename T1, typename T2, typename T3, typename... TN>
        struct __and_<T1, T2, T3, TN...>
            : public conditional<T1::value, __and_<T2, T3, TN...>, T1>::type
        {
        };

        template <bool, typename T = void>
        struct enable_if
        {
        };

        template <typename T>
        struct enable_if<true, T>
        {
            typedef T type;
        };

        template <bool B, typename T = void>
        using enable_if_t = typename enable_if<B, T>::type;

        template <bool __v>
        using __bool_constant = integral_constant<bool, __v>;

        template <typename _Pp>
        struct __not_ : public __bool_constant<!bool(_Pp::value)>
        {
        };

        // remove_reference
        template <typename T>
        struct remove_reference
        {
            typedef T type;
        };

        template <typename T>
        struct remove_reference<T&>
        {
            typedef T type;
        };

        template <typename T>
        struct remove_reference<T&&>
        {
            typedef T type;
        };

        // is_lvalue_reference
        template <typename>
        struct is_lvalue_reference : public false_type
        {
        };

        template <typename T>
        struct is_lvalue_reference<T&> : public true_type
        {
        };

        // is_rvalue_reference
        template <typename>
        struct is_rvalue_reference : public false_type
        {
        };

        template <typename T>
        struct is_rvalue_reference<T&&> : public true_type
        {
        };

        // remove_const
        template <typename T>
        struct remove_const
        {
            typedef T type;
        };

        template <typename T>
        struct remove_const<T const>
        {
            typedef T type;
        };

        // remove_volatile
        template <typename T>
        struct remove_volatile
        {
            typedef T type;
        };

        template <typename T>
        struct remove_volatile<T volatile>
        {
            typedef T type;
        };

        // remove_cv
        template <typename T>
        struct remove_cv
        {
            typedef typename remove_const<typename remove_volatile<T>::type>::type type;
        };

        // remove_extent
        template <typename T>
        struct remove_extent
        {
            typedef T type;
        };

        template <typename T, std::size_t _Size>
        struct remove_extent<T[_Size]>
        {
            typedef T type;
        };

        template <typename T>
        struct remove_extent<T[]>
        {
            typedef T type;
        };

        // is_void
        template <typename>
        struct __is_void_helper : public false_type
        {
        };

        template <>
        struct __is_void_helper<void> : public true_type
        {
        };

        template <typename T>
        struct is_void : public __is_void_helper<typename remove_cv<T>::type>::type
        {
        };

        // is_reference
        template <typename T>
        struct is_reference : public __or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type
        {
        };

        // is_function
        template <typename>
        struct is_function : public false_type
        {
        };

        // is_object
        template <typename T>
        struct is_object : public __not_<__or_<is_function<T>, is_reference<T>, is_void<T>>>::type
        {
        };

        // __is_referenceable
        template <typename T>
        struct __is_referenceable : public __or_<is_object<T>, is_reference<T>>::type{};

        // add_pointer
        template <typename T, bool = __or_<__is_referenceable<T>, is_void<T>>::value>
        struct __add_pointer_helper
        {
            typedef T type;
        };

        template <typename T>
        struct __add_pointer_helper<T, true>
        {
            typedef typename remove_reference<T>::type* type;
        };

        template <typename T>
        struct add_pointer : public __add_pointer_helper<T>
        {
        };

        // is_array
        template <typename>
        struct is_array : public false_type
        {
        };

        template <typename T, std::size_t _Size>
        struct is_array<T[_Size]> : public true_type
        {
        };

        template <typename T>
        struct is_array<T[]> : public true_type
        {
        };

        // decay selectors
        template <typename _Up,
                  bool _IsArray    = is_array<_Up>::value,
                  bool _IsFunction = is_function<_Up>::value>
        struct __decay_selector;

        template <typename _Up>
        struct __decay_selector<_Up, false, false>
        {
            typedef typename remove_cv<_Up>::type __type;
        };

        template <typename _Up>
        struct __decay_selector<_Up, true, false>
        {
            typedef typename remove_extent<_Up>::type* __type;
        };

        template <typename _Up>
        struct __decay_selector<_Up, false, true>
        {
            typedef typename add_pointer<_Up>::type __type;
        };

        // decay
        template <typename T>
        class decay
        {
            typedef typename remove_reference<T>::type __remove_type;

        public:
            typedef typename __decay_selector<__remove_type>::__type type;
        };

        template <typename T>
        using decay_t = typename decay<T>::type;

        template <typename T, class U>
        inline constexpr bool is_same_v = is_same<T, U>::value;

    } // namespace detail
} // namespace rocwmma

// namespace rocwmma
// {
// #if defined(__HIPCC_RTC__)
//     using detail::decay_t;
//     using detail::enable_if_t;
//     using detail::conditional_t;
//     using detail::is_same;
// #else
//     // Use STL implementation otherwise
//     #include <type_traits>
//     #include <limits>

//     using std::decay_t;
//     using std::enable_if_t;
//     using std::conditional_t;
//     using std::is_same;
// #endif // defined(__HIPCC_RTC__)
// }

#include "limits.hpp"

namespace rocwmma
{
    template <typename T>
    using numeric_limits = detail::numeric_limits<T>;
} // namespace rocwmma

#endif // ROCWMMA_TYPE_TRAITS_UTILITY_HPP
