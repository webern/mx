// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DurationData.h"
#include "mx/api/Id.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// in the tuplet ratio, for example 2:3 for
// triplets, the 'actual' part of the ratio
// is the '2' and the 'normal' part of the
// ratio is the '3'

// The shape of a tuplet's bracket. MusicXML calls the older, slur-like shape 'curved'.
enum class TupletLineShape
{
    unspecified,
    straight,
    curved
};

class TupletStart
{
  public:
    // Tells this tuplet apart from another one that is open at the same time, which happens when
    // one tuplet is written inside another. Leave it alone for a tuplet that stands on its own.
    // Give both ends of a tuplet the same identity when tuplets are nested, and mx numbers them
    // for you; an explicit level is written out as it stands.
    SpannerNumber number;

    PositionData positionData;

    int actualNumber;
    DurationName actualDurationName;
    int actualDots;

    int normalNumber;
    DurationName normalDurationName;
    int normalDots;

    // show the number of notes in the tuplet
    // description ratio, for example 3 for triplets
    Bool showActualNumber;

    // show the number of 'normal' notes in the
    // tuplet ratio, for example the 2 in 3:2 for
    // a triplet or the 4 in 5:4 for quintuplet
    Bool showNormalNumber;

    // Whether the note value of the tuplet's own notes is shown beside the bracket, for example the
    // eighth note in "3 eighths". This is optional. Left unspecified, the file does not say, and a
    // reader leaves the note values off, which is what MusicXML asks for. Set it to yes to show
    // them or to no to keep them off.
    Bool showActualType;

    // Whether the note value of the 'normal' notes is shown as well, for example the eighth note in
    // "in the time of 2 eighths". Also optional. MusicXML states both sides in a single attribute,
    // so this field only takes effect when showActualType is yes.
    Bool showNormalType;

    Bool bracket;

    // Whether the bracket is drawn as a straight line or in the older curved style.
    TupletLineShape lineShape;

    // Whether the actual and normal figures above are spelled out in the file next to the notes.
    // mx works this out on its own, so leave both unspecified; they are here for the rare case
    // where you want to decide instead.
    //
    // Unspecified spells out a figure when it says something the notes do not already say. Nested
    // tuplets are where that happens. When one tuplet is written inside another, each note counts
    // against both tuplets at once, so the note by itself cannot tell you the inner tuplet's own
    // ratio, and the tuplet has to state it. A tuplet standing on its own agrees with its notes and
    // is left plain. Setting yes spells the figure out anyway, and no leaves it out.
    //
    // Reading a file sets yes for each figure the file spelled out, so that a score read and
    // written back keeps what it had.
    Bool writeActual;
    Bool writeNormal;

    // The <tuplet> element's id attribute (see Id.h).
    std::optional<Id> id;

    TupletStart()
        : number{}, positionData{}, actualNumber{VALUE_UNSPECIFIED}, actualDurationName{api::DurationName::unspecified},
          actualDots{VALUE_UNSPECIFIED}, normalNumber{VALUE_UNSPECIFIED},
          normalDurationName{api::DurationName::unspecified}, normalDots{VALUE_UNSPECIFIED},
          showActualNumber{Bool::unspecified}, showNormalNumber{Bool::unspecified}, showActualType{Bool::unspecified},
          showNormalType{Bool::unspecified}, bracket{Bool::unspecified}, lineShape{TupletLineShape::unspecified},
          writeActual{Bool::unspecified}, writeNormal{Bool::unspecified}, id{}
    {
    }
};

class TupletStop
{
  public:
    // Matches the number of the TupletStart this ends. See TupletStart::number.
    SpannerNumber number;

    PositionData positionData;

    // Whether a bracket is drawn. MusicXML allows the attribute on both ends of a tuplet, and
    // some programs state it only on the stop.
    Bool bracket;

    // The <tuplet> element's id attribute (see Id.h).
    std::optional<Id> id;

    TupletStop() : number{}, positionData{}, bracket{Bool::unspecified}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(TupletStart)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(actualNumber)
MXAPI_EQUALS_MEMBER(actualDurationName)
MXAPI_EQUALS_MEMBER(actualDots)
MXAPI_EQUALS_MEMBER(normalNumber)
MXAPI_EQUALS_MEMBER(normalDurationName)
MXAPI_EQUALS_MEMBER(normalDots)
MXAPI_EQUALS_MEMBER(showActualNumber)
MXAPI_EQUALS_MEMBER(showNormalNumber)
MXAPI_EQUALS_MEMBER(showActualType)
MXAPI_EQUALS_MEMBER(showNormalType)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(lineShape)
MXAPI_EQUALS_MEMBER(writeActual)
MXAPI_EQUALS_MEMBER(writeNormal)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TupletStart);

MXAPI_EQUALS_BEGIN(TupletStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TupletStop);
} // namespace api
} // namespace mx
