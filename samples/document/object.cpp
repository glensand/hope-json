#include <iostream>
#include <fstream>

#include "erock/document.h"
#include "erock/types_macro.h"
#include "hope/tuple/print_tuple.h"
#include "hope/tuple/tuple_policy.h"
#include "erock/optional.h"

struct simple_struct final {
    NJINT(field1);
    SJBOOL(field2);
    SJREAL(field3);
};

struct struct_with_struct final {
    SJOBJECT(struct_field, simple_struct);
    SJREAL(field2);
    SJSTRING(field3);
};

int main(){
    constexpr auto* json = 
        "{"
            "\"field1\" : 11,"
            "\"field2\" : true,"
            "\"field3\" : 11.0"
        "}";
    
    try {
        auto&& doc = erock::load<simple_struct>(json);

        std::cout << "Parsed" << std::endl;
        auto&& tuple = tuple_from_struct(doc, hope::field_policy::reference{});
        tuple.for_each([&](auto&& field) {
            std::cout << field.name << " : " << erock::get(field.value) << std::endl;
        });
    }
    catch(const std::exception& ex) {
        std::cout << ex.what();
    }

    auto&& doc = erock::load<simple_struct>(json);
    std::cout << "Parsed" << std::endl;
    simple_struct ss;
    auto&& tuple = tuple_from_struct(ss, hope::field_policy::reference{});

    struct_with_struct s;
    s.field3 = "new string";
    s.field2 = 11.0;
    s.struct_field.value.field1 = 38;
    auto&& json_serialized = erock::store(s);
    std::cout << json_serialized << std::endl;

    return 0;
}