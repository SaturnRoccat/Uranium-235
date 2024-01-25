#include "strUtil.hpp"


namespace Uranium
{
    bool containsUppercase(const std::string& str)
    {
        for (char c : str)
        {
            if (c >= 'A' && c <= 'Z')
            {
                return true;
            }
        }
        return false;
    };

    void toLowercase(std::string& str)
    {
        for (char& c : str)
        {
            if (c >= 'A' && c <= 'Z')
            {
                c += 32;
            }
        }
    };
}