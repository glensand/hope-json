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
    \brief File contains load/store functions
*/

#pragma once

#include "erock/strict/load.h"
#include "erock/strict/store.h"
#include "erock/strict/document_detail.h"

#include <stdexcept>
#include <string_view>

namespace erock {

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
        detail::assert_load_valid(doc.Parse(json.data()));
        TValue object;
        detail::load(doc, object);
        return object;
    }

    /**
     * \brief Converts given structure to the JSON DOM tree, then stores it to the string
     */ 
    template<typename TValue>
    auto store(const TValue& value) {
        rapidjson::Document doc;
    }
}

/*! @} */
