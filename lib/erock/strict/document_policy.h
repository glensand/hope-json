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
    \brief File contains utility functions for load operations
*/

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "hope/components/user_defined_types.h"

#include "erock/error_info.h"

#include <stdexcept>
#include <string>

namespace erock::strict {

    /**
     * \brief Policy is used to check if a value exists
     * 
     * */
    struct presenter final {
        template<typename TExpected>
        static bool is(rapidjson::Value& value, std::string_view name) { 
            assert_object_not_null(value, name);
            assert_type_valid<TExpected>(value, name);
            return true; 
        }
    };

    /**
     * \brief Policy is used to extract value type from wrapper (probably) value
     * 
     * */
    struct value_trait final {
        template<typename TValue>
        using raw_value_t = std::decay_t<TValue>;

        template<typename TValue>
        static decltype(auto) value(TValue&& v){
            return v;
        }
    };
}

/*! @} */
