/* Copyright (C) 2025 - 2026 JSON Parser Tests
 * Comprehensive test suite for the custom JSON parser
 */

#include <gtest/gtest.h>
#include "hope/json_parser.h"
#include <sstream>

using namespace hope::json;

// ============================================================================
// Basic Type Tests
// ============================================================================

TEST(JsonParserBasics, ParseNull) {
    Document doc;
    auto result = doc.Parse("null");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsNull());
}

TEST(JsonParserBasics, ParseTrue) {
    Document doc;
    auto result = doc.Parse("true");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsTrue());
    EXPECT_TRUE(doc.GetBool());
}

TEST(JsonParserBasics, ParseFalse) {
    Document doc;
    auto result = doc.Parse("false");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsFalse());
    EXPECT_FALSE(doc.GetBool());
}

TEST(JsonParserBasics, ParseInteger) {
    Document doc;
    auto result = doc.Parse("42");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsInt());
    EXPECT_EQ(doc.GetInt(), 42);
}

TEST(JsonParserBasics, ParseNegativeInteger) {
    Document doc;
    auto result = doc.Parse("-123");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsInt());
    EXPECT_EQ(doc.GetInt(), -123);
}

TEST(JsonParserBasics, ParseDouble) {
    Document doc;
    auto result = doc.Parse("3.14");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsDouble());
    EXPECT_DOUBLE_EQ(doc.GetDouble(), 3.14);
}

TEST(JsonParserBasics, ParseScientificNotation) {
    Document doc;
    auto result = doc.Parse("1.23e-4");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsDouble());
    EXPECT_NEAR(doc.GetDouble(), 1.23e-4, 1e-8);
}

TEST(JsonParserBasics, ParseString) {
    Document doc;
    auto result = doc.Parse("\"hello world\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "hello world");
}

TEST(JsonParserBasics, ParseEmptyString) {
    Document doc;
    auto result = doc.Parse("\"\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "");
}

// ============================================================================
// String Escape Sequence Tests
// ============================================================================

TEST(JsonParserEscapes, ParseStringWithQuote) {
    Document doc;
    auto result = doc.Parse("\"\\\"quoted\\\"\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "\"quoted\"");
}

TEST(JsonParserEscapes, ParseStringWithBackslash) {
    Document doc;
    auto result = doc.Parse("\"path\\\\to\\\\file\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "path\\to\\file");
}

TEST(JsonParserEscapes, ParseStringWithNewline) {
    Document doc;
    auto result = doc.Parse("\"line1\\nline2\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "line1\nline2");
}

TEST(JsonParserEscapes, ParseStringWithTab) {
    Document doc;
    auto result = doc.Parse("\"col1\\tcol2\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "col1\tcol2");
}

TEST(JsonParserEscapes, ParseStringWithCarriageReturn) {
    Document doc;
    auto result = doc.Parse("\"line1\\rline2\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "line1\rline2");
}

TEST(JsonParserEscapes, ParseStringWithUnicode) {
    Document doc;
    auto result = doc.Parse("\"Hello \\u0041\"");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
    EXPECT_STREQ(doc.GetString(), "Hello A");
}

// ============================================================================
// Array Tests
// ============================================================================

TEST(JsonParserArray, ParseEmptyArray) {
    Document doc;
    auto result = doc.Parse("[]");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 0);
}

TEST(JsonParserArray, ParseSimpleArray) {
    Document doc;
    auto result = doc.Parse("[1, 2, 3]");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 3);
    
    auto& arr = doc.GetArray();
    EXPECT_TRUE(arr[0]->IsInt());
    EXPECT_EQ(arr[0]->GetInt(), 1);
    EXPECT_TRUE(arr[1]->IsInt());
    EXPECT_EQ(arr[1]->GetInt(), 2);
    EXPECT_TRUE(arr[2]->IsInt());
    EXPECT_EQ(arr[2]->GetInt(), 3);
}

TEST(JsonParserArray, ParseStringArray) {
    Document doc;
    auto result = doc.Parse("[\"a\", \"b\", \"c\"]");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 3);
    
    auto& arr = doc.GetArray();
    EXPECT_STREQ(arr[0]->GetString(), "a");
    EXPECT_STREQ(arr[1]->GetString(), "b");
    EXPECT_STREQ(arr[2]->GetString(), "c");
}

TEST(JsonParserArray, ParseMixedArray) {
    Document doc;
    auto result = doc.Parse("[1, \"two\", 3.0, true, null]");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 5);
    
    auto& arr = doc.GetArray();
    EXPECT_TRUE(arr[0]->IsInt());
    EXPECT_TRUE(arr[1]->IsString());
    EXPECT_TRUE(arr[2]->IsDouble());
    EXPECT_TRUE(arr[3]->IsTrue());
    EXPECT_TRUE(arr[4]->IsNull());
}

TEST(JsonParserArray, ParseNestedArray) {
    Document doc;
    auto result = doc.Parse("[[1, 2], [3, 4]]");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 2);
    
    auto& arr = doc.GetArray();
    EXPECT_TRUE(arr[0]->IsArray());
    EXPECT_EQ(arr[0]->Size(), 2);
    EXPECT_TRUE(arr[1]->IsArray());
    EXPECT_EQ(arr[1]->Size(), 2);
}

// ============================================================================
// Object Tests
// ============================================================================

TEST(JsonParserObject, ParseEmptyObject) {
    Document doc;
    auto result = doc.Parse("{}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    EXPECT_EQ(doc.Size(), 0);
}

TEST(JsonParserObject, ParseSimpleObject) {
    Document doc;
    auto result = doc.Parse("{\"name\": \"John\", \"age\": 30}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    EXPECT_EQ(doc.Size(), 2);
    
    EXPECT_TRUE(doc.HasMember("name"));
    EXPECT_TRUE(doc.HasMember("age"));
    EXPECT_STREQ(doc["name"].GetString(), "John");
    EXPECT_EQ(doc["age"].GetInt(), 30);
}

TEST(JsonParserObject, ParseObjectWithVariousTypes) {
    Document doc;
    auto result = doc.Parse(
        "{\"string\": \"value\", \"number\": 42, \"float\": 3.14, \"bool\": true, \"null\": null}"
    );
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    
    EXPECT_TRUE(doc["string"].IsString());
    EXPECT_TRUE(doc["number"].IsInt());
    EXPECT_TRUE(doc["float"].IsDouble());
    EXPECT_TRUE(doc["bool"].IsTrue());
    EXPECT_TRUE(doc["null"].IsNull());
}

TEST(JsonParserObject, ParseObjectWithArrayValue) {
    Document doc;
    auto result = doc.Parse("{\"items\": [1, 2, 3]}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    
    EXPECT_TRUE(doc["items"].IsArray());
    EXPECT_EQ(doc["items"].Size(), 3);
}

TEST(JsonParserObject, ParseObjectWithObjectValue) {
    Document doc;
    auto result = doc.Parse("{\"person\": {\"name\": \"Alice\", \"age\": 25}}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    
    EXPECT_TRUE(doc["person"].IsObject());
    EXPECT_STREQ(doc["person"]["name"].GetString(), "Alice");
    EXPECT_EQ(doc["person"]["age"].GetInt(), 25);
}

TEST(JsonParserObject, ParseObjectWithUnicodeName) {
    Document doc;
    auto result = doc.Parse("{\"\\u006e\\u0061\\u006d\\u0065\": \"test\"}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
}

// ============================================================================
// Whitespace Handling Tests
// ============================================================================

TEST(JsonParserWhitespace, ParseWithLeadingWhitespace) {
    Document doc;
    auto result = doc.Parse("  \n\t  {\"key\": \"value\"}");
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
}

TEST(JsonParserWhitespace, ParseWithInterspersedWhitespace) {
    Document doc;
    auto result = doc.Parse(
        "{\n"
        "  \"name\" : \"John\" ,\n"
        "  \"age\" : 30\n"
        "}"
    );
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsObject());
    EXPECT_STREQ(doc["name"].GetString(), "John");
}

TEST(JsonParserWhitespace, ParseArrayWithWhitespace) {
    Document doc;
    auto result = doc.Parse(
        "[\n"
        "  1 ,\n"
        "  2 ,\n"
        "  3\n"
        "]"
    );
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 3);
}

// ============================================================================
// Stringify/Serialization Tests
// ============================================================================

TEST(JsonParserStringify, StringifyNull) {
    Document doc;
    doc.Parse("null");
    std::string json = doc.Stringify();
    EXPECT_EQ(json, "null");
}

TEST(JsonParserStringify, StringifyBool) {
    Document doc1, doc2;
    doc1.Parse("true");
    doc2.Parse("false");
    EXPECT_EQ(doc1.Stringify(), "true");
    EXPECT_EQ(doc2.Stringify(), "false");
}

TEST(JsonParserStringify, StringifyNumbers) {
    Document doc;
    doc.Parse("[42, 3.14, -100]");
    std::string json = doc.Stringify();
    EXPECT_TRUE(json.find("42") != std::string::npos);
    EXPECT_TRUE(json.find("3.14") != std::string::npos || json.find("3.1") != std::string::npos);
}

TEST(JsonParserStringify, StringifyString) {
    Document doc;
    doc.Parse("\"hello\"");
    EXPECT_EQ(doc.Stringify(), "\"hello\"");
}

TEST(JsonParserStringify, StringifyStringWithEscapes) {
    Document doc;
    doc.Parse("\"line1\\nline2\"");
    std::string json = doc.Stringify();
    EXPECT_TRUE(json.find("\\n") != std::string::npos);
}

TEST(JsonParserStringify, StringifyArray) {
    Document doc;
    doc.Parse("[1, 2, 3]");
    std::string json = doc.Stringify();
    EXPECT_EQ(json, "[1,2,3]");
}

TEST(JsonParserStringify, StringifyObject) {
    Document doc;
    doc.Parse("{\"a\": 1}");
    std::string json = doc.Stringify();
    EXPECT_TRUE(json.find("\"a\"") != std::string::npos);
    EXPECT_TRUE(json.find("1") != std::string::npos);
}

TEST(JsonParserStringify, StringifyComplexStructure) {
    Document doc;
    doc.Parse("{\"items\": [1, 2, 3], \"name\": \"test\"}");
    std::string json = doc.Stringify();
    EXPECT_TRUE(json.find("\"items\"") != std::string::npos);
    EXPECT_TRUE(json.find("[1,2,3]") != std::string::npos);
    EXPECT_TRUE(json.find("\"name\"") != std::string::npos);
    EXPECT_TRUE(json.find("\"test\"") != std::string::npos);
}

// ============================================================================
// Complex Structure Tests
// ============================================================================

TEST(JsonParserComplex, ParseComplexPersonObject) {
    const char* json = R"({
        "firstName": "John",
        "lastName": "Doe",
        "age": 30,
        "isStudent": false,
        "courses": ["Math", "Science", "English"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown",
            "zipcode": "12345"
        }
    })";
    
    Document doc;
    auto result = doc.Parse(json);
    EXPECT_TRUE(result);
    
    EXPECT_STREQ(doc["firstName"].GetString(), "John");
    EXPECT_STREQ(doc["lastName"].GetString(), "Doe");
    EXPECT_EQ(doc["age"].GetInt(), 30);
    EXPECT_FALSE(doc["isStudent"].GetBool());
    EXPECT_EQ(doc["courses"].Size(), 3);
    EXPECT_STREQ(doc["address"]["city"].GetString(), "Anytown");
}

TEST(JsonParserComplex, ParseArrayOfObjects) {
    const char* json = R"([
        {"id": 1, "name": "Alice"},
        {"id": 2, "name": "Bob"},
        {"id": 3, "name": "Charlie"}
    ])";
    
    Document doc;
    auto result = doc.Parse(json);
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsArray());
    EXPECT_EQ(doc.Size(), 3);
    
    auto& arr = doc.GetArray();
    EXPECT_EQ(arr[0]->operator[]("id").GetInt(), 1);
    EXPECT_STREQ(arr[1]->operator[]("name").GetString(), "Bob");
}

// ============================================================================
// Error Handling Tests
// ============================================================================

TEST(JsonParserErrors, ParseInvalidJson) {
    Document doc;
    auto result = doc.Parse("{invalid}");
    EXPECT_FALSE(result);
}

TEST(JsonParserErrors, ParseUnterminatedString) {
    Document doc;
    auto result = doc.Parse("\"unterminated");
    EXPECT_FALSE(result);
}

TEST(JsonParserErrors, ParseMissingComma) {
    Document doc;
    auto result = doc.Parse("[1 2 3]");
    EXPECT_FALSE(result);
}

TEST(JsonParserErrors, ParseMissingColon) {
    Document doc;
    auto result = doc.Parse("{\"key\" \"value\"}");
    EXPECT_FALSE(result);
}

TEST(JsonParserErrors, ParseEmptyInput) {
    Document doc;
    auto result = doc.Parse("");
    EXPECT_FALSE(result);
}

// ============================================================================
// Value Manipulation Tests
// ============================================================================

TEST(JsonParserManipulation, CreateAndSetValues) {
    Document doc;
    doc.SetObject();
    
    doc["name"] = Value("Alice");
    doc["age"] = Value(25);
    doc["active"] = Value(true);
    
    EXPECT_STREQ(doc["name"].GetString(), "Alice");
    EXPECT_EQ(doc["age"].GetInt(), 25);
    EXPECT_TRUE(doc["active"].GetBool());
}

TEST(JsonParserManipulation, CreateArray) {
    Document doc;
    doc.SetArray();
    
    doc.GetArray().push_back(std::make_shared<Value>(1));
    doc.GetArray().push_back(std::make_shared<Value>(2));
    doc.GetArray().push_back(std::make_shared<Value>(3));
    
    EXPECT_EQ(doc.Size(), 3);
}

TEST(JsonParserManipulation, ModifyExistingObject) {
    Document doc;
    doc.Parse("{\"x\": 10, \"y\": 20}");
    
    doc["x"] = Value(100);
    EXPECT_EQ(doc["x"].GetInt(), 100);
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST(JsonParserEdgeCases, ParseZero) {
    Document doc;
    auto result = doc.Parse("0");
    EXPECT_TRUE(result);
    EXPECT_EQ(doc.GetInt(), 0);
}

TEST(JsonParserEdgeCases, ParseLargeNumber) {
    Document doc;
    auto result = doc.Parse("9223372036854775807");
    EXPECT_TRUE(result);
}

TEST(JsonParserEdgeCases, ParseNegativeZero) {
    Document doc;
    auto result = doc.Parse("-0");
    EXPECT_TRUE(result);
}

TEST(JsonParserEdgeCases, ParseStringWithAllEscapeSequences) {
    Document doc;
    auto result = doc.Parse(
        "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\""
    );
    EXPECT_TRUE(result);
    EXPECT_TRUE(doc.IsString());
}

TEST(JsonParserEdgeCases, RoundTripSimple) {
    const char* original = "{\"key\": \"value\"}";
    Document doc1;
    doc1.Parse(original);
    
    std::string serialized = doc1.Stringify();
    Document doc2;
    auto result = doc2.Parse(serialized.c_str());
    
    EXPECT_TRUE(result);
    EXPECT_STREQ(doc2["key"].GetString(), "value");
}

TEST(JsonParserEdgeCases, RoundTripComplex) {
    const char* original = R"({
        "array": [1, 2, 3],
        "nested": {"a": "b"},
        "bool": true,
        "null": null
    })";
    
    Document doc1;
    doc1.Parse(original);
    
    std::string serialized = doc1.Stringify();
    Document doc2;
    auto result = doc2.Parse(serialized.c_str());
    
    EXPECT_TRUE(result);
    EXPECT_EQ(doc2["array"].Size(), 3);
}
