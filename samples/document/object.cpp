#include "erock/strict/document.h"

struct simple_struct{
    erock::int_t v1{"field1"};
    erock::bool_t v2{"field2"};
    erock::real_t v3{"field3"};
};

int main(){
    using doc_t = erock::object<simple_struct>;
    auto&& doc = erock::load<doc_t>("test.json");
}