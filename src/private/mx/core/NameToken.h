// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <string>
#include <string_view>

namespace mx::core
{

/// An xs:NMTOKEN: a non-empty run of XML name characters
/// (letters/digits/'.'/'-'/'_'/':'). Wider than an NCName -- a leading
/// digit is allowed (`<lyric number="1">`) -- but still forbids whitespace
/// and emptiness. Construction and mutation repair to the nearest valid
/// token; the natural zero is "X".
class NameToken final
{
  public:
    NameToken();

    explicit NameToken(std::string value);

    const std::string &value() const noexcept
    {
        return m_value;
    }

    void setValue(std::string value);

    std::string toString() const
    {
        return m_value;
    }

    /// Strict: a non-empty all-name-character token.
    static bool tryParse(std::string_view text, NameToken &out);

    /// Lenient: repairs into the nearest valid token.
    static NameToken parse(std::string_view text);

    bool operator==(const NameToken &other) const noexcept = default;

  private:
    void repair();

    std::string m_value;
};

} // namespace mx::core
