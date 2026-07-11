// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/TimeSignatureData.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace mx
{
namespace api
{

// The full MusicXML time-symbol vocabulary. Used for a complex meter's own symbol and for an
// interchangeable pair's symbol. Unlike the narrow TimeSignatureSymbol (common/cut only), this
// includes the unusual display modes that force a signature into the complex arena.
enum class ComplexTimeSymbol
{
    unspecified,  // ordinary fractional display
    common,       // the C glyph
    cut,          // the slashed-C glyph
    singleNumber, // the meter drawn as a single number
    note,         // the beat-type drawn as a downstem note glyph
    dottedNote    // the beat-type drawn as a dotted downstem note glyph
};

// The line drawn between beats and beat-type (MusicXML time-separator). 'unspecified' writes no
// attribute (the spec default is "none").
enum class TimeSeparator
{
    unspecified,
    none,
    horizontal,
    diagonal,
    vertical,
    adjacent
};

// How a dual/interchangeable pair is visually joined (MusicXML time-relation).
enum class TimeRelation
{
    unspecified,
    parentheses,
    bracket,
    equals,
    slash,
    space,
    hyphen
};

// The alternate meter of an interchangeable dual pair, e.g. the "(6/8)" of "3/4 (6/8)". It decorates
// a primary metered signature and carries its own relation, symbol, and separator, independent of
// the primary's.
struct InterchangeableTimeSignature
{
    // The alternate meter's fractions. One = simple, more than one = composite. Nonempty when used.
    std::vector<TimeFraction> fractions;

    TimeRelation relation{TimeRelation::unspecified};
    ComplexTimeSymbol symbol{ComplexTimeSymbol::unspecified};
    TimeSeparator separator{TimeSeparator::unspecified};
};

MXAPI_EQUALS_BEGIN(InterchangeableTimeSignature)
MXAPI_EQUALS_MEMBER(fractions)
MXAPI_EQUALS_MEMBER(relation)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(separator)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(InterchangeableTimeSignature);

// A metered (as opposed to senza-misura) complex time signature: a primary meter of one or more
// fractions, plus its decorations.
//
// Note: A single fraction with no symbol, no separator, and no interchangeable is simple-equivalent
// and the TimeChoice constructor collapses it back to the simple case;
struct MeteredTimeSignature
{
    // The primary meter's fractions. One = a single meter (decorated); more than one = composite
    // (additive), e.g. 2/4 + 3/8. Nonempty in a valid state.
    std::vector<TimeFraction> fractions{TimeFraction{"4", "4"}};

    ComplexTimeSymbol symbol{ComplexTimeSymbol::unspecified};
    TimeSeparator separator{TimeSeparator::unspecified};
    std::optional<InterchangeableTimeSignature> interchangeable;
};

MXAPI_EQUALS_BEGIN(MeteredTimeSignature)
MXAPI_EQUALS_MEMBER(fractions)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(separator)
MXAPI_EQUALS_MEMBER(interchangeable)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MeteredTimeSignature);

// Models the full MusicXML time element, except for the most simple cases which are modeled by
// TimeSignatureData.
//
// Included:
// - composite meters
// - symbols
// - separator selection
// - interchangeable meters
// - senza-misura
//
// Include a ComplexTimeSignature in your score using TimeChoice{complex};
class ComplexTimeSignature
{
  public:
    enum class Kind
    {
        metered,
        senzaMisura
    };

    // Defaults to a metered 4/4.
    //
    // Note: if supplied to a TimeChoice, this would collapse to a simple time signature.
    ComplexTimeSignature();

    // Construct a metered time signature.
    ComplexTimeSignature(MeteredTimeSignature value);

    // Construct a senza-misura time signature.
    //
    // glyph is free text; MusicXML defines no valid/invalid values beyond xs:string (e.g. "X").
    ComplexTimeSignature(std::string glyph);

    Kind kind() const;
    bool isMetered() const;
    bool isSenzaMisura() const;

    // Returns a copy of the internally held MeteredTimeSignature.
    //
    // Precondition: isMetered(). If !isMetered(), a default constructed MeteredTimeSignature is
    // returned.
    const MeteredTimeSignature metered() const;

    // Returns a copy of the senza-misura glyph (which could be an empty string).
    //
    // Precondition: isSenzaMisura(). If !isSenzaMisura(), a default constructed (empty) string is
    // returned.
    const std::string senzaMisura() const;

    bool operator==(const ComplexTimeSignature &other) const;

  private:
    std::variant<MeteredTimeSignature, std::string> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(ComplexTimeSignature);

} // namespace api
} // namespace mx
