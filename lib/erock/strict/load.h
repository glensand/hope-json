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

#include <string_view>
#include "rapidjson/document.h"
#include "hope/tuple/tuple_from_struct.h"
#include "erock/strict/types.h"

namespace erock::detail {

    void extract(rapidjson::Value& doc, bool_t& val){
        val = doc[val.name.data()].GetBool();
    }

    void extract(rapidjson::Value& doc, int_t& val){
        val = doc[val.name.data()].GetInt();
    }

    void extract(rapidjson::Value& doc, real_t& val){
        val = doc[val.name.data()].GetDouble();
    }

    void extract(rapidjson::Value& doc, string_t& val){
        val = doc[val.name.data()].GetString();
    }

    template<typename TValue>
    void extract(rapidjson::Value& doc, array_t<TValue>& values){
        auto&& json_array = doc[values.name.data()];
        for(auto&& it : json_array.GetArray()){
            TValue cur_value;
            load(it, cur_value);
            values.emplace_back(std::move(cur_value));
        }
    }
    
    template<typename TValue>
    void load(rapidjson::Value& doc, object<TValue>& val){
        if constexpr(is_inbuilt_v<TValue>){
            extract(doc, val);
        } else if constexpr(!is_inbuilt_v<TValue>) { // for msvc
            auto&& obj = doc[val.name.data()];
            auto&& tuple = hope::tuple_from_struct(val.value, 
                hope::field_policy::reference{});
            tuple.for_each([&](auto&& field){
                load(obj, field);
            });
        }
    }

}

/*! @} */
