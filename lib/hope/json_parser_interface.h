/* Copyright (C) 2025 - 2026 JSON Parser Interface
 * Interface for pluggable JSON parsers
 */

#pragma once

#include <string>
#include <string_view>
#include <memory>

namespace hope::json {

    /**
     * \brief Abstract interface for JSON value representation
     *
     * All JSON parser implementations should provide a Value-like class
     * that supports this interface for interoperability with hope-json.
     */
    class IJsonValue {
    public:
        virtual ~IJsonValue() = default;

        // Type checks
        virtual bool IsNull() const = 0;
        virtual bool IsBool() const = 0;
        virtual bool IsTrue() const = 0;
        virtual bool IsFalse() const = 0;
        virtual bool IsNumber() const = 0;
        virtual bool IsInt() const = 0;
        virtual bool IsDouble() const = 0;
        virtual bool IsString() const = 0;
        virtual bool IsObject() const = 0;
        virtual bool IsArray() const = 0;

        // Getters
        virtual bool GetBool() const = 0;
        virtual int GetInt() const = 0;
        virtual int64_t GetInt64() const = 0;
        virtual double GetDouble() const = 0;
        virtual const char* GetString() const = 0;
        virtual size_t GetStringLength() const = 0;

        // Object operations
        virtual IJsonValue& operator[](const char* key) = 0;
        virtual bool HasMember(const char* key) const = 0;

        // Array operations
        virtual size_t Size() const = 0;

        // Setters
        virtual void SetNull() = 0;
        virtual void SetBool(bool b) = 0;
        virtual void SetInt(int i) = 0;
        virtual void SetInt64(int64_t i) = 0;
        virtual void SetDouble(double d) = 0;
        virtual void SetString(const char* s, size_t len) = 0;
        virtual void SetObject() = 0;
        virtual void SetArray() = 0;

        // Array iteration
        virtual void PushBack(const IJsonValue& v, void* allocator = nullptr) = 0;
        virtual void AddMember(const char* key, const IJsonValue& v, void* allocator = nullptr) = 0;
    };

    /**
     * \brief Abstract interface for JSON documents
     */
    class IJsonDocument : public IJsonValue {
    public:
        struct ParseResult {
            bool ok = false;
            int error_code = 0;
            size_t offset = 0;

            bool operator!() const { return !ok; }
            explicit operator bool() const { return ok; }

            int Code() const { return error_code; }
            size_t Offset() const { return offset; }
        };

        virtual ~IJsonDocument() = default;

        // Parsing
        virtual ParseResult Parse(const char* json) = 0;

        // Stringification
        virtual std::string Stringify() const = 0;

        // Allocator for memory management
        virtual void* GetAllocator() = 0;
    };

}  // namespace hope::json
