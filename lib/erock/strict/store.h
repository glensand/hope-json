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
#include "hope/tuple/tuple_from_struct.h"

namespace erock::detail {

    /*
    * Stores trivial values into specified object
    */
    template<typename TValue>
    std::enable_if_t<is_inbuilt_v<TValue>>
    store(const TValue& value, rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {
        object = value;
    }

    template<typename TValue>
    void store(const raw_array_t<TValue>& value, rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator);

    /**
     * \brief Stores object of defined type to the given json-object
     */ 
    template<typename TValue>
    std::enable_if_t<!is_inbuilt_v<TValue>>
    store(const TValue& value, rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {
        auto&& tuple = hope::tuple_from_struct(value, hope::field_policy::reference{});
        tuple.for_each([&](auto&& child){
            using child_t = std::decay_t<decltype(child)>;
            static_assert(is_object_v<child_t>, 
                "EROCK-JSON::store: All the types of the user defined structure"
                "must be wrapped with erock::object structure.\n"
                "It is required 'cause this is only one way how to tell the library what name the object has to has"
            );
            rapidjson::Value json_child;
            store(child.value, json_child, allocator);
            rapidjson::GenericStringRef<char> rapid_name{ child.name.data() };
            object.AddMember(rapid_name, json_child.Move(), allocator);
        });
    }

    /*
    * Stores array into specified object
    */
    template<typename TValue>
    void store(const raw_array_t<TValue>& value, rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {
        object.SetArray();
        for(auto&& obj : value) {
            rapidjson::Value element;
            //store(obj, element, allocator);
            object.PushBack(element.Move(), allocator); 
        }
    }

    /**
     * \brief Stores object of defined type to the given json-document
     */ 
    template<typename TValue>
    void store(const TValue& value, rapidjson::Document& document) {
        store(value, document, document.GetAllocator());
    }
}

/*! @} */
