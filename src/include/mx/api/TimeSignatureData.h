// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
namespace api
{

// The symbol drawn in place of, or alongside, a simple time signature. This is the narrow vocabulary
// that belongs to the simple case: an ordinary fraction, optionally shown with the C (common) or the
// slashed-C (cut) glyph. The weirder display modes (single-number, note, dotted-note) live only in
// the complex arena (see ComplexTimeSignature.h) and are not expressible here on purpose.
enum class TimeSignatureSymbol
{
    unspecified, // ordinary fractional display, e.g. 4/4 drawn as two numbers
    common,      // the C glyph (4/4)
    cut          // the slashed-C glyph (2/2)
};

// One beats/beat-type pair. The strings are free-form by design: they may hold compound values
// ("3+2"), decimals ("1 + 2.5"), or compound denominators ("8 + 16"). This is the shared leaf reused
// by the simple TimeSignatureData and by the composite/interchangeable meters in the complex arena;
// it carries no symbol, which is what lets it be shared across arenas whose symbol vocabularies differ.
struct TimeFraction
{
    // the top number of the time signature, e.g. "5" in "5/4" or "3+2" in "(3+2)/8"
    std::string beats;

    // the bottom number of the time signature, e.g. "4" in "5/4"
    std::string beatType;
};

MXAPI_EQUALS_BEGIN(TimeFraction)
MXAPI_EQUALS_MEMBER(beats)
MXAPI_EQUALS_MEMBER(beatType)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TimeFraction);

// The simple time signature: the ordinary N/D that you reach for 99% of the time, optionally shown
// with the common or cut glyph. Everything unusual (composite meters, senza-misura, interchangeable
// dual meters, and the single-number/note/dotted-note display modes) is quarantined in
// ComplexTimeSignature. Access a measure's time signature through TimeChoice (TimeChoice.h), which
// switches between this simple case and the complex one and holds the whole-<time> attributes
// (isImplicit, display).
//
// Defaults to 4/4 with no symbol.
struct TimeSignatureData
{
    TimeSignatureSymbol symbol{TimeSignatureSymbol::unspecified};
    TimeFraction fraction{"4", "4"};
};

MXAPI_EQUALS_BEGIN(TimeSignatureData)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(fraction)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TimeSignatureData);

} // namespace api
} // namespace mx
