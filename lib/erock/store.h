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

#include "erock/json_parser.h"

#include "hope_core/type_traits/type_value_map.h"
#include "hope_core/tuple/tuple_from_struct.h"

#include "erock/types.h"

namespace erock {

    template<typename TClass>
    class store_op final {
    public:
        store_op(const TClass& value) 
            : m_object(value) {

        }

        void execute(hope::json::IJsonDocument& document) {
            store_impl(m_object, document);
        }

    private:
        constexpr auto setter_map() const {
            return hope::type_value_map(
                    hope::tv<raw_int_t>(&hope::json::IJsonValue::SetInt),
                    hope::tv<raw_bool_t>(&hope::json::IJsonValue::SetBool),
                    hope::tv<raw_real_t>(&hope::json::IJsonValue::SetDouble)
            );
        }

        template<typename TObject>
        void store_impl(const raw_array_t<TObject>& value, hope::json::IJsonValue& object) {
            object.SetArray();
            for(auto&& obj : value) {
                hope::json::Value element;
                store_impl(obj, element);
                object.PushBack(element.Move(), nullptr); 
            }
        }

        template<typename TObject>
        void store_impl(const TObject& value, hope::json::IJsonValue& json_val) {
            if constexpr (is_inbuild_v<TObject>) {
                using raw_value_t = typename value_trait<TObject>::value_t;
                if constexpr (std::is_same_v<raw_value_t, raw_string_t>) {
                    json_val.SetString(get(value).c_str(), get(value).size());
                } else if constexpr(!std::is_same_v<raw_value_t, raw_string_t>) { // msvc needed special care
                    auto&& method = setter_map().template get<raw_value_t>();
                    (json_val.*method)(get(value));
                }
            } else if constexpr (!is_inbuild_v<TObject>) {
                json_val.SetObject();
                auto&& tuple = hope::tuple_from_struct(value, hope::field_policy::reference{});
                tuple.for_each([&](auto&& child){
                    if(!has(child)) return;
                    using child_t = std::decay_t<decltype(child)>;
                    static_assert(is_object_v<child_t>, 
                        "EROCK-JSON::store: All the types of the user defined structure"
                        "must be wrapped with erock::object structure.\n"
                        "It is required 'cause this is only one way how to tell the library what name the object has to has"
                    );
                    hope::json::Value json_child;
                    store_impl(child.value, json_child);
                    json_val.AddMember(child.name.data(), json_child.Move(), nullptr);
                });
            }
        }

        const TClass& m_object;
    };

    template<typename TClass>
    store_op(TClass)->store_op<TClass>;

}

/*! @} */
