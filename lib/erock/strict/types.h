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

#include "erock/strict/object.h"
#include "erock/types.h"
#include <cmath>
#include <string>
#include <vector>
#include <type_traits>

namespace erock::strict {

    /**
     * Named integer, this type has to be used to load/store integer from json
     */
    using int_t = object<raw_int_t>;

    /**
     * Named floating point value, this type has to be used to load/store floating point value from json
     */
    using real_t = object<raw_real_t>;

    /**
     * Named boolean value, this type has to be used to load/store boolean value from json
     */
    using bool_t = object<raw_bool_t>;

    /**
     * Named string object, this type has to be used to load/store string value from json 
     */
    using string_t = object<raw_string_t>;

    /** The only one templated type, represents array(in terms as json); usage is obvious
     */
    template<typename TValue>
    using array_t = object<raw_array_t<TValue>>;

}

/*! @} */
