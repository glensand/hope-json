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
    \brief File contains implementations of the json loading functions. These functions should not be used directly;
*/

#pragma once

#include <string_view>
#include "rapidjson/document.h"
#include "hope/tuple/tuple_from_struct.h"
#include "erock/strict/types.h"
#include "erock/strict/type_traits.h"
#include "erock/strict/object_traits.h"

namespace erock::detail {

    /**
     *  Declaration section
     */
    template<typename TValue>
    void load(rapidjson::Value& doc, object<TValue>& val); // declared due to call in extract function

    template<typename TValue>
    void load_object(rapidjson::Value& doc, TValue& val); // declared due to call in load function

    /**
     * Extract functions section
     */
    inline 
    void extract(rapidjson::Value& doc, bool_t& val){
        val = doc[val.name.data()].GetBool();
    }

    inline
    void extract(rapidjson::Value& doc, int_t& val){
        val = doc[val.name.data()].GetInt();
    }

    inline
    void extract(rapidjson::Value& doc, real_t& val){
        val = doc[val.name.data()].GetDouble();
    }

    inline
    void extract(rapidjson::Value& doc, string_t& val){
        val = doc[val.name.data()].GetString();
    }

    template<typename TValue>
    void extract(rapidjson::Value& doc, array_t<TValue>& values){
        auto&& json_array = doc[values.name.data()];
        for(auto&& it : json_array.GetArray()) {
            auto&& obj = it;
            TValue cur_value;
            load_object(it, cur_value);
            values.value.emplace_back(std::move(cur_value));
        }
    } 

    template<typename TValue>
    void load(rapidjson::Value& doc, object<TValue>& val){
        if constexpr(is_inbuilt_v<TValue>) {
            extract(doc, val);
        } else if constexpr(!is_inbuilt_v<TValue>) { // for msvc
            auto&& obj = doc[val.name.data()];
            load_object(obj, val.value);
        }
    }

    template<typename TValue>
    void load_object(rapidjson::Value& doc, TValue& val){
        auto&& tuple = hope::tuple_from_struct(val, hope::field_policy::reference{});
        tuple.for_each([&](auto&& field){
            using field_t = std::decay_t<decltype(field)>;
            static_assert(is_object_v<field_t>, 
                "EROCK-JSON::load_object: All the types of the user defined structure"
                "must be wrapped with erock::object structure.\n"
                "It is required 'cause this is only one way how to tell the library what name the object has to has"
            );
            load(doc, field);
        });
    }

    template<typename TValue>
    void load(rapidjson::Document& doc, TValue& val){
        static_assert(!is_object_v<TValue>, 
            "EROCK-JSON::load: erock::object type should not be used as wrapper as json document\n"
            "Remove the wrapper or pass value field and try compile again"
        );
        load_object(doc, val);
    }

}

/*! @} */
