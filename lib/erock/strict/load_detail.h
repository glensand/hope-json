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
    \brief File contains utility functions for load operations
*/

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "hope/components/typemap.h"
#include <stdexcept>
#include <string>

namespace erock::detail {

    inline
    std::string extract_exception_with_name(std::string_view name){
        return std::string("erock::extract, the object with name [") + 
                name.data() + "] ";
    }

    inline
    void assert_object_not_null(rapidjson::Value& obj, std::string_view name){
        if(obj.IsNull()){
            throw std::runtime_error(
                extract_exception_with_name(name) + "is not exists in the given json string."
            );
        }
    }

    template<typename TValue>
    void assert_type_valid(rapidjson::Value& obj, std::string_view name) {
        // assert the object is object or array(in terms of json)
        if constexpr (hope::is_vector_v<TValue>){
            if(!obj.IsArray()){
                throw std::runtime_error(
                    extract_exception_with_name(name) + 
                    "has not appropriate type, expected type is Array"
                );
            }
        }
        if constexpr (is_object_v<TValue>) {
            if(!obj.IsObject()){
                throw std::runtime_error(
                    extract_exception_with_name(name) + 
                    "has not appropriate type, expected type is Object"
                );
            }
        }
    }

    template<>
    void assert_type_valid<raw_bool_t>(rapidjson::Value& obj, std::string_view name){
        if(!obj.IsBool()){
            throw std::runtime_error(
                extract_exception_with_name(name) +
                "has no appropriate type, expected type is Bool"
            );
        }
    }

    template<>
    void assert_type_valid<raw_int_t>(rapidjson::Value& obj, std::string_view name){

    }

}

/*! @} */
