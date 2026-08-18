// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/TimeChoice.h"

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
    const auto &metered = value.metered();
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
    return TimeSignatureData{metered.fractions.front(), *simpleSymbol};
}

TimeChoice::TimeChoice() : myValue{TimeSignatureData{}}
{
}

TimeChoice::TimeChoice(TimeSignatureData value) : myValue{std::move(value)}
{
}

TimeChoice::TimeChoice(ComplexTimeSignature value)
{
    if (const auto simpleEquivalent = timeChoiceAsSimpleEquivalent(value); simpleEquivalent.has_value())
    {
        myValue = *simpleEquivalent;
    }
    else
    {
        myValue = std::move(value);
    }
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

const TimeSignatureData TimeChoice::simple() const
{
    if (const auto *value = std::get_if<TimeSignatureData>(&myValue))
    {
        return *value;
    }
    return TimeSignatureData{};
}

const ComplexTimeSignature TimeChoice::complex() const
{
    if (const auto *value = std::get_if<ComplexTimeSignature>(&myValue))
    {
        return *value;
    }
    return ComplexTimeSignature{};
}

bool TimeChoice::operator==(const TimeChoice &other) const
{
    return isImplicit == other.isImplicit && display == other.display && id == other.id && myValue == other.myValue;
}

} // namespace api
} // namespace mx
