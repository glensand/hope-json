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
    \brief File contains load/store functions
*/

#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "erock/strict/load.h"
#include "erock/strict/store.h"
#include "erock/error_info.h"

#include <stdexcept>
#include <string_view>

namespace erock::strict {

    /**
     * \brief Value set policy, is used for transparent value providing
     * 
     * */
    struct setter final {
        template<typename TReceiver>
        void operator()(TReceiver&& receiver, rapidjson::Value& json) const {
                    // prepare specific extractors, to avoid boilerplate
            hope::type_value_map map(
                hope::tv<raw_string_t>(&rapidjson::Value::GetString),
                hope::tv<raw_int_t>(&rapidjson::Value::GetInt),
                hope::tv<raw_bool_t>(&rapidjson::Value::GetBool),
                hope::tv<raw_real_t>(&rapidjson::Value::GetDouble)
            );
            auto&& method = map.template get<std::decay_t<TReceiver>>();
            receiver = (json.*method)(); 
        }
    };

    /**
     * \brief Policy is used to check if a value exists
     * 
     * */
    struct presenter final {
        template<typename TExpected>
        bool operator()(rapidjson::Value& value, std::string_view name) const { 
            detail::assert_object_not_null(value, name);
            detail::assert_type_valid<TExpected>(value, name);
            return true; 
        }
    };
    
    /**
     * \brief Policy is used to check if a value is trivial
     * 
     * */
    struct inbuild_checker final {
        template<typename TValue>
        constexpr bool operator()() const { 
            return is_inbuilt_v<TValue>;
        }
    };

    /**
     * \brief Tries to parse JSON string, if succeeded then stores all the loaded values to the related 
     * fields of the structure. The type of the structure is required, obviously; 
     * 
     * @param json String with json to be parsed;
     * @param TValue - Type of the structure which fields matches the objects from JSON file. Each field ot the TValue 
     * have to be an instance of the erock::field structure with proper name. All these fields are required in the file, 
     * if the attribute of the object is not present in the file, the default value will be stored to the desired position.
     * 
     * \throws std::runtime_error with description of the occurred error (Invalid value, not all braces are placed right e.t.c)
     */ 
    template<typename TValue>
    auto load(std::string_view json) {
        rapidjson::Document doc;
        erock::detail::assert_load_valid(doc.Parse(json.data()));
        TValue object;
        using policy_map_t = hope::type_map<
            hope::type_pair<detail::set_policy, setter>,
            hope::type_pair<detail::present_policy, presenter>,
            hope::type_pair<detail::inbuild_policy, inbuild_checker>
        >;
        detail::load(doc, object, policy_map_t{});
        return object;
    }

    /**
     * \brief Converts given structure to the JSON DOM tree, then stores it to the string
     * @param value Object to be stored to the json
     * 
     * @return valid JSON string (result string is the reflection of the given object)
     * \throws std::runctime error if the object is not valid
     */ 
    template<typename TValue>
    auto store(const TValue& value) {

        static_assert(!is_inbuilt_v<TValue>, 
            "---- EROCK ASSERTION FAILURE ----; An attempt was made to store object of inappropriate type"
        );

        rapidjson::Document doc;
        detail::store(value, doc);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return std::string { buffer.GetString() };
    }
}

/*! @} */
