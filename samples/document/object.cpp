#include <iostream>
#include <fstream>

#include "erock/strict/document.h"
#include "erock/strict/types_macro.h"
#include "hope/tuple/print_tuple.h"
#include "hope/tuple/tuple_policy.h"

struct simple_struct final {
    JINT(field1);
    JBOOL(field2);
    JREAL(field3);
};

struct struct_with_struct final {
    JOBJECT(struct_field, simple_struct);
    JREAL(field2);
    JSTRING(field3);
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
            std::cout << field.name << " : " << field.value << std::endl;
        });
    }
    catch(const std::exception& ex) {
        std::cout << ex.what();
    }

    struct_with_struct s;
    s.field3 = "new string";
    s.field2 = 11.0;
    s.struct_field.value.field1 = 38;
    auto&& json_serialized = erock::store(s);
    std::cout << json_serialized << std::endl;
    return 0;
}