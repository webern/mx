// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
namespace api
{
enum class ClefSymbol
{
    none,
    g,
    f,
    c,
    percussion,
    tab,
    jianpu
};

enum class ClefLocation
{
    unspecified,
    beforeBarline,
    afterBarline,
    midMeasure
};

constexpr ClefSymbol DEFAULT_CLEF_SYMBOL = ClefSymbol::g;
constexpr int DEFAULT_CLEF_LINE = 2;
constexpr int DEFAULT_CLEF_OCTAVE_CHANGE = 0;

class ClefData
{
  public:
    ClefData();

    // Most users can ignore this; leave it unspecified. It only controls whether the clef's
    // optional number attribute is written. unspecified (the default) applies the right rule
    // automatically: omit the number on a single-staff part (where 1 is implied) and include it
    // otherwise. yes/no force the attribute on or off. It exists for round-trip fidelity - reading
    // a file sets yes/no only when the source diverged from the automatic rule.
    Bool writeStaffNumber;
    ClefSymbol symbol;
    int line;
    // When true (the default), the writer emits <line>. Set to false when the source
    // had no <line> element so the round-trip does not inject an implied default (#228).
    bool isLineSpecified;
    int octaveChange;
    bool isOctaveChangeSpecified;
    int tickTimePosition;
    ClefLocation location;
    // Visibility of the clef via the MusicXML print-object attribute.
    // unspecified -> omit the attribute, yes/no -> write print-object verbatim.
    Bool printObject;
    std::string toString() const;

    // convenience - set symbol, line and octave for common clefs
    void setTreble();
    void setBass();
    void setAlto();
    void setTenor();
    void setBaritone();
    void setSoprano();
    void setMezzoSoprano();

    // convenience - check symbol, line and octave for common clefs
    bool isTreble() const;
    bool isBass() const;
    bool isAlto() const;
    bool isTenor() const;
    bool isBaritone() const;
    bool isSoprano() const;
    bool isMezzoSoprano() const;
};

MXAPI_EQUALS_BEGIN(ClefData)
MXAPI_EQUALS_MEMBER(writeStaffNumber)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(line)
MXAPI_EQUALS_MEMBER(isLineSpecified)
MXAPI_EQUALS_MEMBER(octaveChange)
MXAPI_EQUALS_MEMBER(isOctaveChangeSpecified)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(location)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ClefData);
} // namespace api
} // namespace mx
