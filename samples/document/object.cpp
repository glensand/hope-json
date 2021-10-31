#include <iostream>
#include <fstream>

#include "erock/strict/document.h"
#include "hope/tuple/print_tuple.h"
#include "hope/tuple/tuple_policy.h"

struct simple_struct{
    erock::int_t v1{"field1"};
    erock::bool_t v2{"field2"};
    erock::real_t v3{"field3"};
};

int main(){
    constexpr auto* json = 
        "{"
            "\"field1\" : 11,"
            "\"field2\" : true,"
            "\"field3\" : 11.0"
        "}";
    
    try {
        using doc_t = erock::object<simple_struct>;
        auto&& doc = erock::load<doc_t>(json);

        std::cout << "Parsed" << std::endl;
        auto&& tuple = tuple_from_struct(doc.value, hope::field_policy::reference{});
        tuple.for_each([&](auto&& field) {
            std::cout << field.name << " : " << field.value << std::endl;
        });
    }
    catch(const std::exception& ex) {
        std::cout << ex.what();
    }

    return 0;
}