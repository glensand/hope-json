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
#include "hope/components/typemap.h"

#include "erock/object_traits.h"
#include "erock/strict/types.h"
#include "erock/strict/type_traits.h"
#include "erock/strict/load_detail.h"

namespace erock::strict::detail {

    struct set_policy final{};
    struct present_policy final{};
    struct inbuild_policy final{};

    template<typename Policy, typename... Ts>
    using get_t = typename hope::type_map<Ts...>::template get_t<Policy>;

    template<typename TValue, typename... Ts>
    void extract(rapidjson::Value& json, TValue& val, hope::type_map<Ts...>){
        using inbuild_checker_t = get_t<inbuild_policy, Ts...>;
        if constexpr (inbuild_checker_t{}.template operator()<TValue>()) {
            // create setter using given set policy
            using setter_t = get_t<set_policy, Ts...>;
            setter_t{}(val, json);
        } else if constexpr (!inbuild_checker_t{}.template operator()<TValue>()) {
            auto&& tuple = hope::tuple_from_struct(val, hope::field_policy::reference{});
            tuple.for_each([&](auto&& field){
                using field_t = std::decay_t<decltype(field)>;
                using value_t = std::decay_t<decltype(field.value)>;
                static_assert(is_object_v<field_t>, 
                    "EROCK-JSON::extract: All the types of the user defined structure"
                    "must be wrapped with erock::object structure.\n"
                    "It is required 'cause this is only one way how to tell the library what name the object has to has"
                );

                auto&& json_value = json[field.name.data()];
                if(get_t<present_policy, Ts...>().template operator()<value_t>(json_value, field.name)) {
                    extract(json_value, field.value, hope::type_map<Ts...>{});
                }
            });
        }
    }

    template<typename TValue, typename... Ts> 
    void extract(rapidjson::Value& doc, raw_array_t<TValue>& values, hope::type_map<Ts...>){
        for(auto&& it : doc.GetArray()) {
            auto&& obj = it;
            TValue cur_value;
            assert_type_valid<TValue>(it, "An element of the Array");
            extract(it, cur_value, hope::type_map<Ts...>{});
            values.emplace_back(std::move(cur_value));
        }
    } 

    /**
     * @brief Tries to store all the parsed values from json DOM three to the given structure
     */ 
    template<typename TValue, typename... Ts>
    void load(rapidjson::Document& doc, TValue& val, hope::type_map<Ts...>){
        static_assert(!is_object_v<TValue>, 
            "EROCK-JSON::load: erock::object type should not be used as wrapper as json document\n"
            "Remove the wrapper or pass value field and try compile again"
        );
        extract(doc, val, hope::type_map<Ts...>{});
    }

}

/*! @} */
