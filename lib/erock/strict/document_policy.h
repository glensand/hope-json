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
     * \brief Value set policy, is used for transparent value providing
     * 
     * */
    struct setter final {
        template<typename TReceiver>
        void operator()(TReceiver&& receiver, rapidjson::Value& json) const {
                    // prepare specific extractors, to avoid boilerplate
            hope::type_value_map map(
                hope::tv<raw_string_t>(&rapidjson::Value::GetString),
                hope::tv<raw_int_t>(&rapidjson::Value::GetInt),
                hope::tv<raw_bool_t>(&rapidjson::Value::GetBool),
                hope::tv<raw_real_t>(&rapidjson::Value::GetDouble)
            );
            auto&& method = map.template get<std::decay_t<TReceiver>>();
            receiver = (json.*method)(); 
        }
    };

    /**
     * \brief Policy is used to check if a value exists
     * 
     * */
    struct presenter final {
        template<typename TExpected>
        bool operator()(rapidjson::Value& value, std::string_view name) const { 
            assert_object_not_null(value, name);
            assert_type_valid<TExpected>(value, name);
            return true; 
        }
    };
    
    /**
     * \brief Policy is used to check if a value is trivial
     * 
     * */
    struct inbuild_checker final {
        template<typename TValue>
        constexpr bool operator()() const { 
            return is_inbuilt_v<TValue>;
        }
    };

    /**
     * \brief Policy is used to extract value type from wrapper (probably) value
     * 
     * */
    struct value_trait final {
        template<typename TValue>
        using raw_value_t = TValue;
    };
}

/*! @} */
