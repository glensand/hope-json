/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <strict> Strict
    @{
    \file
    \brief File contains definitions of the strict json types

    Strict mean the order of the filds cannot be changed, and each subobject of the json object cannot be null.
    This rule helps library to convert text json to their structure representation
*/

#pragma once

#include "erock/strict/types.h"

namespace erock::strict {

    template<typename TValue>
    constexpr bool is_inbuilt_v = hope::contains<TValue>(registered_raw_types_t{});

}

/*! @} */
