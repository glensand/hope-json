/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

#include "erock/strict/document.h"

#include "gtest/gtest.h"
#include <iostream>

namespace{

    struct simple_struct final {
        erock::int_t v1{"field1"};
        erock::bool_t v2{"field2"};
        erock::real_t v3{"field3"};
    };

    struct struct_with_struct final {
        erock::object<simple_struct> f1{"struct_field"};
        erock::real_t v3{"field2"};
    };

    struct struct_with_struct_array final {
        erock::array_t<simple_struct> f1{"struct_array"};
        erock::real_t v3{"field2"};
    };

    struct struct_with_simple_array final {
        //erock::array_t<
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
}

TEST(Initial, simple_struct){
    using doc_t = simple_struct;
    auto&& doc = erock::load<doc_t>(simple_json);
    ASSERT_TRUE(doc.v1 == 11);
    ASSERT_TRUE(doc.v2);
    ASSERT_TRUE(std::abs(doc.v3 - 11.0) < FLT_EPSILON);
}

TEST(Initial, struct_with_struct){
    using doc_t = struct_with_struct;
    auto&& doc = erock::load<doc_t>(struct_with_struct_json);
    auto&& inner_structure = doc.f1.value;
    ASSERT_TRUE(inner_structure.v1 == 11);
    ASSERT_TRUE(inner_structure.v2);
    ASSERT_TRUE(std::abs(inner_structure.v3 - 11.0) < FLT_EPSILON);
}

TEST(Initial, struct_with_struct_array){
    using doc_t = struct_with_struct_array;
    auto&& doc = erock::load<doc_t>(struct_with_struct_array_json);
    auto&& vec = doc.f1.value;
    ASSERT_TRUE(vec.size() == 3);
    for(auto&& v : vec){
        ASSERT_TRUE(v.v1 == 11);
        ASSERT_TRUE(v.v2);
        ASSERT_TRUE(std::abs(v.v3 - 11.0) < FLT_EPSILON);
    }
}