// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <optional>
#include <string>

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

// Where a clef sits relative to its measure. A clef at the start of a measure normally prints
// before the barline; beforeBarline and afterBarline choose between the two, which matters for cue
// clefs and for the clef that resumes after a repeated section. midMeasure describes a clef change
// partway through the measure, positioned by tickTimePosition. unspecified leaves the choice to the
// notation program.
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
    // Marks a supplementary clef placed on the staff alongside the regular one - a cue clef, or a
    // second clef in effect at the same time. MusicXML writes this as clef@additional="yes"; such a
    // clef sits at a non-standard line position, is not restated at the start of each system, and
    // notation software disregards its line. unspecified (the default) omits the attribute, which
    // describes an ordinary clef.
    Bool additional;
    // Visibility of the clef via the MusicXML print-object attribute.
    // unspecified -> omit the attribute, yes/no -> write print-object verbatim.
    Bool printObject;

    // The <clef> element's id attribute (see ApiCommon.h).
    std::optional<std::string> id;

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
MXAPI_EQUALS_MEMBER(additional)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ClefData);
} // namespace api
} // namespace mx
