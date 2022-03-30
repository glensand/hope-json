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

#include "erock/load.h"
#include "erock/store.h"
#include "erock/error_info.h"

#include <stdexcept>
#include <string_view>

namespace erock::strict {

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
        assert_load_valid(doc.Parse(json.data()));
        TValue object;
        load_op(
            object,
            hope::type_map<
                hope::type_pair<load_policy::present, presenter>,
                hope::type_pair<load_policy::value, value_trait>
            >{}
        )
        .execute(doc);

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
        store_op(
            value,
            hope::type_map<
                hope::type_pair<store_policy::value, value_trait>
            >{}
        )
        .execute(doc);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return std::string { buffer.GetString() };
    }
}

/*! @} */
