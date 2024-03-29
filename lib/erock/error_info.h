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

#include "hope/components/user_defined_types.h"

#include "erock/object_traits.h"

#include <stdexcept>
#include <string>

namespace erock {

    inline 
    void assert_load_valid(const rapidjson::ParseResult& ok) {
        if (!ok) {
            throw std::runtime_error(
                std::string(GetParseError_En(ok.Code())) +
                "\nOffset: " + std::to_string(ok.Offset())
            );
        }
    }

    inline
    std::string extract_exception_with_name(std::string_view name){
        return std::string("erock::extract, the object with name [") + 
                name.data() + "] ";
    }

    inline
    void assert_object_not_null(rapidjson::Value& obj, std::string_view name){
        if(obj.IsNull()) {
            throw std::runtime_error(
                extract_exception_with_name(name) + "is not exists in the given json string."
            );
        }
    }

    template<typename TValue>
    void assert_type_valid(rapidjson::Value& obj, std::string_view name) {
        // assert the object is object or array(in terms of json)
        auto&& validate_obj = [&](auto method, const auto* type){
            if(!(obj.*method)()){
                throw std::runtime_error(
                    extract_exception_with_name(name) + 
                    "has not appropriate type, expected type is [" + type + "]"
                );
            }
        }; // array and object needs special care
        if constexpr (hope::is_vector_v<TValue>){
            validate_obj(&rapidjson::Value::IsArray, "Array");
        }
        if constexpr (is_object_v<TValue>) {
            validate_obj(&rapidjson::Value::IsObject, "Object");
        }
        if constexpr (is_inbuild_v<TValue>){
            const hope::type_value_map map(
                hope::tv<raw_string_t>(std::pair(&rapidjson::Value::IsString, "String")),
                hope::tv<raw_int_t>(std::pair(&rapidjson::Value::IsInt, "Int")),
                hope::tv<raw_bool_t>(std::pair(&rapidjson::Value::IsBool, "Bool")),
                hope::tv<raw_real_t>(std::pair(&rapidjson::Value::IsDouble, "Real"))
            );
            validate_obj(map.template get<TValue>().first, map.template get<TValue>().second);
        }
    }
}

/*! @} */
