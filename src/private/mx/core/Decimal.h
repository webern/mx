// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <string>
#include <string_view>

namespace mx::core
{

/// A lossless decimal: a numeric face for arithmetic and clamping, and a
/// text face for round-trip fidelity (plan §2.6). A Decimal parsed from text
/// remembers the exact input spelling and serializes it back verbatim; a
/// Decimal constructed or mutated numerically serializes the shortest
/// fixed-notation string that round-trips the value. Comparison is numeric:
/// the text face is presentation, not identity.
class Decimal
{
  public:
    Decimal() = default;

    explicit Decimal(double value) : m_value{value}
    {
    }

    double value() const noexcept
    {
        return m_value;
    }

    /// Numeric mutation drops any preserved text face: the value changed,
    /// so the old spelling no longer describes it.
    void setValue(double value)
    {
        m_value = value;
        m_text.clear();
    }

    /// The wire spelling: the preserved text face when one exists, else the
    /// shortest fixed-notation round-trip of the numeric face.
    std::string toString() const;

    /// Strict parse (whole input must be a decimal). Preserves the input
    /// spelling as the text face.
    static bool tryParse(std::string_view text, Decimal &out);

    /// Lenient parse: garbage yields 0 (import leniency, plan §2.4).
    static Decimal parse(std::string_view text);

    bool operator==(const Decimal &other) const noexcept
    {
        return m_value == other.m_value;
    }

    bool operator!=(const Decimal &other) const noexcept
    {
        return !(*this == other);
    }

  private:
    double m_value{0.0};
    std::string m_text; // empty: no preserved face
};

} // namespace mx::core
