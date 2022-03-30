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
    \brief File contains definitions of the strict json object
*/

#pragma once

#include <string_view>
#include <type_traits>

#include "rapidjson/document.h"

#include "erock/operation.h"
#include "hope/typelist/type_value_map.h"
#include "hope/tuple/tuple_from_struct.h"

namespace erock {

    struct store_policy final {
        struct value final { };
    };

    template<typename TClass, typename... Ts>
    class store_op final : public operation<Ts...>{
    public:
        store_op(const TClass& value, hope::type_map<Ts...>) 
            : m_value(value) {

        }

        void execute(rapidjson::Document& document) {
            m_allocator = &document.GetAllocator();
            store_impl(m_value, document);
        }

    private:
        using super = operation<Ts...>;
        using policy = store_policy;

        constexpr auto setter_map() const {
            return hope::type_value_map(
                    hope::tv<raw_int_t>(&rapidjson::Value::SetInt),
                    hope::tv<raw_bool_t>(&rapidjson::Value::SetBool),
                    hope::tv<raw_real_t>(&rapidjson::Value::SetDouble)
            );
        }

        template<typename TValue>
        void store_impl(const raw_array_t<TValue>& value, rapidjson::Value& object) {
            object.SetArray();
            for(auto&& obj : value) {
                rapidjson::Value element;
                store_impl(obj, element);
                object.PushBack(element.Move(), *m_allocator); 
            }
        }

        template<typename TValue>
        void store_impl(const TValue& value, rapidjson::Value& json) {
            using value_policy_t = typename super::template get_t<policy::value>;
            using raw_value_t = typename value_policy_t::template raw_value_t<TValue>;
            if constexpr (is_inbuilt_v<raw_value_t>) {
                auto&& inner_value = value_policy_t::value(value);
                if constexpr (std::is_same_v<raw_value_t, raw_string_t>) {
                    json.SetString(inner_value.c_str(), inner_value.size());
                } else if constexpr(!std::is_same_v<TValue, raw_string_t>) { // msvc needed special care
                    auto&& method = setter_map().template get<TValue>();
                    (json.*method)(inner_value);
                }
            } else if constexpr (!is_inbuilt_v<raw_value_t>) {
                json.SetObject();
                auto&& tuple = hope::tuple_from_struct(value, hope::field_policy::reference{});
                tuple.for_each([&](auto&& child){
                    using child_t = std::decay_t<decltype(child)>;
                    static_assert(is_object_v<child_t>, 
                        "EROCK-JSON::store: All the types of the user defined structure"
                        "must be wrapped with erock::object structure.\n"
                        "It is required 'cause this is only one way how to tell the library what name the object has to has"
                    );
                    rapidjson::Value json_child;
                    store_impl(child.value, json_child);
                    rapidjson::GenericStringRef<char> rapid_name{ child.name.data() };
                    json.AddMember(rapid_name, json_child.Move(), *m_allocator);
                });
            }
        }

        rapidjson::Document::AllocatorType* m_allocator{ nullptr };
        const TClass& m_value;
    };

    template<typename TClass, typename... Ts>
    store_op(TClass, hope::type_map<Ts...>)->store_op<TClass, Ts...>;

}

/*! @} */
