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
    \brief File contains implementations of the json loading functions. These functions should not be used directly;
*/

#pragma once

#include <string_view>
#include "hope/tuple/tuple_from_struct.h"
#include "hope/typelist/type_value_map.h"
#include "erock/strict/types.h"
#include "erock/strict/type_traits.h"
#include "erock/strict/object_traits.h"
#include "erock/strict/load_detail.h"

namespace erock::detail {

    template<typename TValue>
    std::enable_if_t<is_inbuilt_v<TValue>> 
    extract(rapidjson::Value& doc, TValue& val) {
        hope::type_value_map map(
            hope::tv<raw_string_t>(&rapidjson::Value::GetString),
            hope::tv<raw_int_t>(&rapidjson::Value::GetInt),
            hope::tv<raw_bool_t>(&rapidjson::Value::GetBool),
            hope::tv<raw_real_t>(&rapidjson::Value::GetDouble)
        );
        auto&& method = map.template get<TValue>();
        val = (doc.*method)();
    }

    template<typename TValue> // predefined 'cause it is used in the method below
    void extract(rapidjson::Value& doc, raw_array_t<TValue>& values);

    template<typename TValue>
    std::enable_if_t<!is_inbuilt_v<TValue>>
    extract(rapidjson::Value& doc, TValue& val){
        auto&& tuple = hope::tuple_from_struct(val, hope::field_policy::reference{});
        tuple.for_each([&](auto&& field){
            using field_t = std::decay_t<decltype(field)>;
            using value_t = std::decay_t<decltype(field.value)>;
            static_assert(is_object_v<field_t>, 
                "EROCK-JSON::extract: All the types of the user defined structure"
                "must be wrapped with erock::object structure.\n"
                "It is required 'cause this is only one way how to tell the library what name the object has to has"
            );
            auto&& json_value = doc[field.name.data()];
            assert_object_not_null(json_value, field.name);
            assert_type_valid<value_t>(json_value, field.name);
            extract(json_value, field.value);
        });
    }

    template<typename TValue>
    void extract(rapidjson::Value& doc, raw_array_t<TValue>& values){
        for(auto&& it : doc.GetArray()) {
            auto&& obj = it;
            TValue cur_value;
            assert_type_valid<TValue>(it, "An element of the Array");
            extract(it, cur_value);
            values.emplace_back(std::move(cur_value));
        }
    } 

    /**
     * @brief Tries to store all the parsed values from json DOM three to the given structure
     */ 
    template<typename TValue>
    void load(rapidjson::Document& doc, TValue& val){
        static_assert(!is_object_v<TValue>, 
            "EROCK-JSON::load: erock::object type should not be used as wrapper as json document\n"
            "Remove the wrapper or pass value field and try compile again"
        );
        extract(doc, val);
    }

}

/*! @} */
