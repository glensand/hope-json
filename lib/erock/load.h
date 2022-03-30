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

#include "erock/operation.h"
#include "erock/object_traits.h"
#include "erock/error_info.h"
#include "erock/strict/types.h"
#include "erock/strict/document_policy.h"

namespace erock {

    struct load_policy final {
        struct present final { };
        struct value final { };
    };
        
    template<typename TClass, typename... Ts>
    class load_op final : public operation<Ts...>{
    public:
        static_assert(!is_object_v<TClass>, 
            "EROCK-JSON::load: erock::object type should not be used as wrapper as json document\n"
            "Remove the wrapper or pass value field and try compile again"
        );

        load_op(TClass& val, hope::type_map<Ts...>) 
            : m_value(val){

        }

        void execute(rapidjson::Document& json) {
            extract(json, m_value);
        }

    private:
        using super = operation<Ts...>;
        using policy = load_policy;

        constexpr auto getter_map() const {
            return hope::type_value_map(
                hope::tv<raw_string_t>(&rapidjson::Value::GetString),
                hope::tv<raw_int_t>(&rapidjson::Value::GetInt),
                hope::tv<raw_bool_t>(&rapidjson::Value::GetBool),
                hope::tv<raw_real_t>(&rapidjson::Value::GetDouble)
            );
        }

        template<typename TValue>
        void extract(rapidjson::Value& json, TValue& val){
            using value_policy_t = typename super::template get_t<policy::value>;
            using raw_value_t = typename value_policy_t::template raw_value_t<TValue>;
            if constexpr (is_inbuilt_v<raw_value_t>) {
                auto&& map = getter_map();
                auto&& method = map.template get<raw_value_t>();
                auto&& inner_value = value_policy_t::value(val);
                inner_value = (json.*method)();
            } else if constexpr (!is_inbuilt_v<raw_value_t>) {
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
                    using present_t = typename super::template get_t<policy::present>;
                    if(present_t::template is<value_t>(json_value, field.name)) {
                        extract(json_value, field.value);
                    }
                });
            }
        }

        template<typename TValue> 
        void extract(rapidjson::Value& json, raw_array_t<TValue>& values) {
            for(auto&& it : json.GetArray()) {
                TValue cur_value;
                using value_t = typename super::template get_t<policy::value>::template raw_value_t<TValue>;
                assert_type_valid<value_t>(it, "An element of the Array");
                extract(it, cur_value);
                values.emplace_back(std::move(cur_value));
            }
        }

        TClass& m_value;
    };

    template<typename TClass, typename... Ts>
    load_op(TClass, hope::type_map<Ts...>)->load_op<TClass, Ts...>;

}

/*! @} */
