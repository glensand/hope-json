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

#include "erock/strict/field.h"
#include <cmath>
#include <string>
#include <vector>

namespace erock {

    /**
     * Integer type, 64 bit are used to simplify usage in signed-unsigned context
     */
    using e_int_t = field<int64_t>;

    /**
     * Floating point type, long double are used so that everyone has enough :)
     */
    using e_float_t = field<long double>;

    /**
     * Boolean type, should be used as is...
     */
    using e_bool_t = field<bool>;

    /**
     */
    using e_string_t = field<std::string>;

    /**
     */
    template<typename TValue>
    using e_array_t = field<std::vector<TValue>>;
}

/*! @} */
