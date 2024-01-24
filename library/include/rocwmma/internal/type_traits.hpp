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

#ifndef ROCWMMA_TYPE_TRAITS_HPP
#define ROCWMMA_TYPE_TRAITS_HPP

#include "utility/type_traits.hpp"

namespace rocwmma
{
#if !defined(__HIPCC_RTC__)
    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
    constexpr auto maxExactInteger() -> decltype(numeric_limits<T>::max())
    {
        return numeric_limits<T>::max();
    }

    template <typename T,
              typename std::
                  enable_if_t<std::is_floating_point<T>::value && numeric_limits<T>::digits, int>
              = 0>
    constexpr auto maxExactInteger() ->
        typename std::conditional_t<std::is_same<T, float64_t>::value, int64_t, int32_t>
    {
        using RetT =
            typename std::conditional_t<std::is_same<T, float64_t>::value, int64_t, int32_t>;
        return ((RetT)1 << numeric_limits<T>::digits);
    }

    template <typename T,
              typename std::enable_if_t<
#if !ROCWMMA_NO_HALF
                  std::is_same<T, hfloat16_t>::value ||
#endif // !ROCWMMA_NO_HALF
                      std::is_same<T, float16_t>::value,
                  int>
              = 0>
    constexpr auto maxExactInteger() -> int32_t
    {
        // f16 mantissa is 10 bits
        return ((int32_t)1 << 11);
    }

    template <typename T, typename std::enable_if_t<std::is_same<T, bfloat16_t>::value, int> = 0>
    constexpr auto maxExactInteger() -> int32_t
    {
        // b16 mantissa is 7 bits
        return ((int32_t)1 << 8);
    }

    template <typename T, typename std::enable_if_t<std::is_same<T, float8_t>::value, int> = 0>
    constexpr auto maxExactInteger() -> int32_t
    {
        // f8 mantissa is 3 bits
        return ((int32_t)1 << 4);
    }

    template <typename T, typename std::enable_if_t<std::is_same<T, bfloat8_t>::value, int> = 0>
    constexpr auto maxExactInteger() -> int32_t
    {
        // bf8 mantissa is 2 bits
        return ((int32_t)1 << 3);
    }

    template <typename T, typename std::enable_if_t<std::is_same<T, xfloat32_t>::value, int> = 0>
    constexpr auto maxExactInteger() -> int32_t
    {
        // xf32 mantissa is 7 bits
        return ((int32_t)1 << 8);
    }
#endif // !defined(__HIPCC_RTC__)

} // namespace rocwmma

#endif // ROCWMMA_TYPE_TRAITS_HPP
