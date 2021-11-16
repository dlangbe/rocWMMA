/*******************************************************************************
 *
 * MIT License
 *
 * Copyright 2021 Advanced Micro Devices, Inc.
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

#ifndef WMMA_DEVICE_LOAD_CONTAMINATION_H
#define WMMA_DEVICE_LOAD_CONTAMINATION_H

#include "MappingUtil.h"
#include "WMMA.h"

template <uint32_t BlockM, uint32_t BlockN, typename DataT, typename Layout>
__global__ void loadContamination(
    uint32_t m, uint32_t n, DataT const* in, DataT* out, uint32_t ld, DataT param1, DataT param2)
{
    using Mapping = MappingUtil<BlockM, BlockN, DataT, Layout>;

    int paddedld = std::is_same<Layout, row_major>::value ? param2 : param1;

    // Create frags and fill
    auto frag = wmma::fragment<accumulator, BlockM, BlockN, 1, DataT, Layout>();

    // Map, load and store.
    auto* read  = Mapping::dataCoord(in, ld);
    auto* write = Mapping::dataCoord(out, ld);
    wmma::load_matrix_sync(frag, read, ld);
    wmma::store_matrix_sync(write, frag, ld);
}

#endif // WMMA_DEVICE_LOAD_CONTAMINATION_H
