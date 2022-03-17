/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

#include "erock/strict/document.h"
#include "erock/strict/types_macro.h"
#include "gtest/gtest.h"
#include <iostream>

namespace{

    struct simple_struct final {
        JINT(field1);
        JBOOL(field2);
        JREAL(field3);
    };

    struct struct_with_struct final {
        JOBJECT(struct_field, simple_struct);
        JREAL(field2);
    };

    struct struct_with_struct_array final {
        JARRAY(struct_array, simple_struct);
        JREAL(field2);
    };

    struct struct_with_simple_array final {
        JARRAY(simple_array, erock::raw_int_t);
    };

    constexpr auto* simple_json = 
        "{"
            "\"field1\" : 11,"
            "\"field2\" : true,"
            "\"field3\" : 11.0"
        "}";

    constexpr auto* struct_with_struct_json = 
        "{"
            "\"struct_field\" :"
            "{"
                "\"field1\" : 11,"
                "\"field2\" : true,"
                "\"field3\" : 11.0"
            "},"
            "\"field2\" : 11.0"
        "}";

    constexpr auto* struct_with_struct_array_json = 
        "{"
            "\"struct_array\" : ["
            "{"
                "\"field1\" : 11,"
                "\"field2\" : true,"
                "\"field3\" : 11.0"
            "},"
            "{"
                "\"field1\" : 11,"
                "\"field2\" : true,"
                "\"field3\" : 11.0"
            "},"
            "{"
                "\"field1\" : 11,"
                "\"field2\" : true,"
                "\"field3\" : 11.0"
            "}"
            "],"
            "\"field2\" : 11.0"
        "}";

    constexpr auto* simple_array_json = 
        "{"
            "\"simple_array\" : [1,2,3,4,5,6,7]"
        "}";
}

TEST(Initial, simple_struct){
    using doc_t = simple_struct;
    auto&& doc = erock::load<doc_t>(simple_json);
    ASSERT_TRUE(doc.field1 == 11);
    ASSERT_TRUE(doc.field2);
    ASSERT_TRUE(std::abs(doc.field3 - 11.0) < FLT_EPSILON);
}

TEST(Initial, struct_with_struct){
    using doc_t = struct_with_struct;
    auto&& doc = erock::load<doc_t>(struct_with_struct_json);
    auto&& inner_structure = doc.struct_field.value;
    ASSERT_TRUE(inner_structure.field1 == 11);
    ASSERT_TRUE(inner_structure.field2);
    ASSERT_TRUE(std::abs(inner_structure.field3 - 11.0) < FLT_EPSILON);
}

TEST(Initial, struct_with_struct_array){
    using doc_t = struct_with_struct_array;
    auto&& doc = erock::load<doc_t>(struct_with_struct_array_json);
    auto&& vec = doc.struct_array.value;
    ASSERT_TRUE(vec.size() == 3);
    for(auto&& v : vec){
        ASSERT_TRUE(v.field1 == 11);
        ASSERT_TRUE(v.field2);
        ASSERT_TRUE(std::abs(v.field3 - 11.0) < FLT_EPSILON);
    }
}

TEST(Initial, struct_with_simple_array){
    using doc_t = struct_with_struct_array;
    auto&& doc = erock::load<struct_with_simple_array>(simple_array_json);
    auto&& vec = doc.simple_array.value;
    ASSERT_TRUE(vec.size() == 7);
}

TEST(Store, struct_with_simple_array){
    using doc_t = struct_with_struct_array;
    auto&& doc = erock::load<struct_with_simple_array>(simple_array_json);
    auto&& loaded_result = erock::store(doc);
    auto&& loaded_doc = erock::load<struct_with_simple_array>(loaded_result);
}