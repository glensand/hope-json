/* Copyright (C) 2025 - Custom JSON Parser
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

/*! \defgroup <parser> Parser
    @{
    \file
    \brief Custom lightweight JSON parser (RapidJSON replacement)
*/

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <cstdio>

#include "erock/json_parser_interface.h"

namespace hope::json {

    enum class ValueType {
        kNull,
        kFalse,
        kTrue,
        kObject,
        kArray,
        kString,
        kNumber,
        kInt,
        kDouble
    };

    // Dummy allocator for compatibility with RapidJSON API
    class DummyAllocator {
    public:
        void* Allocate(size_t size) { return nullptr; }
        void Deallocate(void* p) {}
    };

    class Value;
    using ValueRef = std::shared_ptr<Value>;

    class Value {
    public:
        using Object = std::unordered_map<std::string, ValueRef>;
        using Array = std::vector<ValueRef>;

        // Constructors
        Value() : type_(ValueType::kNull) {}
        explicit Value(bool b) : type_(b ? ValueType::kTrue : ValueType::kFalse) {}
        explicit Value(int i) : type_(ValueType::kInt), int_value_(i) {}
        explicit Value(int64_t i) : type_(ValueType::kInt), int_value_(i) {}
        explicit Value(double d) : type_(ValueType::kDouble), double_value_(d) {}
        explicit Value(const std::string& s) : type_(ValueType::kString), string_value_(s) {}
        explicit Value(std::string_view s) : type_(ValueType::kString), string_value_(s) {}
        explicit Value(const char* s) : type_(ValueType::kString), string_value_(s) {}

        // Type checks
        bool IsNull() const { return type_ == ValueType::kNull; }
        bool IsBool() const { return type_ == ValueType::kTrue || type_ == ValueType::kFalse; }
        bool IsTrue() const { return type_ == ValueType::kTrue; }
        bool IsFalse() const { return type_ == ValueType::kFalse; }
        bool IsNumber() const { return type_ == ValueType::kInt || type_ == ValueType::kDouble; }
        bool IsInt() const { return type_ == ValueType::kInt; }
        bool IsDouble() const { return type_ == ValueType::kDouble; }
        bool IsString() const { return type_ == ValueType::kString; }
        bool IsObject() const { return type_ == ValueType::kObject; }
        bool IsArray() const { return type_ == ValueType::kArray; }

        // Getters
        bool GetBool() const {
            if (type_ == ValueType::kTrue) return true;
            if (type_ == ValueType::kFalse) return false;
            throw std::runtime_error("Value is not a boolean");
        }

        int GetInt() const {
            if (type_ == ValueType::kInt) return static_cast<int>(int_value_);
            if (type_ == ValueType::kDouble) return static_cast<int>(double_value_);
            throw std::runtime_error("Value is not an integer");
        }

        int64_t GetInt64() const {
            if (type_ == ValueType::kInt) return int_value_;
            if (type_ == ValueType::kDouble) return static_cast<int64_t>(double_value_);
            throw std::runtime_error("Value is not an integer");
        }

        double GetDouble() const {
            if (type_ == ValueType::kDouble) return double_value_;
            if (type_ == ValueType::kInt) return static_cast<double>(int_value_);
            throw std::runtime_error("Value is not a number");
        }

        const char* GetString() const {
            if (type_ != ValueType::kString) {
                throw std::runtime_error("Value is not a string");
            }
            return string_value_.c_str();
        }

        size_t GetStringLength() const {
            if (type_ != ValueType::kString) {
                throw std::runtime_error("Value is not a string");
            }
            return string_value_.size();
        }

        // Object operations
        Value& operator[](const char* key) {
            if (type_ != ValueType::kObject) {
                type_ = ValueType::kObject;
                object_value_ = std::make_shared<Object>();
            }
            if (!object_value_) {
                object_value_ = std::make_shared<Object>();
            }
            auto& ref = (*object_value_)[key];
            if (!ref) {
                ref = std::make_shared<Value>();
            }
            return *ref;
        }

        Value& operator[](const std::string& key) {
            return (*this)[key.c_str()];
        }

        bool HasMember(const char* key) const {
            if (type_ != ValueType::kObject || !object_value_) return false;
            return object_value_->count(key) > 0 && (*object_value_)[key] != nullptr;
        }

        bool HasMember(const std::string& key) const {
            return HasMember(key.c_str());
        }

        // Array operations
        Array& GetArray() {
            if (type_ != ValueType::kArray) {
                type_ = ValueType::kArray;
                array_value_ = std::make_shared<Array>();
            }
            if (!array_value_) {
                array_value_ = std::make_shared<Array>();
            }
            return *array_value_;
        }

        const Array& GetArray() const {
            if (type_ != ValueType::kArray) {
                throw std::runtime_error("Value is not an array");
            }
            if (!array_value_) {
                throw std::runtime_error("Array is null");
            }
            return *array_value_;
        }

        size_t Size() const {
            if (type_ == ValueType::kArray && array_value_) {
                return array_value_->size();
            }
            if (type_ == ValueType::kObject && object_value_) {
                return object_value_->size();
            }
            return 0;
        }

        // Setters
        void SetNull() { type_ = ValueType::kNull; }
        void SetBool(bool b) { type_ = b ? ValueType::kTrue : ValueType::kFalse; }
        void SetInt(int i) { type_ = ValueType::kInt; int_value_ = i; }
        void SetInt64(int64_t i) { type_ = ValueType::kInt; int_value_ = i; }
        void SetDouble(double d) { type_ = ValueType::kDouble; double_value_ = d; }
        
        void SetString(const char* s, size_t len) {
            type_ = ValueType::kString;
            string_value_.assign(s, len);
        }

        void SetString(const std::string& s) {
            type_ = ValueType::kString;
            string_value_ = s;
        }

        void SetObject() {
            type_ = ValueType::kObject;
            object_value_ = std::make_shared<Object>();
        }

        void SetArray() {
            type_ = ValueType::kArray;
            array_value_ = std::make_shared<Array>();
        }

        // Move semantics
        Value Move() {
            return *this;
        }

        void PushBack(const Value& v, void* allocator = nullptr) {
            if (type_ != ValueType::kArray) {
                SetArray();
            }
            auto copy = std::make_shared<Value>(v);
            array_value_->push_back(copy);
        }

        void AddMember(const char* key, const Value& v, void* allocator = nullptr) {
            if (type_ != ValueType::kObject) {
                SetObject();
            }
            (*object_value_)[key] = std::make_shared<Value>(v);
        }

        // Iterator support for arrays
        class ArrayIterator {
        public:
            explicit ArrayIterator(Array* arr, size_t idx = 0) : array_(arr), index_(idx) {}

            Value& operator*() { return *(*array_)[index_]; }
            Value* operator->() { return (*array_)[index_].get(); }

            ArrayIterator& operator++() {
                ++index_;
                return *this;
            }

            bool operator!=(const ArrayIterator& other) const {
                return index_ != other.index_;
            }

        private:
            Array* array_;
            size_t index_;
        };

        ArrayIterator begin() {
            if (type_ != ValueType::kArray) {
                throw std::runtime_error("Value is not an array");
            }
            return ArrayIterator(array_value_.get(), 0);
        }

        ArrayIterator end() {
            if (type_ != ValueType::kArray) {
                throw std::runtime_error("Value is not an array");
            }
            return ArrayIterator(array_value_.get(), array_value_->size());
        }

        friend class Document;

    private:
        ValueType type_;
        std::string string_value_;
        int64_t int_value_ = 0;
        double double_value_ = 0.0;
        std::shared_ptr<Object> object_value_;
        std::shared_ptr<Array> array_value_;
    };

    class ParseResult {
    public:
        ParseResult() : ok_(true), error_code_(0), offset_(0) {}
        explicit ParseResult(int code, size_t offset) 
            : ok_(false), error_code_(code), offset_(offset) {}

        bool operator!() const { return !ok_; }
        explicit operator bool() const { return ok_; }

        int Code() const { return error_code_; }
        size_t Offset() const { return offset_; }

    private:
        bool ok_;
        int error_code_;
        size_t offset_;
    };

    class Document : public Value {
    public:
        using AllocatorType = DummyAllocator;

        ParseResult Parse(const char* json) {
            Parser parser(json);
            try {
                auto result = parser.parseValue();
                *static_cast<Value*>(this) = std::move(result);
                return ParseResult();
            } catch (const std::exception& e) {
                return ParseResult(1, parser.getOffset());
            }
        }

        void* GetAllocator() {
            return &allocator_;
        }

        std::string Stringify() const {
            return stringifyValue(*static_cast<const Value*>(this));
        }

    private:
        AllocatorType allocator_;

        std::string stringifyValue(const Value& val) const {
            if (val.IsNull()) {
                return "null";
            } else if (val.IsTrue()) {
                return "true";
            } else if (val.IsFalse()) {
                return "false";
            } else if (val.IsInt()) {
                return std::to_string(val.GetInt());
            } else if (val.IsDouble()) {
                auto d = val.GetDouble();
                std::string result = std::to_string(d);
                // Remove trailing zeros for clean output
                size_t dot_pos = result.find('.');
                if (dot_pos != std::string::npos) {
                    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
                    if (result.back() == '.') result.pop_back();
                }
                return result;
            } else if (val.IsString()) {
                std::string result = "\"";
                const std::string& str = val.string_value_;
                for (char c : str) {
                    switch (c) {
                        case '"': result += "\\\""; break;
                        case '\\': result += "\\\\"; break;
                        case '\b': result += "\\b"; break;
                        case '\f': result += "\\f"; break;
                        case '\n': result += "\\n"; break;
                        case '\r': result += "\\r"; break;
                        case '\t': result += "\\t"; break;
                        default:
                            if (static_cast<unsigned char>(c) < 0x20) {
                                char buf[7];
                                snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                                result += buf;
                            } else {
                                result += c;
                            }
                    }
                }
                result += "\"";
                return result;
            } else if (val.IsArray()) {
                std::string result = "[";
                const auto& arr = val.GetArray();
                for (size_t i = 0; i < arr.size(); ++i) {
                    result += stringifyValue(*arr[i]);
                    if (i < arr.size() - 1) result += ",";
                }
                result += "]";
                return result;
            } else if (val.IsObject()) {
                std::string result = "{";
                if (val.object_value_) {
                    bool first = true;
                    for (const auto& pair : *val.object_value_) {
                        if (!first) result += ",";
                        result += "\"" + pair.first + "\":" + stringifyValue(*pair.second);
                        first = false;
                    }
                }
                result += "}";
                return result;
            }
            return "";
        }

        class Parser {
        public:
            explicit Parser(const char* json) : data_(json), offset_(0) {}

            Value parseValue() {
                skipWhitespace();
                if (!data_[offset_]) {
                    throw std::runtime_error("Unexpected end of input");
                }

                char c = data_[offset_];
                if (c == '{') return parseObject();
                if (c == '[') return parseArray();
                if (c == '"') return parseString();
                if (c == 't' || c == 'f') return parseBoolean();
                if (c == 'n') return parseNull();
                if (c == '-' || std::isdigit(c)) return parseNumber();

                throw std::runtime_error(std::string("Unexpected character: ") + c);
            }

            size_t getOffset() const { return offset_; }

        private:
            const char* data_;
            size_t offset_;

            void skipWhitespace() {
                while (data_[offset_] && std::isspace(data_[offset_])) {
                    ++offset_;
                }
            }

            Value parseObject() {
                Value obj;
                obj.SetObject();
                ++offset_; // skip '{'
                skipWhitespace();

                if (data_[offset_] == '}') {
                    ++offset_;
                    return obj;
                }

                while (true) {
                    skipWhitespace();
                    if (data_[offset_] != '"') {
                        throw std::runtime_error("Expected string key in object");
                    }

                    auto key = parseStringValue();
                    skipWhitespace();

                    if (data_[offset_] != ':') {
                        throw std::runtime_error("Expected ':' after key in object");
                    }
                    ++offset_;

                    auto value = parseValue();
                    obj[key.c_str()] = value;

                    skipWhitespace();
                    if (data_[offset_] == '}') {
                        ++offset_;
                        break;
                    }
                    if (data_[offset_] != ',') {
                        throw std::runtime_error("Expected ',' or '}' in object");
                    }
                    ++offset_;
                }

                return obj;
            }

            Value parseArray() {
                Value arr;
                arr.SetArray();
                ++offset_; // skip '['
                skipWhitespace();

                if (data_[offset_] == ']') {
                    ++offset_;
                    return arr;
                }

                while (true) {
                    auto value = parseValue();
                    arr.GetArray().push_back(std::make_shared<Value>(value));

                    skipWhitespace();
                    if (data_[offset_] == ']') {
                        ++offset_;
                        break;
                    }
                    if (data_[offset_] != ',') {
                        throw std::runtime_error("Expected ',' or ']' in array");
                    }
                    ++offset_;
                    skipWhitespace();
                }

                return arr;
            }

            Value parseString() {
                auto str = parseStringValue();
                return Value(str);
            }

            std::string parseStringValue() {
                ++offset_; // skip opening quote
                std::string result;

                while (data_[offset_] && data_[offset_] != '"') {
                    if (data_[offset_] == '\\') {
                        ++offset_;
                        if (!data_[offset_]) {
                            throw std::runtime_error("Unexpected end in string escape");
                        }

                        switch (data_[offset_]) {
                            case '"': result += '"'; break;
                            case '\\': result += '\\'; break;
                            case '/': result += '/'; break;
                            case 'b': result += '\b'; break;
                            case 'f': result += '\f'; break;
                            case 'n': result += '\n'; break;
                            case 'r': result += '\r'; break;
                            case 't': result += '\t'; break;
                            case 'u': {
                                ++offset_;
                                uint32_t codepoint = 0;
                                for (int i = 0; i < 4; ++i) {
                                    codepoint = codepoint * 16;
                                    if (std::isdigit(data_[offset_])) {
                                        codepoint += data_[offset_] - '0';
                                    } else if (data_[offset_] >= 'a' && data_[offset_] <= 'f') {
                                        codepoint += data_[offset_] - 'a' + 10;
                                    } else if (data_[offset_] >= 'A' && data_[offset_] <= 'F') {
                                        codepoint += data_[offset_] - 'A' + 10;
                                    } else {
                                        throw std::runtime_error("Invalid unicode escape");
                                    }
                                    ++offset_;
                                }
                                --offset_; // back up one since we'll increment at the end
                                // Simple UTF-8 encoding (only handles BMP for simplicity)
                                if (codepoint < 0x80) {
                                    result += static_cast<char>(codepoint);
                                } else if (codepoint < 0x800) {
                                    result += static_cast<char>(0xC0 | (codepoint >> 6));
                                    result += static_cast<char>(0x80 | (codepoint & 0x3F));
                                } else {
                                    result += static_cast<char>(0xE0 | (codepoint >> 12));
                                    result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                                    result += static_cast<char>(0x80 | (codepoint & 0x3F));
                                }
                                break;
                            }
                            default: throw std::runtime_error("Unknown escape sequence");
                        }
                        ++offset_;
                    } else {
                        result += data_[offset_];
                        ++offset_;
                    }
                }

                if (data_[offset_] != '"') {
                    throw std::runtime_error("Unterminated string");
                }
                ++offset_; // skip closing quote

                return result;
            }

            Value parseBoolean() {
                if (data_[offset_] == 't') {
                    if (std::string_view(data_ + offset_, 4) == "true") {
                        offset_ += 4;
                        return Value(true);
                    }
                } else if (data_[offset_] == 'f') {
                    if (std::string_view(data_ + offset_, 5) == "false") {
                        offset_ += 5;
                        return Value(false);
                    }
                }
                throw std::runtime_error("Invalid boolean");
            }

            Value parseNull() {
                if (std::string_view(data_ + offset_, 4) == "null") {
                    offset_ += 4;
                    return Value();
                }
                throw std::runtime_error("Invalid null");
            }

            Value parseNumber() {
                size_t start = offset_;

                if (data_[offset_] == '-') ++offset_;

                if (!std::isdigit(data_[offset_])) {
                    throw std::runtime_error("Invalid number");
                }

                if (data_[offset_] == '0') {
                    ++offset_;
                } else {
                    while (std::isdigit(data_[offset_])) ++offset_;
                }

                bool isDouble = false;
                if (data_[offset_] == '.') {
                    isDouble = true;
                    ++offset_;
                    if (!std::isdigit(data_[offset_])) {
                        throw std::runtime_error("Invalid number: expected digit after decimal point");
                    }
                    while (std::isdigit(data_[offset_])) ++offset_;
                }

                if (data_[offset_] == 'e' || data_[offset_] == 'E') {
                    isDouble = true;
                    ++offset_;
                    if (data_[offset_] == '+' || data_[offset_] == '-') ++offset_;
                    if (!std::isdigit(data_[offset_])) {
                        throw std::runtime_error("Invalid number: expected digit in exponent");
                    }
                    while (std::isdigit(data_[offset_])) ++offset_;
                }

                std::string numStr(data_ + start, offset_ - start);
                if (isDouble) {
                    return Value(std::stod(numStr));
                } else {
                    return Value(static_cast<int64_t>(std::stoll(numStr)));
                }
            }
        };
    };

}  // namespace json
}  // namespace hope

/*! @} */
