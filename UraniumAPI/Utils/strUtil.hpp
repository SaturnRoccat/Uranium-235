#pragma once 
#include <string>
#include "rapidProxy.hpp"

namespace Uranium
{
    bool containsUppercase(const std::string& str);
    bool containsUppercase(const char* str);

    void toLowercase(std::string& str); 
    void toLowercase(char* str);

    // Mostly a utility class for handling strings so RJ doesnt have to do like 6000 allocations and string refs
    struct CStrWithLength
    {
        const char* str;
        unsigned long long length; 

        operator const char*() { return str;}
        operator size_t () { return length; }
        operator rapidjson::Value () { return rapidjson::Value(str, length); }

        rapidjson::Value toValue() { return rapidjson::Value(str, length); }
    };
    
    
} // namespace Uranium
