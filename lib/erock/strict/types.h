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

#include "erock/strict/object.h"
#include <cmath>
#include <string>
#include <vector>
#include <type_traits>

namespace erock {

    /**
     * Integer type, 64 bit are used to simplify usage in signed-unsigned context
     */
    using int_t = object<int64_t>;

    /**
     * Floating point type, long double are used so that everyone has enough :)
     */
    using real_t = object<long double>;

    /**
     * Boolean type, should be used as is...
     */
    using bool_t = object<bool>;

    /**
     */
    using string_t = object<std::string>;

    /**
     */
    template<typename TValue>
    using array_t = object<std::vector<TValue>>;

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
