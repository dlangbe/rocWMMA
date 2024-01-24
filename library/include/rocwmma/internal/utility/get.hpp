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

#ifndef ROCWMMA_GET_HPP
#define ROCWMMA_GET_HPP

#include "../types.hpp"

namespace rocwmma
{
    namespace detail
    {
        template <uint32_t Idx, typename DataT>
        ROCWMMA_HOST_DEVICE constexpr auto get(DataT&& obj)
        {
            return reinterpret_cast<DataT*>(obj)[Idx];
        }
    }
}

namespace rocwmma
{
    // Use custom rocwmma get implementation for cases where STL is not available
#ifdef __HIPCC_RTC__
    using detail::get;

    // Use STL implementation otherwise
#else
#include <tuple>
    using std::get;
#endif // __HIPCC_RTC__

    // VecT extensions
    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr inline DataT& get(HIP_vector_type<DataT, VecSize>& v)
    {
        return reinterpret_cast<DataT*>(&v.data)[Idx];
    }

    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr inline DataT get(HIP_vector_type<DataT, VecSize> const& v)
    {
        return v.data[Idx];
    }

    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr inline DataT get(HIP_vector_type<DataT, VecSize>&& v)
    {
        return v.data[Idx];
    }

    // non_native_vector_base extensions
    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr static inline DataT&
        get(non_native_vector_base<DataT, VecSize>& v)
    {
        return v[Idx];
    }

    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr static inline DataT
        get(non_native_vector_base<DataT, VecSize> const& v)
    {
        return v[Idx];
    }

    template <uint32_t Idx, typename DataT, uint32_t VecSize>
    ROCWMMA_HOST_DEVICE constexpr static inline DataT
        get(non_native_vector_base<DataT, VecSize>&& v)
    {
        return v[Idx];
    }
} // namespace rocwmma

#endif // ROCWMMA_GET_HPP
