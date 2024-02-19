#include "strUtil.hpp"


namespace Uranium
{
    bool containsUppercase(const std::string& str)
    {
        for (char c : str)
        {
            if (c >= 'A' && c <= 'Z')
                return true;
        }
        return false;
    };
    bool containsUppercase(const char* str)
    {
        while (*str != '\0')
        {
			if (*str >= 'A' && *str <= 'Z')
				return true;
			str++;
		}
		return false;
	}
    bool containsUppercase(std::string_view str)
    {
        for (char c : str)
        {
			if (c >= 'A' && c <= 'Z')
				return true;
		}
        return false;
    }
    bool containsUppercase(CStrWithLength str)
    {
        for (size_t i = 0; i < str.size(); i++)
        {
			if (str[i] >= 'A' && str[i] <= 'Z')
				return true;
		}
        return false;
    }
    ;

    void toLowercase(std::string& str)
    {
        for (char& c : str)
        {
            if (c >= 'A' && c <= 'Z')
                c += 32;
        }
    };

    void toLowercase(char* str)
    {
        while (*str != '\0')
        {
			if (*str >= 'A' && *str <= 'Z')
				*str += 32;
			str++;
		}
	}
    void toLowercase(CStrWithLength string)
    {
        for (size_t i = 0; i < string.size(); i++)
		{ 
            if (string[i] >= 'A' && string[i] <= 'Z')
                string[i] += 32;
        }
    };
}