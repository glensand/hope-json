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

#include <string>
#include <vector>

namespace erock {

    /**
     */
    using e_string_t = std::string;

    /**
     */
    using e_real_t = double;

    /**
     */
    using e_int_t = int;

    /**
     */
    using e_bool_t = bool;

    /**
     */
    template<typename TValue>
    using e_array_t = std::vector<TValue>;

    /**
     */
    struct erock_tag { };
    
}

/*! @} */
