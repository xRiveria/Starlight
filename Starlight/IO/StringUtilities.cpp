#include "StringUtilities.h"
#include <algorithm>

bool StringUtilities::IsEmptyOrWhitespace(const std::string& text)
{
    // Check if it's empty.
    if (text.empty())
    {
        return true;
    }

    // Check if its made up of whitespace characters.
    for (char _char : text)
    {
        if (!std::isspace(_char))
        {
            return false;
        }
    }

    return true;
}

bool StringUtilities::IsAlphanumeric(const std::string& text)
{
    if (IsEmptyOrWhitespace(text))
    {
        return false;
    }

    for (char _char : text)
    {
        if (!std::isalnum(_char))
        {
            return false;
        }
    }

    return true;
}

std::string StringUtilities::ConvertToUppercase(const std::string& lowercase)
{
    std::string convertedString = lowercase;

    // Convert string to uppercase.
    std::for_each(convertedString.begin(), convertedString.end(), [](char& character)
    {
        character = std::toupper(character);
    });

    return convertedString;
}

std::string StringUtilities::ConvertToLowercase(const std::string& uppercase)
{
    std::string convertedString = uppercase;

    // Convert string to lowercase.
    std::for_each(convertedString.begin(), convertedString.end(), [](char& character)
    {
        character = std::tolower(character);
    });

    return convertedString;
}

std::string StringUtilities::RemoveInvalidCharacters(const std::string& text)
{
    std::string legalText = text;

    // Remove characters which are illegal for both names and paths.
    std::string illegalCharacters = ":?\"<>|";

    for (auto iterator = legalText.begin(); iterator < legalText.end(); ++iterator)
    {
        if (illegalCharacters.find(*iterator) != std::string::npos)
        {
            *iterator = '_'; // Replace with _.
        }
    }

    return legalText;
}
