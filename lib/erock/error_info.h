/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <core> Core
    @{
    \file
    \brief File contains utility functions for load operations
*/

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <stdexcept>
#include <string>

namespace erock::detail {

    inline 
    void assert_load_valid(const rapidjson::ParseResult& ok) {
        if (!ok) {
            throw std::runtime_error(
                std::string(GetParseError_En(ok.Code())) +
                "\nOffset: " + std::to_string(ok.Offset())
            );
        }
    }

}

/*! @} */
