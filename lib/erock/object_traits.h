/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <core> Core
    @{
    \file
    \brief File contains traits for JSON object types
*/

#pragma once

#include <type_traits>
#include "erock/object.h"

namespace erock  {

    template <typename T>
    struct is_object final : std::false_type {};

    template <class TValue>
    struct is_object<base_object<TValue>> final : std::true_type {};

    /**
     * Checks if the specified type is object in terms of the erock library. 
     * NOTE: Only erock objects might be processed by load/store operations 
     * @param TValue the type to be checked
     */
    template<typename TValue>
    constexpr bool is_object_v = is_object<TValue>::value;
    
}

/*! @} */
