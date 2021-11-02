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
     * \brief Tries to load JSON file with given name, if succeeded then stores all the loaded values to the related 
     * fields of the specified structure. 
     * 
     * @param file_name The name of the strict JSON file
     * @typename TValue - Type of the structure which fields matches the objects from JSON file. Each field ot the TValue 
     * have to be an instance of the erock::field structure with proper name. All these fields are required in the file, 
     * if the attribute of the object is not present in the file, the default value will be stored to the desired position.
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
     * \brief Opens or creates file with given name (to owerwrite) then stores all contant to these file.
     */ 
    template<typename TValue>
    auto store(const TValue& value) {

    }
}

/*! @} */
