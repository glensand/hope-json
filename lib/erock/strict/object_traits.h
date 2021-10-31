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
    \brief File contains definitions of the strict json object
*/

#pragma once

#include "erock/strict/object.h"

namespace erock  {

    template<typename TValue>
    constexpr auto is_object(TValue) { return false; }

    template<typename TValue>
    constexpr auto is_object(object<TValue>) { return true; }

    /**
     * Checks if the specified type is object in terms of the erock library. 
     * NOTE: Only erock objects might be processed by load/store operations 
     * @param TValue the type to be checked
     */
    template<typename TValue>
    constexpr bool is_object_v = is_object(TValue{});

}

/*! @} */
