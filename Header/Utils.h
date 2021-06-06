#pragma once
#include <iostream>

namespace Utils
{
    static bool Replace(std::string& str, const std::string& from, const std::string& to)
    {
        const size_t startPos = str.find(from);
        if (startPos == std::string::npos)
            return false;
        str.replace(startPos, from.length(), to);
        return true;
    }
}


//std::string string("hello $name");
//replace(string, "$name", "Somename");