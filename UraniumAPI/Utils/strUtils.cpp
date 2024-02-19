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
	};

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
	};
}