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
    \brief File contains definitions of the strict json object
*/

#pragma once

#include <string_view>
#include "rapidjson/document.h"
#include "hope/tuple/tuple_from_struct.h"

namespace erock::detail {

    /**
     * \brief Opens or creates file with given name (to owerwrite) then stores all contant to these file.
     */ 
    template<typename TValue>
    void store(const TValue& value, std::string_view file_name) {

    }
}

/*! @} */
