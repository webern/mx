// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Token.h"

#include <utility>

namespace mx::core
{

// The ASCII subset of the XML NCName character classes (the schema's
// vocabularies are ASCII; the strict parse is the only consumer, so the
// approximation can only under-accept).
bool tokenIsNameStart(char c) noexcept
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool tokenIsNameChar(char c) noexcept
{
    return tokenIsNameStart(c) || (c >= '0' && c <= '9') || c == '-' || c == '.';
}

Token::Token()
{
    repair();
}

Token::Token(std::string value) : m_value{std::move(value)}
{
    repair();
}

void Token::setValue(std::string value)
{
    m_value = std::move(value);
    repair();
}

void Token::repair()
{
    std::string cleaned;
    cleaned.reserve(m_value.size());
    for (const char c : m_value)
    {
        if (tokenIsNameChar(c))
        {
            cleaned.push_back(c);
        }
    }
    // An NCName cannot begin with a digit, '-', or '.'.
    std::size_t start = 0;
    while (start < cleaned.size() && !tokenIsNameStart(cleaned[start]))
    {
        ++start;
    }
    cleaned.erase(0, start);
    if (cleaned.empty())
    {
        cleaned = "X";
    }
    m_value = std::move(cleaned);
}

bool Token::tryParse(std::string_view text, Token &out)
{
    if (text.empty() || !tokenIsNameStart(text.front()))
    {
        return false;
    }
    for (const char c : text)
    {
        if (!tokenIsNameChar(c))
        {
            return false;
        }
    }
    out = Token{std::string{text}};
    return true;
}

Token Token::parse(std::string_view text)
{
    return Token{std::string{text}};
}

} // namespace mx::core
