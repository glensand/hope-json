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

struct simple_struct{
    erock::int_t v1{"field1"};
    erock::bool_t v2{"field2"};
    erock::real_t v3{"field3"};
};

TEST(Initial, simple_struct){
    using doc_t = erock::object<simple_struct>;
    doc_t instance;
    auto&& object = erock::load<doc_t>("test.json");
}