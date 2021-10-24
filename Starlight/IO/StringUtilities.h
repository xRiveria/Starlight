#pragma once
#include <string>

class StringUtilities
{
public:
    static bool IsEmptyOrWhitespace(const std::string& text);
    static bool IsAlphanumeric(const std::string& text);
    static std::string RemoveInvalidCharacters(const std::string& text);
    static std::string ConvertToUppercase(const std::string& lowercase);
    static std::string ConvertToLowercase(const std::string& uppercase);
};