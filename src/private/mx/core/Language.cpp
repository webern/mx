// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Language.h"

#include <cstddef>
#include <utility>

namespace mx::core
{

// The pattern is ASCII (the builtin xs:language datatype); a non-ASCII letter
// is a grammar violation, not something to widen the check for.
bool languageIsLetter(char c) noexcept
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool languageIsSubtagChar(char c) noexcept
{
    return languageIsLetter(c) || (c >= '0' && c <= '9');
}

// The xs:language pattern: 1-8 letters, then zero or more '-' followed by 1-8
// letters or digits.
bool languageMatchesPattern(std::string_view text) noexcept
{
    if (text.empty())
    {
        return false;
    }
    std::size_t i = 0;
    std::size_t run = 0;
    while (i < text.size() && languageIsLetter(text[i]))
    {
        ++i;
        ++run;
    }
    if (run == 0 || run > 8)
    {
        return false;
    }
    while (i < text.size())
    {
        if (text[i] != '-')
        {
            return false;
        }
        ++i;
        run = 0;
        while (i < text.size() && languageIsSubtagChar(text[i]))
        {
            ++i;
            ++run;
        }
        if (run == 0 || run > 8)
        {
            return false;
        }
    }
    return true;
}

Language::Language() = default;

Language::Language(std::string value) : m_value{std::move(value)}
{
    repair();
}

void Language::setValue(std::string value)
{
    m_value = std::move(value);
    repair();
}

void Language::repair()
{
    // Surrounding XML whitespace is not a repair (the contract the identity
    // wrappers share), so normalize it away before judging the tag.
    m_value = std::string{trimWhitespace(m_value)};
    if (!m_value.empty() && !languageMatchesPattern(m_value))
    {
        m_value.clear();
    }
}

bool Language::tryParse(std::string_view text, Language &out)
{
    if (!text.empty() && !languageMatchesPattern(text))
    {
        return false;
    }
    out = Language{std::string{text}};
    return true;
}

Language Language::parse(std::string_view text)
{
    return Language{std::string{text}};
}

Language Language::parse(std::string_view text, ValueParseOutcome &outcome)
{
    Language strict;
    outcome = tryParse(trimWhitespace(text), strict) ? ValueParseOutcome::valid : ValueParseOutcome::invalid;
    return parse(text);
}

} // namespace mx::core