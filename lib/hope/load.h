/* Copyright (C) 2021 - 2026 Gleb Bezborodov - All Rights Reserved
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
#include <type_traits>

#include "hope/json_parser.h"

#include "hope_core/type_traits/type_value_map.h"
#include "hope_core/tuple/tuple_from_struct.h"

#include "hope/types.h"
#include "hope/object_traits.h"
#include "hope/policy.h"

namespace hope {

    template<typename TClass>
    class load_op final {
    public:
        static_assert(!is_object_v<TClass>,
            "HOPE-JSON::load: hope::object type should not be used as wrapper as json document\n"
            "Remove the wrapper or pass value field and try compile again"
        );

        load_op(TClass& val)
            : m_object(val){

        }

        void execute(hope::json::Document& json) {
            extract(json, m_object);
        }

    private:

        constexpr auto getter_map() const {
            return hope::type_value_map(
                hope::tv<raw_string_t>(&hope::json::Value::GetString),
                hope::tv<raw_int_t>(&hope::json::Value::GetInt),
                hope::tv<raw_bool_t>(&hope::json::Value::GetBool),
                hope::tv<raw_real_t>(&hope::json::Value::GetDouble)
            );
        }

        template<typename TObject>
        void extract(hope::json::Value& json_val, TObject& val){
            using raw_value_t = typename value_trait<TObject>::value_t;
            if constexpr (is_inbuild_v<raw_value_t>) {
                auto&& map = getter_map();
                auto&& method = map.template get<raw_value_t>();
                val = (json_val.*method)();
            } else if constexpr (!is_inbuild_v<raw_value_t>) {
                auto&& tuple = hope::tuple_from_struct(val, hope::field_policy::reference{});
                tuple.for_each([&](auto&& field){
                    using field_t = std::decay_t<decltype(field)>;
                    using value_t = std::decay_t<decltype(field.value)>;
                    static_assert(is_object_v<field_t>,
                        "HOPE-JSON::extract: All the types of the user defined structure"
                        "must be wrapped with hope::object structure.\n"
                        "It is required 'cause this is only one way how to tell the library what name the object has to has"
                    );

                    auto&& json_value = json_val[field.name.data()];
                    present_policy<field_t> p{};
                    if(p.template is<value_t>(json_value, field.name)) {
                        extract(json_value, field.value);
                    }
                });
            }
        }

        template<typename TObject>
        void extract(hope::json::Value& json_val, raw_array_t<TObject>& values) {
            for(size_t i = 0; i < json_val.Size(); ++i) {
                TObject cur_object;
                auto& element = *json_val.GetArray()[i];
                extract(element, cur_object);
                values.emplace_back(std::move(cur_object));
            }
        }

        TClass& m_object;
    };

    template<typename TClass>
    load_op(TClass)->load_op<TClass>;

}

/*! @} */
