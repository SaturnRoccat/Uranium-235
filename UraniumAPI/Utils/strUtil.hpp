#pragma once 
#include <string>
#include <functional>
#include <string_view>
#include "rapidProxy.hpp"
#include "packedLengthBool.hpp"


namespace Uranium
{
    // Mostly a utility class for handling strings so RJ doesnt have to do like 6000 allocations and string refs
    class CStrWithLength
    {
    public:
        CStrWithLength() : str(NULL), lengthAndAutoRelease(0) {}
        CStrWithLength(const char* str, unsigned long long length) : str(const_cast<char*>(str)), lengthAndAutoRelease(length) {}
        template<size_t N>
        CStrWithLength(const char(&str)[N]) : str(const_cast<char*>(str)), lengthAndAutoRelease(N) {}
        ~CStrWithLength() { if (lengthAndAutoRelease.getBool()) delete[] str; }
        explicit CStrWithLength(const std::string& str) 
        {
            char* temp = new char[str.size()];
            this->str = temp;
			this->lengthAndAutoRelease.setLength(str.size());
			memcpy(this->str, str.c_str(), str.size());
        }
    public:
        inline rapidjson::Value toValue() { return rapidjson::Value(str, rapidjson::SizeType(lengthAndAutoRelease.getLength())); }
        inline char* c_str() { return str; }
        inline size_t size() { return lengthAndAutoRelease.getLength(); }
        inline void setAutoDelete(bool autoDelete) { this->lengthAndAutoRelease.setBool(autoDelete); }
        inline std::string toString() { return std::string(str, lengthAndAutoRelease.getLength()); }
    public:
        operator const char* () { return str; }
        operator char* () { return str; }
        operator size_t () { return lengthAndAutoRelease.getLength(); }
        explicit operator rapidjson::Value() { return rapidjson::Value(str, rapidjson::SizeType(lengthAndAutoRelease.getLength())); }
        operator std::string_view() { return std::string_view(str, lengthAndAutoRelease.getLength()); }
        char& operator[](size_t index) { return str[index]; }
    private:
        char* str = nullptr;
        PackedLengthBool lengthAndAutoRelease = 0;

    friend struct CStrWithLengthHasher; // for hashing
    };

    struct CStrWithLengthHasher
    {
        std::size_t operator()(CStrWithLength& k) const
        {
            return std::hash<std::string_view>()(std::string_view(k.str, k.size()));
        }
    };
    bool containsUppercase(const std::string& str);
    bool containsUppercase(const char* str);
    bool containsUppercase(std::string_view str);
    bool containsUppercase(CStrWithLength str);

    void toLowercase(std::string& str); 
    void toLowercase(char* str);
    void toLowercase(std::string_view str);
    void toLowercase(CStrWithLength str);




} // namespace Uranium
