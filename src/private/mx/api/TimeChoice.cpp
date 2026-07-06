// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/TimeChoice.h"

#include <cassert>
#include <optional>
#include <utility>

namespace mx
{
namespace api
{

// Maps a complex symbol to the narrow simple symbol, but only for the three values the simple case
// can express. The unusual display modes (single-number, note, dotted-note) have no simple
// equivalent, so they return nullopt and keep the signature complex.
static std::optional<TimeSignatureSymbol> timeChoiceSimpleSymbol(ComplexTimeSymbol symbol)
{
    switch (symbol)
    {
    case ComplexTimeSymbol::unspecified:
        return TimeSignatureSymbol::unspecified;
    case ComplexTimeSymbol::common:
        return TimeSignatureSymbol::common;
    case ComplexTimeSymbol::cut:
        return TimeSignatureSymbol::cut;
    case ComplexTimeSymbol::singleNumber:
    case ComplexTimeSymbol::note:
    case ComplexTimeSymbol::dottedNote:
        return std::nullopt;
    }
    return std::nullopt;
}

// A metered signature is simple-equivalent when it is a single plain fraction with no decorations:
// one fraction, no interchangeable, no separator, and a symbol the simple case can express. Returns
// the equivalent simple value, or nullopt when the meter is genuinely complex.
static std::optional<TimeSignatureData> timeChoiceAsSimpleEquivalent(const ComplexTimeSignature &value)
{
    if (!value.isMetered())
    {
        return std::nullopt;
    }
    const auto &metered = value.asMetered();
    if (metered.fractions.size() != 1 || metered.interchangeable.has_value() ||
        metered.separator != TimeSeparator::unspecified)
    {
        return std::nullopt;
    }
    const auto simpleSymbol = timeChoiceSimpleSymbol(metered.symbol);
    if (!simpleSymbol.has_value())
    {
        return std::nullopt;
    }
    return TimeSignatureData{*simpleSymbol, metered.fractions.front()};
}

TimeChoice::TimeChoice() : myValue{TimeSignatureData{}}
{
}

TimeChoice TimeChoice::simple(TimeSignatureData value)
{
    TimeChoice result;
    result.myValue = std::move(value);
    return result;
}

TimeChoice TimeChoice::complex(ComplexTimeSignature value)
{
    if (const auto simpleEquivalent = timeChoiceAsSimpleEquivalent(value); simpleEquivalent.has_value())
    {
        return simple(*simpleEquivalent);
    }
    TimeChoice result;
    result.myValue = std::move(value);
    return result;
}

TimeChoice::Kind TimeChoice::kind() const
{
    return std::holds_alternative<TimeSignatureData>(myValue) ? Kind::simple : Kind::complex;
}

bool TimeChoice::isSimple() const
{
    return std::holds_alternative<TimeSignatureData>(myValue);
}

bool TimeChoice::isComplex() const
{
    return std::holds_alternative<ComplexTimeSignature>(myValue);
}

const TimeSignatureData &TimeChoice::asSimple() const
{
    assert(isSimple());
    return *std::get_if<TimeSignatureData>(&myValue);
}

const ComplexTimeSignature &TimeChoice::asComplex() const
{
    assert(isComplex());
    return *std::get_if<ComplexTimeSignature>(&myValue);
}

bool TimeChoice::operator==(const TimeChoice &other) const
{
    return isImplicit == other.isImplicit && display == other.display && myValue == other.myValue;
}

} // namespace api
} // namespace mx
