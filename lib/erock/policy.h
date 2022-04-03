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

#include "rapidjson/error/en.h"
#include "erock/error_info.h"

#include <stdexcept>
#include <string>

namespace erock {

    /**
     * \brief Policy is used to check if a value exists
     * 
     * */
    template<typename TValue>
    struct present_policy;
    
    template<typename TValue>
    struct present_policy<strict_object<TValue>> {
        template<typename TExpected>
        static bool is(rapidjson::Value& value, std::string_view name) { 
            assert_object_not_null(value, name);
            assert_type_valid<TExpected>(value, name);
            return true; 
        }
    };

    template<typename TValue>
    struct present_policy<nullable_object<TValue>> {
        template<typename TExpected>
        static bool is(rapidjson::Value& value, std::string_view name) { 
            return !value.IsNull();
        }
    };

    template<typename TValue>
    struct value_trait {
        using value_t = TValue;
    };

    template<typename TValue>
    struct value_trait<optional<TValue>> {
        using value_t = TValue;
    };

    template<typename TValue>
    bool has(const strict_object<TValue>&){
        return true;
    }

    template<typename TValue>
    bool has(const nullable_object<TValue>& obj){
        return obj.value.has;
    }

    template<typename TValue>
    const TValue& get(const TValue& obj){
        return obj;
    }

    template<typename TValue>
    const TValue& get(const optional<TValue>& obj){
        return obj;
    }

}

/*! @} */
