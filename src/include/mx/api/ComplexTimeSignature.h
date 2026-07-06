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
// fractions, plus its decorations. A single fraction with no symbol, no separator, and no
// interchangeable is simple-equivalent and TimeChoice::complex collapses it back to the simple case;
// so a MeteredTimeSignature that survives as complex always carries at least one of: more than one
// fraction, an unusual symbol, a separator, or an interchangeable alternate.
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

// Everything unusual about a time signature, kept out of the way of the simple case. It is itself a
// choice: either a metered signature (composite meters, unusual symbols, separators, interchangeable
// dual meters) or senza-misura (unmetered music). The two are mutually exclusive in MusicXML, which
// this choice makes structurally true: a senza-misura signature cannot carry a symbol or an
// interchangeable, and a metered one cannot be senza-misura.
//
// Reached only through TimeChoice::asComplex(). Constructing one whose metered value is
// simple-equivalent yields a *simple* TimeChoice instead (auto-collapse), so a plain N/D can never
// hide in here.
class ComplexTimeSignature
{
  public:
    enum class Kind
    {
        metered,
        senzaMisura
    };

    // Defaults to a metered 4/4 (this default is never simple-equivalent-collapsed on its own; that
    // happens only when handed to TimeChoice::complex).
    ComplexTimeSignature();

    static ComplexTimeSignature metered(MeteredTimeSignature value);

    // The string is the senza-misura display glyph (often empty, sometimes "X").
    static ComplexTimeSignature senzaMisura(std::string glyph = {});

    Kind kind() const;
    bool isMetered() const;
    bool isSenzaMisura() const;

    // Precondition: isMetered().
    const MeteredTimeSignature &asMetered() const;

    // Precondition: isSenzaMisura(). The returned string is the display glyph (may be empty).
    const std::string &asSenzaMisura() const;

    bool operator==(const ComplexTimeSignature &other) const;

  private:
    std::variant<MeteredTimeSignature, std::string> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(ComplexTimeSignature);

} // namespace api
} // namespace mx
