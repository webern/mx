// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include "mx/core/Lexical.h"

#include <string>
#include <string_view>

namespace mx::core
{

/// The XML namespace's language tag (`xml:lang`): the builtin xs:language
/// pattern -- a letter-led run of 1-8 ASCII letters followed by
/// hyphen-separated subtags of 1-8 ASCII letters or digits -- or the empty
/// string, the "un-declaration" the attribute's schema unions in. The empty
/// tag is also the natural zero and the repair: a tag the grammar forbids
/// reads as "no language stated" rather than as a plausible but wrong tag
/// (surrounding XML whitespace is normalized away first, not a repair). So a
/// lexically invalid tag cannot exist through the public API.
class Language final
{
  public:
    Language();

    explicit Language(std::string value);

    const std::string &value() const noexcept
    {
        return m_value;
    }

    void setValue(std::string value);

    std::string toString() const
    {
        return m_value;
    }

    /// Strict: the language pattern, or empty.
    static bool tryParse(std::string_view text, Language &out);

    /// Lenient: an out-of-grammar tag becomes the empty tag.
    static Language parse(std::string_view text);

    /// Lenient, and says whether the text had to be repaired. Surrounding
    /// XML whitespace is not a repair.
    static Language parse(std::string_view text, ValueParseOutcome &outcome);

    bool operator==(const Language &other) const noexcept = default;

  private:
    void repair();

    std::string m_value;
};

} // namespace mx::core