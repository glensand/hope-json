/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
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

namespace erock {

    template<typename TValue>
    constexpr bool is_vector(std::vector<TValue>){ return true; }

    template<typename TValue>
    constexpr bool is_vector(TValue){ return false; }

    template<typename TValue, typename TClearValue = std::decay_t<TValue>>
    constexpr bool is_inbuilt_v = 
        std::is_same_v<TClearValue, bool> || 
        std::is_same_v<TClearValue, int64_t> ||
        std::is_same_v<TClearValue, std::string> ||
        std::is_same_v<TClearValue, long double> ||
        is_vector(TClearValue{});

}

/*! @} */
