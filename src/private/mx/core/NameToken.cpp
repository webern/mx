// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/NameToken.h"

#include <utility>

namespace mx::core
{

bool nameTokenIsNameChar(char c) noexcept
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '.' ||
           c == ':' || c == '_';
}

NameToken::NameToken()
{
    repair();
}

NameToken::NameToken(std::string value) : m_value{std::move(value)}
{
    repair();
}

void NameToken::setValue(std::string value)
{
    m_value = std::move(value);
    repair();
}

void NameToken::repair()
{
    std::string cleaned;
    cleaned.reserve(m_value.size());
    for (const char c : m_value)
    {
        if (nameTokenIsNameChar(c))
        {
            cleaned.push_back(c);
        }
    }
    if (cleaned.empty())
    {
        cleaned = "X";
    }
    m_value = std::move(cleaned);
}

bool NameToken::tryParse(std::string_view text, NameToken &out)
{
    if (text.empty())
    {
        return false;
    }
    for (const char c : text)
    {
        if (!nameTokenIsNameChar(c))
        {
            return false;
        }
    }
    out = NameToken{std::string{text}};
    return true;
}

NameToken NameToken::parse(std::string_view text)
{
    return NameToken{std::string{text}};
}

} // namespace mx::core
