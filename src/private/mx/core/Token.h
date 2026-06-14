// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <string>
#include <string_view>

namespace mx::core
{

/// An NCName-derived identity token (xs:ID / xs:IDREF): a non-empty NCName
/// (first character a letter or '_', the rest letters/digits/'.'/'-'/'_';
/// no ':', no whitespace). Construction and mutation repair to the nearest
/// valid NCName, so a lexically invalid identity token cannot exist through
/// the public API (plan §2.2). Document-wide ID uniqueness is a cross-object
/// property the type cannot enforce alone; the natural zero is "X".
class Token final
{
  public:
    Token();

    explicit Token(std::string value);

    const std::string &value() const noexcept
    {
        return m_value;
    }

    void setValue(std::string value);

    std::string toString() const
    {
        return m_value;
    }

    /// Strict: a non-empty NCName.
    static bool tryParse(std::string_view text, Token &out);

    /// Lenient: repairs into the nearest valid NCName.
    static Token parse(std::string_view text);

    bool operator==(const Token &other) const noexcept = default;

  private:
    void repair();

    std::string m_value;
};

} // namespace mx::core
